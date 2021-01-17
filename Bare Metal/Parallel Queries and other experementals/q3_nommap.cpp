#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <time.h>


using namespace std;

#define check(expr) if (!(expr)) { perror(#expr); exit(1); }

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

bool sortByVal(const pair<int, float> &a, 
               const pair<int, float> &b) 
{ 
    return (a.second > b.second); 
} 

template<class T>
struct Vector {
   uint64_t count;
   int fd;
   T* data;

   Vector() : data(nullptr) {}
   Vector(const char *pathname) { readBinary(pathname); }
   ~Vector() { if (data) free(data); }

   void readBinary(const char *pathname) {
      fd=open(pathname,O_RDONLY);
      check(fd!=-1);
      struct stat sb;
      check(fstat(fd, &sb)!=-1);
      count=static_cast<uint64_t>(sb.st_size)/sizeof(T);
      // Replacement for mmap 
      void * buffer = malloc(count*sizeof(T)+1);
      read(fd,buffer, count*sizeof(T));
      data = reinterpret_cast<T*>(buffer);
   }

   uint64_t size() { return count; }
   T operator[](std::size_t idx) { return data[idx]; }
};

typedef std::string str; 

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
   ~Vector() { if (data) free(data); }

   void readBinary(const char *pathname) {
      fd=open(pathname,O_RDONLY);
      check(fd!=-1);
      struct stat sb;
      check(fstat(fd, &sb)!=-1);
      fileSize=static_cast<uint64_t>(sb.st_size);
      // Replacement for mmap 
      void * buffer = malloc(fileSize+1);
      read(fd,buffer, fileSize);
      data = reinterpret_cast<Data*>(buffer);
   }

   uint64_t size() { return data->count; }
   str operator[](std::size_t idx) { auto slot=data->slot[idx]; return str(reinterpret_cast<char*>(data)+slot.offset,slot.size); }
};

double wall0 = 0;
double cpu0  = 0;
double wall1 = 0;
double cpu1  = 0 ;

/*
 *@discription : TPC-H 3 implementierung 
 *@Argument    : definiert der scale factor 
 */
void calculate(float part) {

   Vector<str> l_shipdate("tpch1binary/l_shipdate");
   Vector<int> c_custkey("tpch1binary/c_custkey");
   Vector<int> o_orderkey("tpch1binary/o_orderkey");
   Vector<int> o_custkey("tpch1binary/o_custkey");
   Vector<str> c_mktsegment("tpch1binary/c_mktsegment");
   Vector<int> o_shippriority("tpch1binary/o_shippriority");
   Vector<str> o_orderdate("tpch1binary/o_orderdate");
   Vector<float> l_discount("tpch1binary/l_discount");
   Vector<int> l_orderkey("tpch1binary/l_orderkey");
   Vector<float> l_extendedprice("tpch1binary/l_extendedprice");
   

    //  Start Timers
    wall0 = get_wall_time();
    cpu0  = get_cpu_time();

    // Materilize the tables c_ and  o_
    std::unordered_map<int,int> hash_table; 
    for(int i = 0; i < c_custkey.size() * part; i++) {
        if(c_mktsegment[i] == BUILDING)
            hash_table[c_custkey[i]] = c_custkey[i]; 
    }

    // Join both tables c_custemkey 
    str BUILDING("BUILDING");
    std::unordered_map<int,int> resut_table_orderkey;
    std::unordered_map<int,int> resut_table_indexes;
    for (int i = 0 ; i < o_orderkey.size() * part; i++) {
        if(hash_table.find(o_custkey[i]) != hash_table.end() ) {
            
            resut_table_orderkey[i] = o_orderkey[i];
            resut_table_indexes[i] = i;
        }
    }
    hash_table.clear();
        
    std::unordered_map<int,float> revenue;
    std::unordered_map<int,int> result_shippriority;
    std::unordered_map<int,int> result_orderkey;
    std::unordered_map<int,str> result_orderdate;

    std::unordered_map<int,int> result_o_indexs;
    str DATUM("1995-03-15");

    int sum_index = 0;
    int l_orderkey_size = l_orderkey.size() * part; 
    // do the aggrigation  revenue += (l_extendedprice[i] * l_discount[i])
    for (int i = 0 ; i < l_orderkey_size -1; i++) {

        std::unordered_map<int, int>::iterator it = resut_table_orderkey.find(l_orderkey[i]);  
        if(it != resut_table_orderkey.end()) {
        int index = it->first;

            if(l_shipdate[i] >  DATUM) {
                if(o_orderdate[resut_table_indexes[index]] < DATUM) {
                    if( l_orderkey[i+1] == l_orderkey[i]) {
                    
                        revenue[sum_index] += (l_extendedprice[i] * l_discount[i]);
                    } else {
        
                        revenue[sum_index] += (l_extendedprice[i] * l_discount[i]); 
                        result_shippriority[sum_index] = o_shippriority[resut_table_indexes[index]];
                        result_orderkey[sum_index] = l_orderkey[i];
                        result_orderdate[sum_index] = o_orderdate[resut_table_indexes[index]];
                        sum_index++;
                    }
                }
            }
        }
    }

    resut_table_orderkey.clear();
    resut_table_indexes.clear();

    unordered_map<int, float> :: iterator it;
    vector<pair<int, float>> vec;

    for (it=revenue.begin(); it!=revenue.end(); it++) {
        vec.push_back(make_pair(it->first, it->second));
    }
    
    // sort 
    std::make_heap(vec.begin(), vec.end(), sortByVal); 
    std::sort_heap(vec.begin(), vec.end(), sortByVal); 
    int LIMIT = 10;

    //  Stop timers
    wall1 = get_wall_time();
    cpu1  = get_cpu_time();

    // print the results
    printf("%-15s%-15s%-15s%-15s\n", "-----------------", "----------------", "----------------","----------------");
    printf("|%-15s|%-15s|%-15s|%-15s|\n", "l_orderkey", "revenue", "o_orderdare","o_shippriority");
    printf("%-15s%-15s%-15s%-15s\n", "-----------------", "----------------", "----------------","----------------");
    
    for(int i = 0 ; i < vec.size() && LIMIT > 0; i++) {
        printf("|%-15d|%-15.3f|%-15d|\n",result_orderkey[vec[i].first], revenue[vec[i].first] /*result_orderdate[vec[i].first].data()*/,result_shippriority[vec[i].first]);
        LIMIT--;
    }
    printf("%-15s%-15s%-15s%-15s\n", "-----------------", "----------------", "----------------","----------------");
    
    cout << "Wall Time for part= "<< part<<" is :"<< wall1 - wall0 << endl;
    cout << "CPU Time for  part= "<< part<<" is :"<< cpu1  - cpu0  << endl;
}

int main() {
   
   float  parts [10] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
   for( int i = 0 ; i< 10 ; i++)
      calculate(parts[i]);
    
   return 0;
}

