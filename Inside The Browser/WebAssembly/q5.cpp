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

bool sortByVal(const pair<str, int> &a, 
               const pair<str, int> &b) 
{ 
    return (a.first > b.first); 
} 

bool sortByValFloat(const pair<float, str> &a, 
               const pair<float, str> &b) 
{ 
    return (a.first > b.first); 
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
double cpu1  = 0 ;


/*
 *@discription : TPC-H 5 implementierung 
 *@Argument    : definiert der scale factor 
 */
void calculate(float part) {
   // http://hyper-db.com/
    
   /*
      SELECT N_NAME, SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)) AS REVENUE
      FROM CUSTOMER, ORDERS, LINEITEM, SUPPLIER, NATION, REGION
      WHERE C_CUSTKEY = O_CUSTKEY AND L_ORDERKEY = O_ORDERKEY AND L_SUPPKEY = S_SUPPKEY
      AND C_NATIONKEY = S_NATIONKEY AND S_NATIONKEY = N_NATIONKEY AND N_REGIONKEY = R_REGIONKEY
      AND R_NAME = 'ASIA' AND O_ORDERDATE >= '1994-01-01'
      AND O_ORDERDATE < DATEADD(YY, 1, cast('1994-01-01' as date))
      GROUP BY N_NAME
      ORDER BY REVENUE DESC
   */
   
   //  Stop timers
   wall0 = get_wall_time();
   cpu0  = get_cpu_time();

   Vector<int> s_suppkey("tpch1binary/s_suppkey");
   Vector<int> s_nationkey("tpch1binary/s_nationkey");

   Vector<int> c_custkey("tpch1binary/c_custkey");
   Vector<int> c_nationkey("tpch1binary/c_nationkey");

   Vector<int> o_orderkey("tpch1binary/o_orderkey");
   Vector<int> o_custkey("tpch1binary/o_custkey");
   Vector<str> o_orderdate("tpch1binary/o_orderdate");

   Vector<int> l_orderkey("tpch1binary/l_orderkey");
   Vector<int> l_suppkey("tpch1binary/l_suppkey");
   Vector<float> l_extendedprice("tpch1binary/l_extendedprice");
   Vector<float> l_discount("tpch1binary/l_discount");

   Vector<int> n_nationkey("tpch1binary/n_nationkey");
   Vector<str> n_name("tpch1binary/n_name");
   Vector<int> n_regionkey("tpch1binary/n_regionkey");
   Vector<int> r_regionkey("tpch1binary/r_regionkey");
   Vector<str> r_name("tpch1binary/r_name");
   
   //  Stop timers
   wall1 = get_wall_time();
   cpu1  = get_cpu_time();

   cout << "Wall Time for Loading Data took:"<< wall1 - wall0 << endl;
   cout << "CPU Time for  Loading Data took:"<< cpu1  - cpu0  << endl;

   //  Stop timers
   wall0 = get_wall_time();
   cpu0  = get_cpu_time();

   std::unordered_map<int , int> o_materialization;
   std::unordered_map<int , int> r_materialization;
   std::unordered_map<int , int> c_materialization;
   std::unordered_map<int , int> s_materialization;
   std::unordered_map<int , int> n_materialization;
   std::unordered_map<int , int> c_n_key_materialization;

   str ASIA("ASIA");
   str DATUM_1("1994-01-01");
   str DATUM_2("1995-01-01");
   
   //materialize the table r_
   for(int i = 0 ; i < r_name.size(); i++) {
      if(r_name[i] == ASIA) {
         r_materialization[r_regionkey[i]] = i;
      }
   }

   //materialize the table o_
   for(int i = 0 ; i < o_orderkey.size()*part; i++) {
      if(o_orderdate[i] > DATUM_1 && o_orderdate[i] < DATUM_2) {

         o_materialization[o_orderkey[i]] = i;
      }
   }
   
   //materialize the table c_
   for(int i = 0; i< c_custkey.size()*part; i++) {
      c_materialization[c_custkey[i]] = i; 
      c_n_key_materialization[c_nationkey[i]] = i ; 
   }

   //materialize the table s_
   for(int i = 0; i< s_suppkey.size()*part; i++) {
      s_materialization[s_suppkey[i]] = i;   
   }

   //materialize the table n_
   for(int i = 0; i< n_nationkey.size(); i++) {
      n_materialization[n_nationkey[i]] = i;   
   }

   std::vector<pair<str,int>> N_NAME;
   std::vector<float> L_EXTENDEDPRICE;
   std::vector<float> L_DISCOUNT;

   //  Join the tables
   int result_index = 0 ;
   for(int i = 0 ; i <  l_orderkey.size()*part; i++ ) {

      if(o_materialization.find(l_orderkey[i]) != o_materialization.end() && s_materialization.find(l_suppkey[i]) != s_materialization.end()) {
         int key_o   = o_materialization.find(l_orderkey[i])->first;
         int index_o = o_materialization.find(l_orderkey[i])->second;
         int key_s   = s_materialization.find(l_suppkey[i])->first;
         int index_s = s_materialization.find(l_suppkey[i])->second;

         if(c_materialization.find(o_custkey[index_o]) != c_materialization.end()
            && c_n_key_materialization.find(s_nationkey[index_s]) != c_n_key_materialization.end()) {
            
            int c_key   = c_materialization.find(o_custkey[index_o])->first;
            int c_index = c_materialization.find(o_custkey[index_o])->second;
            int n_key   = c_n_key_materialization.find(s_nationkey[index_s])-> first; 

            if(n_materialization.find(n_key) != n_materialization.end()) {

               int n_index = n_materialization.find(n_key)->first;
               
               if(r_materialization.find(n_regionkey[n_index]) != r_materialization.end()) {

                  int index_r = r_materialization.find(n_regionkey[n_index])->second;
                  N_NAME.push_back(make_pair(n_name[n_index], result_index));
                  L_EXTENDEDPRICE.push_back(l_extendedprice[i]);
                  L_DISCOUNT.push_back(l_discount[i]);    
                  result_index++;
               }

            }
         }
      }
   }

   //Clear the used columns
   o_materialization.clear();
   r_materialization.clear();
   c_materialization.clear();
   s_materialization.clear();
   n_materialization.clear();
   c_n_key_materialization.clear();

   std::vector<pair<float,str>> REVENUE; // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT))
   // sort
   std::make_heap(N_NAME.begin(), N_NAME.end(), sortByVal);
   std::sort_heap(N_NAME.begin(), N_NAME.end(), sortByVal);
   // The Aggrigation  --> REVENUE
   float REVENUE_ = 0 ; 
   if(N_NAME.size() > 0) {
      for(int i = 0 ; i < N_NAME.size()-1; i++ ) {

         int index = N_NAME[i].second;
         if(N_NAME[i].first == N_NAME[i+1].first) {

            REVENUE_ += (L_EXTENDEDPRICE[index] * L_DISCOUNT[index]);
         }else {

            REVENUE_ += (L_EXTENDEDPRICE[index] * L_DISCOUNT[index]);
            REVENUE.push_back(make_pair(REVENUE_, N_NAME[i].first));
            REVENUE_ = 0 ;
         }

      }
   }

   std::make_heap(REVENUE.begin(), REVENUE.end(), sortByValFloat);
   std::sort_heap(REVENUE.begin(), REVENUE.end(), sortByValFloat);

    //  Stop timers
    wall1 = get_wall_time();
    cpu1  = get_cpu_time();

   cout << "Wall Time for part= "<< part<<" is :"<< wall1 - wall0 << endl;
   cout << "CPU Time for  part= "<< part<<" is :"<< cpu1  - cpu0  << endl;

   printf("%-9s%-17s\n", "-------------","----------------");
   printf("|%-9s|%-17s|\n", "N_NAME", "REVENUE");
   printf("%-9s%-17s\n", "-------------","----------------");
   for (auto& it: REVENUE) {
      
      printf("|%-9s|%-17f|\n",it.second.c_str(), it.first);
   }
   printf("%-9s%-17s\n", "-------------","----------------");
}

int main() {

   // Parts for the sacle factor test
   float  parts [10] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
   for( int i = 0 ; i< 10 ; i++)
      calculate(parts[i]);

   return 0;
}

