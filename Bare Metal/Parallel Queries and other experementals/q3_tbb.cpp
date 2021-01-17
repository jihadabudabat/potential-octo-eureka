#include <cassert>
#include <cstring>
#include <experimental/string_view>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <execution>
#include "PerfEvent.hpp"
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <exception>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/parallel_for.h>
#include <tbb/task_arena.h>
#include <tbb/concurrent_vector.h>

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

//typedef char* str;
typedef std::experimental::string_view str;

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

 
/*
* TPC-H 3 implementation using tbb library
*
*/
int main() {
   
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
   

    PerfEvent e;
    e.startCounters();
    //  Start Timers
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();

    tbb::concurrent_unordered_map<int,int> hash_table;
    // Materilize C_
    size_t size = c_custkey.size(); 
    tbb::parallel_for(tbb::blocked_range<uint64_t>(0, size),[&](const tbb::blocked_range<uint64_t>& range) {
        
        #pragma simd 
        for(uint64_t i=range.begin(); i<range.end(); i++) {    
            if( c_mktsegment[i] == BUILDING )
                hash_table[c_custkey[i]] = c_custkey[i]; 
        }             
    });
    

    // Join the tables C_ and O_ 
    str BUILDING("BUILDING");
    tbb::concurrent_unordered_map<int,int> resut_table_orderkey;
    tbb::concurrent_unordered_map<int,int> resut_table_indexes;
    size = o_orderkey.size();
    tbb::parallel_for(tbb::blocked_range<uint64_t>(0, size),[&](const tbb::blocked_range<uint64_t>& range) {
        
        #pragma simd 
        for (uint64_t i=range.begin(); i<range.end(); i++) {
            if(hash_table.find(o_custkey[i]) != hash_table.end() ) {
                resut_table_orderkey[i] = o_orderkey[i];
                resut_table_indexes[i] = i;
            }
        }
    });

    hash_table.clear();
        
    tbb::concurrent_unordered_map<int,float> revenue;
    tbb::concurrent_unordered_map<int,int> result_shippriority;
    tbb::concurrent_unordered_map<int,int> result_orderkey;
    tbb::concurrent_unordered_map<int,str> result_orderdate;

    tbb::concurrent_unordered_map<int,int> result_o_indexs;
    str DATUM("1995-03-15");

    int sum_index = 0;
    int l_orderkey_size = l_orderkey.size(); 

    // Aggrigate : revenue += l_extendedprice[i] * l_discount[i]
    tbb::parallel_for(tbb::blocked_range<uint64_t>(0, l_orderkey_size -1),[&](const tbb::blocked_range<uint64_t>& range) {
        for (uint64_t i=range.begin(); i<range.end(); i++) {

            tbb::concurrent_unordered_map<int, int>::iterator it = resut_table_orderkey.find(l_orderkey[i]);  
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
    });

    resut_table_orderkey.clear();
    resut_table_indexes.clear();
    
    tbb::concurrent_unordered_map<int, float> :: iterator it;
    tbb::concurrent_vector<pair<int, float>> vec;

    for (it=revenue.begin(); it!=revenue.end(); it++) {
        vec.push_back(make_pair(it->first, it->second));
    }
     // Sort
    std::sort(std::execution::par, vec.begin(), vec.end(), sortByVal); 
    int LIMIT = 10;

    //  Stop timers
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();

    printf("%-15s%-15s%-15s%-15s\n", "-----------------", "----------------", "----------------","----------------");
    printf("|%-15s|%-15s|%-15s|%-15s|\n", "l_orderkey", "revenue", "o_orderdare","o_shippriority");
    printf("%-15s%-15s%-15s%-15s\n", "-----------------", "----------------", "----------------","----------------");
    
    for(int i = 0 ; i < vec.size() && LIMIT > 0; i++) {
        printf("|%-15d|%-15.3f|%-15s|%-15d|\n",result_orderkey[vec[i].first], revenue[vec[i].first], result_orderdate[vec[i].first].data(),result_shippriority[vec[i].first]);
        LIMIT--;
    }
    printf("%-15s%-15s%-15s%-15s\n", "-----------------", "----------------", "----------------","----------------");
    
    cout << "Wall Time = " << wall1 - wall0 << endl;
    cout << "CPU Time  = " << cpu1  - cpu0  << endl;
    
    e.stopCounters();
    e.printReport(std::cout, 1);
    /**/
    std::cout << std::endl;
    
    return 0;
}

