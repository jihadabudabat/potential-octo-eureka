
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
 * Implementation for TPC-H 5
 * @param {int} part for the scale factor test
 */
async function calculate_q5(sort, part) {
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
  let t0 = performance.now();

   let src = "./tpch1binary/";
   var s_suppkey = await readFile(src, "int","s_suppkey");
   var s_nationkey = await readFile(src, "int","s_nationkey");

   var c_custkey = await readFile(src, "int","c_custkey");
   var c_nationkey = await readFile(src, "int","c_nationkey");

   var o_orderkey = await readFile(src, "int","o_orderkey");
   var o_custkey = await readFile(src, "int","o_custkey");
   var o_orderdate = await readFile(src, "str","o_orderdate_text");

   var l_orderkey = await readFile(src, "int","l_orderkey");
   var l_suppkey = await readFile(src, "int","l_suppkey");
   var l_extendedprice = await readFile(src, "float","l_extendedprice");
   var l_discount = await readFile(src, "float","l_discount");

   var n_nationkey = await readFile(src, "int","n_nationkey");
   var n_name = await readFile(src, "str","n_name_text");
   var n_regionkey = await readFile(src, "int","n_regionkey");
   var r_regionkey = await readFile(src, "int","r_regionkey");
   var r_name = await readFile(src, "str","r_name_text");
   
   let t1 = performance.now();
   console.log("Query 5 loading "+ part +"  took "+ (Math.round(((t1 - t0) + Number.EPSILON) * 1000) / 1000)+ " ms");
   t0 = performance.now();
 
   

   let o_materialization = new Map();
   let r_materialization = new Map()
   let c_materialization = new Map()
   let s_materialization = new Map()
   let n_materialization = new Map()
   let c_n_key_materialization = new Map()

   let ASIA = "ASIA";
   let DATUM_1 = new Date("1994-01-01").getTime();
   let DATUM_2 = new Date("1995-01-01").getTime();
   // materialize the table r_
   for (let i = 0; i < r_name.length * part; i++) {
      if (r_name[i] === ASIA) {
         r_materialization.set(r_regionkey[i] , i);
      }
   }

   // materialize the table o_
   for (let i = 0; i < o_orderkey.length * part; i++) {
      if (new Date(o_orderdate[i]).getTime() > DATUM_1 && new Date(o_orderdate[i]).getTime() < DATUM_2) {

         o_materialization.set(o_orderkey[i], i);
      }
   }

   // materialize the table c_
   for (let i = 0; i < c_custkey.length * part; i++) {
      c_materialization.set(c_custkey[i], i);
      c_n_key_materialization.set(c_nationkey[i], i);
   }

   // materilize s_
   for (let i = 0; i < s_suppkey.length * part; i++) {
      s_materialization.set(s_suppkey[i], i);
   }

   // materilize n_
   for (let i = 0; i < n_nationkey.length; i++) {
      n_materialization.set(n_nationkey[i], i);
   }

   let N_NAME = new Map();
   let L_EXTENDEDPRICE = [];
   let L_DISCOUNT = [];

   // Join the Tables
   let result_index = 0;
   for (let i = 0; i < l_orderkey.length * part; i++) {

      if (o_materialization.has(l_orderkey[i]) && s_materialization.has(l_suppkey[i])) {
         let key_o = l_orderkey[i];
         let index_o = o_materialization.get(l_orderkey[i]);
         let key_s = l_suppkey[i];
         let index_s = s_materialization.get(l_suppkey[i]);

         if (c_materialization.has(o_custkey[index_o]) && c_n_key_materialization.has(s_nationkey[index_s])) {

            let c_key = o_custkey[index_o];
            let c_index = c_materialization.get(o_custkey[index_o]);
            let n_key = s_nationkey[index_s];

            if (n_materialization.get(n_key)) {

               let n_index = n_materialization.get(n_key);

               if (r_materialization.get(n_regionkey[n_index])) {

                  let index_r = r_materialization.get(n_regionkey[n_index]);

                  if (!N_NAME.has(n_name[n_index])) {
                     N_NAME.set(n_name[n_index], [result_index]);
                  } else {
                     let arr = (N_NAME.get(n_name[n_index]));
                     arr.push(result_index);
                     N_NAME.set(n_name[n_index], arr);
                  }

                  L_EXTENDEDPRICE.push(l_extendedprice[i]);
                  L_DISCOUNT.push(l_discount[i]);
                  result_index++;
               }

            }
         }
      }
   }

   o_materialization.clear();
   r_materialization.clear();
   c_materialization.clear();
   s_materialization.clear();
   n_materialization.clear();
   c_n_key_materialization.clear();

   let REVENUE = new Map(); // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT))

   // Sort
   N_NAME = new Map([...N_NAME.entries()].sort());

   let REVENUE_ = 0;
   // The Aggrigation
   for (let i = 0; i < N_NAME.size; i++) {

      let name = Array.from(N_NAME.keys())[i];
      let arr = N_NAME.get(name);
      // if the current equals the next --> aggrigate
      for (let j = 0; j < arr.length; j++) {

         let index = arr[j];

         REVENUE_ += (L_EXTENDEDPRICE[index] * L_DISCOUNT[index]);

      }
      // 
      REVENUE.set(REVENUE_, name);
      REVENUE_ = 0;
   }
   // Sort
   REVENUE = new Map([...REVENUE.entries()].sort());
   //console.log(REVENUE);
   t1 = performance.now();
   console.log("Query 5 with scale factor "+ part +"  took "+ (Math.round(((t1 - t0) + Number.EPSILON) * 1000) / 1000)+ " ms");

}
async function main_q5(sort) {


   var parts = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0];
   for (var i = 0; i < 10; i++)
      await calculate_q5(sort, parts[i]);

}
