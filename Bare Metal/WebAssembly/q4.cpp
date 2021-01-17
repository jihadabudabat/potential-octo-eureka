#include <cassert>
#include <cstring>
#include <algorithm> 
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <unordered_map>
#include <string>     
#include <sys/time.h>
#include <time.h>   
//#include <emscripten.h>

using namespace std;
typedef std::string str;

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        perror(" Exception get Time");
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

#define check(expr) if (!(expr)) { perror(#expr); exit(1); }
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

template<class T>
struct Vector {
   uint64_t count;
   int fd;
   T* data;

   Vector() : data(nullptr) {}
   Vector(const char *pathname) { readBinary(pathname); }
   ~Vector() { if (data) check(munmap(data,count*sizeof(T))==0); }

   void readBinary(const char *pathname) {
      fd=open(pathname,O_RDONLY);
      check(fd!=-1);
      struct stat sb;
      check(fstat(fd, &sb)!=-1);
      count=static_cast<uint64_t>(sb.st_size)/sizeof(T);
      data=reinterpret_cast<T*>(mmap(nullptr, count*sizeof(T), PROT_READ, MAP_PRIVATE, fd, 0));
      check(data!=MAP_FAILED);
   }

   uint64_t size() { return count; }
   T operator[](std::size_t idx) { return data[idx]; }
};


template<>
struct Vector<str> {
  struct Data {
     uint64_t count;
     struct {
        uint64_t size;
        uint64_t offset;
     } slot[];
  };

   uint64_t fileSize;
   int fd;
   Data* data;

   Vector() : data(nullptr) {}
   Vector(const char *pathname) { readBinary(pathname); }
   ~Vector() { if (data) check(munmap(data,fileSize)==0); }

   void readBinary(const char *pathname) {
      fd=open(pathname,O_RDONLY);
      check(fd!=-1);
      struct stat sb;
      check(fstat(fd, &sb)!=-1);
      fileSize=static_cast<uint64_t>(sb.st_size);
      data=reinterpret_cast<Data*>(mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0));
      check(data!=MAP_FAILED);
   }

   uint64_t size() { return data->count; }
   str operator[](std::size_t idx) { auto slot=data->slot[idx]; return str(reinterpret_cast<char*>(data)+slot.offset,slot.size); }
};

double wall0 = 0;
double cpu0  = 0;
double wall1 = 0;
double cpu1  = 0;


/*
 *@discription : TPC-H 4 implementierung 
 *@Argument    : definiert der scale factor 
 */
void calculate(float part) {

   /*
      select o_orderpriority, count(*) as order_count from ORDERS 
      no joins
      month and exists (select * from LINEITEM where l_orderkey = o_orderkey and l_commitdate < l_receiptdate) 
      group by o_orderpriority order by o_orderpriority;
   */
   //  Start Timers
   wall0 = get_wall_time();
   cpu0  = get_cpu_time();

   Vector<int> o_orderkey("tpch1binary/o_orderkey");
   Vector<str> o_orderdate("tpch1binary/o_orderdate");
   Vector<str> o_orderpriority("tpch1binary/o_orderpriority");

   Vector<int> l_orderkey("tpch1binary/l_orderkey");
   Vector<str> l_commitdate("tpch1binary/l_commitdate");
   Vector<str> l_receiptdate("tpch1binary/l_receiptdate");

   //  Stop timers
   wall1 = get_wall_time();
   cpu1  = get_cpu_time();

   cout << "Wall Time for Loading Data took:"<< wall1 - wall0 << endl;
   cout << "CPU Time for  Loading Data took:"<< cpu1  - cpu0  << endl;
   
    //  Start Timers
    wall0 = get_wall_time();
    cpu0  = get_cpu_time();

   // materialize the table o_
   std::unordered_map<int,int> o_orderkey_mat; 
   str date_1("1995-01-01");
   str date_2("1995-04-01");
   for(int i = 0 ; i< o_orderkey.size() *part; i++) {
      if(o_orderdate[i] >= date_1 && o_orderdate[i] <= date_2) {

         o_orderkey_mat[o_orderkey[i]] = i; 
      }
   }

   std::vector<str> O_ORDERPRIORETY;
   //(select * from LINEITEM where l_orderkey = o_orderkey and l_commitdate < l_receiptdate)
   std::unordered_map<int, int> o_orderkey_exsists; 
   for(int i = 0 ; i< l_orderkey.size()*part; i++) {
      if(l_commitdate[i] < l_receiptdate[i] && o_orderkey_mat.find(l_orderkey[i]) != o_orderkey_mat.end()) {
            
            int key = o_orderkey_mat.find(l_orderkey[i])->first;
            int index = o_orderkey_mat.find(l_orderkey[i])->second;
            
            o_orderkey_exsists[key] = key;
            O_ORDERPRIORETY.push_back(o_orderpriority[index]);
      }
   }

   // Sort 
   std::make_heap(O_ORDERPRIORETY.begin(), O_ORDERPRIORETY.end());
   std::sort_heap(O_ORDERPRIORETY.begin(), O_ORDERPRIORETY.end());

   std::unordered_map<str, int> result;
   int count = 0;
   // The aggrigation count(*) 
   for(int i = 0 ; i < O_ORDERPRIORETY.size() -1; i++) {

      if(O_ORDERPRIORETY[i] == O_ORDERPRIORETY[i+1])
         count++;
      else {
         count++;
         result[O_ORDERPRIORETY[i]] = count;
         count = 0 ;
      }
   }
   
   //  Stop timers
   wall1 = get_wall_time();
   cpu1  = get_cpu_time();

   cout << "Wall Time for part= "<< part<<" is :"<< wall1 - wall0 << endl;
   cout << "CPU Time for  part= "<< part<<" is :"<< cpu1  - cpu0  << endl;

   printf("%-13s%-13s\n", "-------------","----------------");
   printf("|%-17s|%-9s|\n", "O_ORDERPRIORETY", "count(*)");
   printf("%-13s%-13s\n", "-------------","----------------");
   for (auto& it: result) {
      
      printf("|%-17s|%-9d|\n",it.first.c_str(), it.second);
   }
   printf("%-13s%-13s\n", "-------------","----------------");

}
int main() {
   
   // Parts for the sacle factor test
   float  parts [10] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
   for( int i = 0 ; i< 10 ; i++)
      calculate(parts[i]);

   return 0;
}

