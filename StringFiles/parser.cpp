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
#include <ctime>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <fstream>
// #include <emscripten.h>

using namespace std;
typedef std::string str;


bool sortByVal(const pair<str, int> &a, 
               const pair<str, int> &b) 
{ 
    return ((a.first).compare(b.first) <= 0); 
} 

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

int main() {


    Vector<str> c_mktsegment("tpch1binary/c_mktsegment");//
    ofstream c_mktsegment_text;//
    c_mktsegment_text.open("tpch1binary/c_mktsegment_text");


    for(int i = 0 ; i < c_mktsegment.size(); i++) {
        c_mktsegment_text << c_mktsegment[i]<< "\n";
    }
    c_mktsegment_text.close();


    Vector<str> l_returnflag("tpch1binary/l_returnflag");//
    Vector<str> l_linestatus("tpch1binary/l_linestatus");//
    Vector<str> l_shipdate("tpch1binary/l_shipdate");//
    Vector<str> l_commitdate("tpch1binary/l_commitdate");//
    Vector<str> l_receiptdate("tpch1binary/l_receiptdate");//
    Vector<str> l_shipinstruct("tpch1binary/l_shipinstruct");//
    Vector<str> l_shipmode("tpch1binary/l_shipmode");//
    Vector<str> l_comment("tpch1binary/l_comment");//
    
    Vector<str> r_name("tpch1binary/r_name");
    
    Vector<str> n_name("tpch1binary/n_name");
    
    Vector<str> s_name("tpch1binary/s_name"); 
    Vector<str> s_address("tpch1binary/s_address"); 
    Vector<str> s_phone("tpch1binary/s_phone"); 
    Vector<str> s_comment("tpch1binary/s_comment"); 
    
    Vector<str> p_mfgr("tpch1binary/p_mfgr"); 
    Vector<str> p_type("tpch1binary/p_type"); 
    
    Vector<str> o_orderdate("tpch1binary/o_orderdate");
    Vector<str> o_clerk("tpch1binary/o_clerk");
    Vector<str> o_orderstatus("tpch1binary/o_orderstatus");
    Vector<str> o_orderpriority("tpch1binary/o_orderpriority");
    Vector<str> o_comment("tpch1binary/o_comment");


    ofstream l_returnflag_text;//
    l_returnflag_text.open("tpch1binary/l_returnflag_text");
    ofstream l_linestatus_text;//
    l_linestatus_text.open("tpch1binary/l_linestatus_text");
    ofstream l_shipdate_text;//
    l_shipdate_text.open("tpch1binary/l_shipdate_text");
    ofstream l_commitdate_text;//
     l_commitdate_text.open("tpch1binary/l_commitdate_text");
    ofstream l_receiptdate_text;//
     l_receiptdate_text.open("tpch1binary/l_receiptdate_text");
    ofstream l_shipinstruct_text;//
     l_shipinstruct_text.open("tpch1binary/l_shipinstruct_text");
    ofstream l_shipmode_text;//
     l_shipmode_text.open("tpch1binary/l_shipmode_text");
    ofstream l_comment_text;//
     l_comment_text.open("tpch1binary/l_comment_text");
    
    for(int i = 0 ; i < l_returnflag.size(); i++) {
        l_returnflag_text << l_returnflag[i]<< "\n";
        l_linestatus_text << l_linestatus[i]<< "\n";
        l_shipdate_text << l_shipdate[i]<< "\n";
        l_commitdate_text << l_commitdate[i]<< "\n";
        l_receiptdate_text << l_receiptdate[i]<< "\n";
        l_shipinstruct_text << l_shipinstruct[i]<< "\n";
        l_shipmode_text << l_shipmode[i]<< "\n";
        l_comment_text << l_comment[i]<< "\n";
    }

    l_returnflag_text.close();
    l_linestatus_text.close();
    l_shipdate_text.close();
    l_commitdate_text.close();
    l_receiptdate_text.close();
    l_shipinstruct_text.close();
    l_shipmode_text.close();
    l_comment_text.close();

    ofstream r_name_text;
     r_name_text.open("tpch1binary/r_name_text");


    for(int i = 0 ; i < r_name.size(); i++) {
        r_name_text << r_name[i]<< "\n";
    }
    r_name_text.close();

    ofstream n_name_text;
     n_name_text.open("tpch1binary/n_name_text");
    
    for(int i = 0 ; i < n_name.size(); i++) {
        n_name_text << n_name[i]<< "\n";
    }
    
    n_name_text.close();



    ofstream s_name_text; 
     s_name_text.open("tpch1binary/s_name_text"); 
    ofstream s_address_text; 
     s_address_text.open("tpch1binary/s_address_text"); 
    ofstream s_phone_text;
     s_phone_text.open("tpch1binary/s_phone_text");  
    ofstream s_comment_text; 
     s_comment_text.open("tpch1binary/s_comment_text"); 
  
    for(int i = 0 ; i < s_name.size(); i++) {

        s_name_text    << s_name[i]   << "\n";
        s_address_text << s_address[i]<< "\n";
        s_phone_text   << s_phone[i]  << "\n";
        s_comment_text << s_comment[i]<< "\n";
    }
    
    s_name_text.close();
    s_address_text.close();
    s_phone_text.close();
    s_comment_text.close();

    ofstream p_mfgr_text; 
     p_mfgr_text.open("tpch1binary/p_mfgr_text"); 
    ofstream p_type_text; 
     p_type_text.open("tpch1binary/p_type_text"); 

    for(int i = 0 ; i < p_type.size(); i++) {
        p_type_text << p_type[i]<< "\n";
        p_mfgr_text    << p_mfgr[i]   << "\n";
    }

    p_type_text.close();
    p_mfgr_text.close();

    ofstream o_orderdate_text;
      o_orderdate_text.open("tpch1binary/o_orderdate_text");
    ofstream o_clerk_text;
     o_clerk_text.open("tpch1binary/o_clerk_text");
    ofstream o_orderstatus_text;
     o_orderstatus_text.open("tpch1binary/o_orderstatus_text");
    ofstream o_orderpriority_text;
     o_orderpriority_text.open("tpch1binary/o_orderpriority_text");
    ofstream o_comment_text;
     o_comment_text.open("tpch1binary/o_comment_text");

    for(int i = 0 ; i < o_orderdate.size(); i++) {

        o_orderdate_text << o_orderdate[i]<< "\n";
        o_clerk_text << o_clerk[i]<< "\n";
        o_orderstatus_text << o_orderstatus[i]<< "\n";
        o_orderpriority_text << o_orderpriority[i]<< "\n";
        o_comment_text << o_comment[i]<< "\n";
    }

    o_orderdate_text.close();
    o_orderdate_text.close();
    o_orderstatus_text.close();
    o_orderpriority_text.close();
    o_comment_text.close();

    return 0;
}