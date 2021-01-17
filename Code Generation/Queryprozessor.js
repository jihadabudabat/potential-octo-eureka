/**
 * The function Reads the binary files Using HTTP Requests
 * 
 * @param {String} url The Path to the File
 */
function fetchLocal(url) {
    return new Promise(function(resolve, reject) {
      var xhr = new XMLHttpRequest
      xhr.responseType = "arraybuffer";
      xhr.onload = function() {
        resolve( xhr.response )
      }
      xhr.onerror = function() {
        reject(new TypeError('Local request failed'))
      }
      xhr.open('GET', url)
      xhr.send(null)
    })
  }

async function readBlob(URL) {

    return await fetchLocal(URL);
}

async function readFile(src, packageName) {

    var data_ = await readBlob(src+packageName);
    return new Int32Array(data_);
}
//------------------------------------------------------------------------------------------- JavaScript Tests: Start
/**
 * This function calculate defined operator in scale of the given parameter "count"
 *  --> Scale factor test
 * @param {String} src  path to the file
 * @param {String} type the operator = ['+','-','*','/'] 
 * @param {int} count  Scale factor, the cumber of loaded rows
 */
async function calculate_JavaScript(src, type, count) {

    var l_orderkey = await readFile(src, "l_orderkey");
if (type == '+') {
        for (let i = 0; i < (count % l_orderkey.length); i++) {
            let x = 1;
            let num = l_orderkey[i];
            for (let j = 0; j < 500; j++)
                x += num;
        }
        return "Finished";
    } else if (type == '-') {
        for (let i = 0; i < (count % l_orderkey.length); i++) {
            let x = 1;
            let num = l_orderkey[i];
            for (let j = 0; j < 500; j++)
                x -= num;
        }
        return "Finished";
    } else if (type == '*') {
        for (let i = 0; i < (count % l_orderkey.length); i++) {
            let x = 1;
            let num = l_orderkey[i];
            for (let j = 0; j < 500; j++)
                x *= num;
        }
        return "Finished";
    } else if (type == '/') {
        for (let i = 0; i < (count % l_orderkey.length); i++) {
            let x = 1;
            let num = l_orderkey[i];
            for (let j = 0; j < 500; j++)
                x /= num;
        }
        return "Finished";
    }
}


/**
 * This function calculate defined operator multiple times over the same data
 * --> Number of Functions
 * @param {String} src  path to the file
 * @param {String} type the operator = ['+','-','*','/'] 
 * @param {int} count  Scale factor, the cumber of loaded rows
 */
async function calculate_JavaScript_functions_number(src, type, count) {

    var l_orderkey = await readFile(src, "l_orderkey");
    if (type == "+") {
        for (let i = 0; i < l_orderkey.length; i++) {
            let x = 1;
            let num = l_orderkey[i];
            for (let j = 0; j < count; j++)
                x += num;
        }
        return "Finished";
    } else if (type == "-") {
        for (let i = 0; i < l_orderkey.length; i++) {
            let x = 1;
            let num = l_orderkey[i];
            for (let j = 0; j < count; j++)
                x -= num;
        }
        return "Finished";
    } else if (type == "*") {
        for (let i = 0; i < l_orderkey.length; i++) {
            let x = 1;
            let num = l_orderkey[i];
            for (let j = 0; j < count; j++)
                x *= num;
        }
        return "Finished";
    } else if (type == "/") {
        for (let i = 0; i < l_orderkey.length; i++) {
            let x = 1;
            let num = l_orderkey[i];
            for (let j = 0; j < count; j++)
                x /= num;
        }
        return "Finished";
    }
}

/**
 * This function calculate defined operator multiple times over the same data
 * --> Number of Functions
 * @param {String} src  path to the file
 * @param {String} type the operator = ['+','-','*','/'] 
 * @param {int} count  Scale factor, the cumber of loaded rows
 */
async function calculate_JavaScript_parameter_number(src, type, count) {

    var l_orderkey = await readFile(src, "l_orderkey");

    let sum;
    if (count == 1) {
        for (let i = 0; i < l_orderkey.length; i++) {
            sum = l_orderkey[i];
        }
    } else if (count == 5) {

        for (let i = 0; i < l_orderkey.length; i++) {
            sum *= calculate_5(type,l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]);
        }
    }
    else if (count == 10) {

        for (let i = 0; i < l_orderkey.length; i++) {
            sum *= calculate_10(type,l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]
                , l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]);
        }
    }
    else if (count == 15) {

        for (let i = 0; i < l_orderkey.length; i++) {
            sum *= calculate_15(type,l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]);
        }
    }
    else if (count == 20) {

        for (let i = 0; i < l_orderkey.length; i++) {
            sum *= calculate_20(type,l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]);
        }
    }
    return "Finished";
}
// ---------------------------------------------------------- Predefined functions for TEsting Argument Count
// Test for five arguments
// Type --> the operator = ['+','-','*','/'] 
function calculate_5(type, one, two, three, four, five) {

    let sum = 1;

    if (type == '+') {
        sum += one;
        sum += two;
        sum += three;
        sum += four;
        sum += five;
    } else if (type == '-') {

        sum -= one;
        sum -= two;
        sum -= three;
        sum -= four;
        sum -= five;
    } else if (type == '*') {

        sum *= one;
        sum *= two;
        sum *= three;
        sum *= four;
        sum *= five;
    } else if (type == '/') {

        sum /= one;
        sum /= two;
        sum /= three;
        sum /= four;
        sum /= five;
    }
}

// Test for ten arguments
// Type --> the operator = ['+','-','*','/'] 
function calculate_10(type,one, two, three, four, five, six, seven, eight, nine, ten) {
    let sum = 1;
    if (type == '+') {

        sum += one;
        sum += two;
        sum += three;
        sum += four;
        sum += five;
        sum += six;
        sum += seven;
        sum += eight;
        sum += nine;
        sum += ten;
    } else if (type == '-') {

        sum -= one;
        sum -= two;
        sum -= three;
        sum -= four;
        sum -= five;
        sum -= six;
        sum -= seven;
        sum -= eight;
        sum -= nine;
        sum -= ten;
    } else if (type == '*') {

        sum *= one;
        sum *= two;
        sum *= three;
        sum *= four;
        sum *= five;
        sum *= six;
        sum *= seven;
        sum *= eight;
        sum *= nine;
        sum *= ten;
    } else if (type == '/') {

        sum /= one;
        sum /= two;
        sum /= three;
        sum /= four;
        sum /= five;
        sum /= six;
        sum /= seven;
        sum /= eight;
        sum /= nine;
        sum /= ten;
    }
}

// Test for fifteen arguments
// Type --> the operator = ['+','-','*','/'] 
function calculate_15(type,one, two, three, four, five, six, seven,
    eight, nine, ten, elven, twelve, therteen,
    fourteen, fifteen) {
    let sum = 1;
    if (type == '+') {

        sum += one;
        sum += two;
        sum += three;
        sum += four;
        sum += five;
        sum += six;
        sum += seven;
        sum += eight;
        sum += nine;
        sum += ten;
        sum += elven;
        sum += twelve;
        sum += therteen;
        sum += fourteen;
        sum += fifteen;
    } else if (type == '-') {

        sum -= one;
        sum -= two;
        sum -= three;
        sum -= four;
        sum -= five;
        sum -= six;
        sum -= seven;
        sum -= eight;
        sum -= nine;
        sum -= ten;
        sum -= elven;
        sum -= twelve;
        sum -= therteen;
        sum -= fourteen;
        sum -= fifteen;
    } else if (type == '*') {

        sum *= one;
        sum *= two;
        sum *= three;
        sum *= four;
        sum *= five;
        sum *= six;
        sum *= seven;
        sum *= eight;
        sum *= nine;
        sum *= ten;
        sum *= elven;
        sum *= twelve;
        sum *= therteen;
        sum *= fourteen;
        sum *= fifteen;
    } else if (type == '/') {

        sum /= one;
        sum /= two;
        sum /= three;
        sum /= four;
        sum /= five;
        sum /= six;
        sum /= seven;
        sum /= eight;
        sum /= nine;
        sum /= ten;
        sum /= elven;
        sum /= twelve;
        sum /= therteen;
        sum /= fourteen;
        sum /= fifteen;
    }
}

// Test for twenty arguments
// Type --> the operator = ['+','-','*','/'] 
function calculate_20(type,one, two, three, four, five, six, seven, eight, nine, ten,
    elven, twelve, therteen, fourteen,
    fifteen, sixteen, seventeen,eighteen, nineteen, twenty) {
        let sum = 1;
    if (type == '+') {

        sum += one;
        sum += two;
        sum += three;
        sum += four;
        sum += five;
        sum += six;
        sum += seven;
        sum += eight;
        sum += nine;
        sum += ten;
        sum += elven;
        sum += twelve;
        sum += therteen;
        sum += fourteen;
        sum += fifteen;
        sum += sixteen;
        sum += seventeen;
        sum += eighteen;
        sum += nineteen;
        sum += twenty;
    } else if (type == '-') {

        sum -= one;
        sum -= two;
        sum -= three;
        sum -= four;
        sum -= five;
        sum -= six;
        sum -= seven;
        sum -= eight;
        sum -= nine;
        sum -= ten;
        sum -= elven;
        sum -= twelve;
        sum -= therteen;
        sum -= fourteen;
        sum -= fifteen;
        sum -= sixteen;
        sum -= seventeen;
        sum -= eighteen;
        sum -= nineteen;
        sum -= twenty;
    } else if (type == '*') {

        sum *= one;
        sum *= two;
        sum *= three;
        sum *= four;
        sum *= five;
        sum *= six;
        sum *= seven;
        sum *= eight;
        sum *= nine;
        sum *= ten;
        sum *= elven;
        sum *= twelve;
        sum *= therteen;
        sum *= fourteen;
        sum *= fifteen;
        sum *= sixteen;
        sum *= seventeen;
        sum *= eighteen;
        sum *= nineteen;
        sum *= twenty;
    } else if (type == '/') {

        sum /= one;
        sum /= two;
        sum /= three;
        sum /= four;
        sum /= five;
        sum /= six;
        sum /= seven;
        sum /= eight;
        sum /= nine;
        sum /= ten;
        sum /= elven;
        sum /= twelve;
        sum /= therteen;
        sum /= fourteen;
        sum /= fifteen;
        sum /= sixteen;
        sum /= seventeen;
        sum /= eighteen;
        sum /= nineteen;
        sum /= twenty;
    }
}
// ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------------- JavaScript Tests: End

// --------------------------------------------------------------------------- Code Generation Tests: Start
/**
 * This Test is for Scale factor. The compiler contains the function generated in Binaryen
 * 
 * @param {String} src  path to the file
 * @param {*} compiler --> The generated function in Binaryen
 * @param {int} count  Scale factor, the cumber of loaded rows
 */
async function main(src, compiler, count) {

    var l_orderkey = await readFile(src, "l_orderkey");
    for (let i = 0; i < (count % l_orderkey.length); i++) {
        compiler.main(l_orderkey[i]);
    }
    return "Finished";

}
/**
 * This Test is for  the number of generated Functions
 * 
 * @param {String} src  path to the file
 * @param {*} compiler --> The generated functions in Binaryen are in the compiler
 */
async function mainfunctiongeneriert(src, compiler) {

    var l_orderkey = await readFile(src, "l_orderkey");
    for (let i = 0; i < l_orderkey.length; i++) {
        compiler.main(l_orderkey[i]);
    }
    return "Finished";

}
/**
 * This Test is for the Argument count
 * 
 * @param {String} src  path to the file
 * @param {*} compiler --> The generated function in Binaryen is in the compiler
 * @param {int} count  Scale factor, the cumber of loaded rows
 */
async function main_multi_function(src, compiler, count) {

    var l_orderkey = await readFile(src, "l_orderkey");
    if (count == 1) {
        for (let i = 0; i < l_orderkey.length; i++) {
            compiler.main(l_orderkey[i]);
        }
    } else if (count == 5) {

        for (let i = 0; i < l_orderkey.length; i++) {
           // Five arguments
            compiler.main(l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]);
        }
    }
    else if (count == 10) {

        for (let i = 0; i < l_orderkey.length; i++) {
            // Ten arguments
            compiler.main(l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]
                , l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]);
        }
    }
    else if (count == 15) {

        for (let i = 0; i < l_orderkey.length; i++) {
            // Fifteen arguments
            compiler.main(l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]);
        }
    }
    else if (count == 20) {

        for (let i = 0; i < l_orderkey.length; i++) {
            // Twenty arguments
            compiler.main(l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i],
                l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i], l_orderkey[i]);
        }
    }
    return "Finished";
}
