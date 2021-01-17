/**
 * @param {*} type  the input type for the binary  = [float , str]
 * @param {*} url   the path to the files
 */
async function fetchLocal(type,url) {
   return new Promise(function(resolve, reject) {
     var xhr = new XMLHttpRequest
       if ( type == "int" || type == "float") {
           xhr.responseType = "arraybuffer";
       }else {

           xhr.responseType = "text";
       }
         
     xhr.onload = function() {
       resolve( xhr.response )
     }
     xhr.onerror = function() {
       reject(new TypeError('Local request failed'));
     }
     xhr.open('GET', url)
     xhr.send(null)
   })
 }

async function readFile(src, type , packageName) {

   var data_ = await fetchLocal(type,src+packageName);
   switch( type){
       case "int":
           return await new Int32Array(data_); 
       case "float":
           return await new Float32Array(data_);
       case "str" :
           return  data_.split("\n");
   }  
}

/**
 * Implementation for TPC-H 4
 * @param {int} part for the scale factor test
 */
async function calculate_q4(part) {
  /*
      select o_orderpriority, count(*) as order_count from ORDERS 
      no joins
      month and exists (select * from LINEITEM where l_orderkey = o_orderkey and l_commitdate < l_receiptdate) 
      group by o_orderpriority order by o_orderpriority;
   */
  let t0 = performance.now();

  let src = "./tpch1binary/";
  let o_orderkey= await readFile(src, "int","o_orderkey");
  let o_orderdate= await readFile(src, "str","o_orderdate_text");
  let o_orderpriority= await readFile(src, "str","o_orderpriority_text");

  let l_orderkey= await readFile(src, "int","l_orderkey");
  let l_commitdate= await readFile(src, "str","l_commitdate_text");
  let l_receiptdate= await readFile(src, "str","l_receiptdate_text");
  let t1 = performance.now();
  console.log("Query 4 loading "+ part +"  took "+ (Math.round(((t1 - t0) + Number.EPSILON) * 1000) / 1000)+ " ms");
  t0 = performance.now();

  
  // materialize the table o_
  let o_orderkey_mat = new Map(); 
  let date_1 = new Date("1995-01-01");
  let date_2 = new Date("1995-04-01");
  for(let i = 0 ; i< o_orderkey.length * part; i++) {
     if(new Date(o_orderdate[i]).getTime() >= date_1.getTime() 
     && new Date(o_orderdate[i]).getTime() <= date_2.getTime()) {

        o_orderkey_mat.set(o_orderkey[i], i); 
     }
  }

  let O_ORDERPRIORETY = [];
  //(select * from LINEITEM where l_orderkey = o_orderkey and l_commitdate < l_receiptdate)
  let o_orderkey_exsists = new Map(); 
  for(let i = 0 ; i< l_orderkey.length*part; i++) {
     if(new Date(l_commitdate[i]).getTime() < new Date(l_receiptdate[i]).getTime() && o_orderkey_mat.has(l_orderkey[i])) {
           
           let key   = l_orderkey[i];
           let index = o_orderkey_mat.has(l_orderkey[i]);
           
           o_orderkey_exsists.set(key, key);
           O_ORDERPRIORETY.push(o_orderpriority[index]);
     }
  }

  // Sort 
  heapSort(O_ORDERPRIORETY);

  let result = new Map();
  let count = 0;
  // the aggrigation count(*)
  for(let i = 0 ; i < O_ORDERPRIORETY.length -1; i++) {

      // if the current equals the next --> count
     if(O_ORDERPRIORETY[i] == O_ORDERPRIORETY[i+1])
        count++;
     else {
        // else count the last element
        count++;
        // the result
        result[O_ORDERPRIORETY[i]] = count;
        count = 0 ;
     }
  }
 
   t1 = performance.now();
  console.log("Query 4 with scale factor "+ part +"  took "+ (Math.round(((t1 - t0) + Number.EPSILON) * 1000) / 1000)+ " ms");

 }
 async function main_q4() {
 
 
    var  parts = [0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0];
    for( var i = 0 ; i< 10 ; i++)
      await  calculate_q4(parts[i]);
 
 }
