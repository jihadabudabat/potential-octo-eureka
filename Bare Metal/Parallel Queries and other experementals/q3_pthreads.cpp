
#include <cassert>
#include <cstring>
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
#include "PerfEvent.hpp"
#include <sys/time.h>
#include <time.h>
#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <atomic>

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
 * This implementation uses Pthreads to parallel the Queries
 */
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

std::unordered_map<int,int> hash_table; 
std::unordered_map<int,int> resut_table_orderkey;
std::unordered_map<int,int> resut_table_indexes;
std::unordered_map<int,int> result_o_indexs;
std::unordered_map<int,float> revenue;
std::unordered_map<int,int> result_shippriority;
std::unordered_map<int,int> result_orderkey;
std::unordered_map<int,str> result_orderdate;
std::unordered_map<int, float>::iterator it;
std::vector<pair<int, float>> vec;

str BUILDING("BUILDING");
str DATUM("1995-03-15");
int sum_index = 0;

// Struct to pass to pthreads as an argument for the function
struct arguments {
    unsigned int o_start;
    unsigned int c_start;
    unsigned int l_start;
    unsigned int o_end;
    unsigned int c_end;
    unsigned int l_end;
}; 

std::atomic<unsigned int> x;
pthread_barrier_t barrier ;
pthread_mutex_t lock; 

int LIMIT = 10;       

// Voind function to be parallized using Pthreads
// This is the implementation of Pthreds in parallel
void * hash_join(void * args) {

    struct arguments * args_  = (arguments *) args;
    unsigned int o_start = args_->o_start;
    unsigned int c_start= args_->c_start;
    unsigned int l_start= args_->l_start;
    unsigned int o_end= args_->o_end;
    unsigned int c_end= args_->c_end;
    unsigned int l_end= args_->l_end;

    // Materilize the Table C_
    for(unsigned int i = c_start ; i < c_end; i++ ) {
        
        if(c_mktsegment[i] == BUILDING) {
            pthread_mutex_lock(&lock);
            hash_table[c_custkey[i]] = c_custkey[i]; 
            pthread_mutex_unlock(&lock); 
        }
    }

    // Join the tables O_ and C_
    for (unsigned int i = o_start ; i < o_end; i++) {
        if(hash_table.find(c_custkey[i]) != hash_table.end() ) {
            
            pthread_mutex_lock(&lock);
            resut_table_orderkey[i] = o_orderkey[i];
            pthread_mutex_unlock(&lock); 
            pthread_mutex_lock(&lock);
            resut_table_indexes[i] = i;
            pthread_mutex_unlock(&lock); 
        }
    }

     // Do the aggrigation   
    for (unsigned int i = l_start ; i < l_end -1; i++) {
        
        pthread_mutex_lock(&lock);
        std::unordered_map<int, int>::iterator it = resut_table_orderkey.find(l_orderkey[i]);  
        if(it != resut_table_orderkey.end()) {
        int index = it->first;

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
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}
/*
 *@discription : TPC-H 3 implementierung  
 */
int main() {
    
    PerfEvent e;
    e.startCounters();
    //  Start Timers
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();

    struct arguments args[4];
    
    size_t l_len = l_discount.size();
    size_t c_len = c_custkey.size();
    size_t o_len = o_orderkey.size();
    
    // create the parts to pass to the threads
    for(int i = 0 ; i < 4; i++ ) {

        size_t start = (l_len/4) * i;
        args[i].l_start   = start;
        args[i].l_end   = (l_len/4)+ start;

        start = (c_len/4) * i;
        args[i].c_start = start;
        args[i].c_end   = (c_len/4)+ start;

        start = (o_len/4) * i;
        args[i].o_start = start;
        args[i].o_end   = (o_len/4)+ start;
    }
    
    // Number of cores
    pthread_t th1;
    pthread_t th2;
    pthread_t th3;
    pthread_t th4;

    // create the threads
    pthread_create(&th1, NULL, &hash_join, &args[0] );
    pthread_create(&th2, NULL, &hash_join, &args[1] );
    pthread_create(&th3, NULL, &hash_join, &args[2] );
    pthread_create(&th4, NULL, &hash_join, &args[3] );
 
    // Join the threads
    pthread_join( th4, NULL );
    pthread_join( th3, NULL );
    pthread_join( th2, NULL );
    pthread_join( th1, NULL );

    //  Stop timers
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();
    
    
    for (it=revenue.begin(); it!=revenue.end(); it++) {
        vec.push_back(make_pair(it->first, it->second));
    }
    // Sort
    std::make_heap(vec.begin(), vec.end(), sortByVal); 
    std::sort_heap(vec.begin(), vec.end(), sortByVal); 

    
    resut_table_orderkey.clear();
    resut_table_indexes.clear();

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

    std::cout << std::endl;
    
    return 0;
}


