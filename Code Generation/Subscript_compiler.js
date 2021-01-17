
// ------------------------------------------- Pre defined/ Compiled WASM Functions
async function compiler(binaryen) {
    // Create a module to work on
    var module = new binaryen.Module();
    console.log(module);

    // Multi function, starting with the contents: Get the 0 and
    // 1 arguments, and multiply them, then return them
    var left = module.local.get(0, binaryen.i32);
    var right = module.local.get(1, binaryen.i32);
    var mul = module.i32.mul(left, right);
    var ret_mul = module.return(mul);

    // Add function, starting with the contents: Get the 0 and
    // 1 arguments, and add them, then return them
    var left = module.local.get(0, binaryen.i32);
    var right = module.local.get(1, binaryen.i32);
    var add = module.i32.add(left, right);
    var ret_add = module.return(add);

    // Add float function, starting with the contents: Get the 0 and
    // 1 arguments, and add them, then return them
    var left = module.local.get(0, binaryen.f32);
    var right = module.local.get(1, binaryen.f32);
    var add_f = module.f32.add(left, right);
    var ret_add_f = module.return(add_f);

    // mult float function, starting with the contents: Get the 0 and
    // 1 arguments, and add them, then return them
    var left = module.local.get(0, binaryen.f32);
    var right = module.local.get(1, binaryen.f32);
    var mul_f = module.f32.mul(left, right);
    var ret_mul_f = module.return(mul_f);

    var ii = binaryen.createType([binaryen.i32, binaryen.i32])
    var ff = binaryen.createType([binaryen.f32, binaryen.f32])
    module.addFunction('add', ii, binaryen.i32, [], ret_add);
    module.addFunction('mul', ii, binaryen.i32, [], ret_mul);
    module.addFunction('addf',ff, binaryen.f32,[], ret_add_f);
    module.addFunction('mulf',ff, binaryen.f32,[], ret_mul_f);
    
    module.addFunctionExport('add', 'add');
    module.addFunctionExport('mul', 'mul');
    module.addFunctionExport('addf', 'addf');
    module.addFunctionExport('mulf', 'mulf');

    module.optimize();
    console.assert(module.validate());
    var binary = module.emitBinary();
    // We don't need the Binaryen module anymore, so we can tell it to
    // clean itself up
    module.dispose();
    // Compile the binary and create an instance
    var compiler = await new WebAssembly.Module(binary);

    var wasm = new WebAssembly.Instance(compiler,{});
    return wasm.exports;
}
//------------------------------------------------------- Pre defined/compiled wasm functions --- End
// ------------------------------------------------------ Example Expretion Compiler -------- Start
/**
 *  This function evaluats an Add function and returns an expretion "Add" if the function is correct
 * 
 * @param {*} left  the left argument
 * @param {*} right the right argument
 * @param {*} module the module
 */
function  add(left,right,module) {
    var left_, right_; 
    let expretion;

    if(left["type"] == "float" || right["type"] == "float") {
    
        if(left["type"] ==  "EXPR")
            left_ = left["value"];
        else 
            left_ = module.f32.const(left["value"]);
        
        left_ = left["value"];
        
        right_ = module.f32.const(right["value"]);
        expretion = module.f32.add(left_, right_);
    }
    else {
        if(left["type"] ==  "EXPR")
            left_ = left["value"];
        else 
            left_ = module.i32.const(left["value"]);
        
        left_ = left["value"];
        right_ = module.i32.const(right["value"]);
        expretion = module.i32.add(left_, right_);
    }
    return expretion;
 }

 /**
 *  This function evaluats muliplication function and returns an expretion "Mul" if the function is correct
 * 
 * @param {*} left  the left argument
 * @param {*} right the right argument
 * @param {*} module the module
 */
 function  mul(left,right,module) {
    var left_, right_; 
    let expretion;
    
    if(left["type"] == "float" || right["type"] == "float") {
    
        if(left["type"] ==  "EXPR")
            left_ = left["value"];
        else 
            left_ = module.f32.const(left["value"]);
        
            left_ = left["value"];

        right_ = module.f32.const(right["value"]);
        expretion = module.f32.mul(left_, right_);
    }
    else {
        if(left["type"] ==  "EXPR")
            left_ = left["value"];
        else 
            left_ = module.i32.const(left["value"]);
        
            left_ = left["value"];

        right_ = module.i32.const(right["value"]);
        expretion = module.i32.mul(left_, right_);
    }
    return expretion;
 }

 /**
 *  This function evaluats an div function and returns an expretion "Div" if the function is correct
 * 
 * @param {*} left  the left argument
 * @param {*} right the right argument
 * @param {*} module the module
 */
 function  div(left,right,module) {
    var left_, right_; 
    let expretion;

    if(left["type"] == "float" || right["type"] == "float") {

        if(left["type"] ==  "EXPR")
                left_ = left["value"];
            else 
                left_ = module.f32.const(left["value"]);
        
                
        left_ = left["value"];

        right_ = module.f32.const(right["value"]);
        expretion = module.f32.div_u(left_, right_);
    }
    else {
        if(left["type"] ==  "EXPR")
            left_ = left["value"];
        else 
            left_ = module.i32.const(left["value"]);

            left_ = left["value"];

        right_ = module.i32.const(right["value"]);
        expretion = module.i32.div_u(left_, right_);
    }
    return expretion;
 }

 /**
 *  This function evaluats an Sub function and returns an expretion "Sub" if the function is correct
 * 
 * @param {*} left  the left argument
 * @param {*} right the right argument
 * @param {*} module the module
 */
 function  sub(left,right,module) {
    var left_, right_; 
    let expretion;

    if(left["type"] == "float" || right["type"] == "float") {

        if(left["type"] ==  "EXPR")
                left_ = left["value"];
            else 
                left_ = module.f32.const(left["value"]);
            
            left_ = left["value"];

        right_ = module.f32.const(right["value"]);
        expretion = module.f32.sub(left_, right_);
    }
    else {
        if(left["type"] ==  "EXPR")
            left_ = left["value"];
        else 
            left_ = module.i32.const(left["value"]);
        
            left_ = left["value"];

        right_ = module.i32.const(right["value"]);
        expretion = module.i32.sub(left_, right_);
    }
    return expretion;
 }

 /**
 *  This function evaluats the operator, it it is correect the it continue to evaluate the parameters
 * 
 * @param {*} left  the left argument
 * @param {*} right the right argument
 * @param {*} module the module
 * @param {*} _ops   the operator [-, +, *, /]
 */
function evaluateOperation(left,right,_ops,module) {
        let _left = [];
        let _right = [];

    // Left is an expretion
    _left["type"] = binaryen.Expression;
    _left["value"]= left;

    // right is a constant
    if (!isNaN(right) && right.toString().indexOf('.') != -1) {
        
        _right["type"] = "float";
        _right["value"]= parseFloat(right);
    } else if (Number.isInteger(right)) {

        _right["type"] = "int";
        _right["value"]=  parseInt(right);
    } else if(right ==  "EXPR") {

        _right["type"] = binaryen.Expression;
        _right["value"]= right;
    } else {

        throw 'An argument is not a number';
    }

    // the current given operators
    switch (_ops) {
        case '+':
             return add(_left,_right,module);   
        case '-':
            return sub(_left,_right,module);
        case '*':
            return mul(_left,_right,module);
        case "/":
            return div(_left,_right,module);
        default:
            throw 'the operator '+_ops+' is not implemented';
    }
 }

 /**
  * This compiler generates Functions to test the scale factor and the function counts
  * 
  * @param {*} type     the operator [-, +, *, /]
  * @param {*} binaryen  The binaryen backend
  * @param {*} count_  the number of functions to be generated
  */
async function __compiler(type,binaryen, count_) {

    var module = new binaryen.Module();

    // Generate Randon Expretions
    let count = 500;
    let expretionarray = [];
    let operatorArray = ['+','-','*','/'];

    // greate a local variable
    let x_index = 0;
    let x = module.local.get(x_index, binaryen.f32);

    // the expretion array to be passed to the block
    // The block contains a sequence of expretions
    expretionarray.push(x);
    expretionarray.push();
    binaryen.setFastMath(true);

    // Falls die Anzhal der Funktionen ist definiert
    if (count_)
        count = count_;
    else 
        count = 500

    for(let i = 0 ; i < count ; i++) {

        // create the expretion
        const right = Math.floor(Math.random() * 10) +1 ;
        expretionarray.push(module.local.set(x_index , evaluateOperation(x,right,type,module)));
    }

    // Return value 
    expretionarray.push(module.return(x));
    // a block inside the function
    const theBlock = binaryen.Block(module.block(null, expretionarray, binaryen.none));
    theBlock.name ="main";
    theBlock.type = binaryen.i32;

    // The function contains all the other expretions
    var ff = binaryen.createType([binaryen.i32])
    module.addFunction('main',ff , binaryen.i32, [], theBlock);
    module.addFunctionExport('main', 'main');
    var binary = module.emitBinary();
    //Uncomment this if you want to see the generated code
    //console.log(module.emitText())
    module.dispose();
    var compiler = await new WebAssembly.Module(binary);
    var wasm = new WebAssembly.Instance(compiler,{});
    return wasm.exports;
}

async function test_main(type,binaryen, count) {
    return await __compiler(type,binaryen,count);
}
//--------------------------------------------------------------- Example Exprition Compiler ----------- End
//--------------------------------------------------------------- Automatic generate multi parameters -- End
 /**
  * This compiler generates a function with multiple parameter
  * 
  * @param {*} type     the operator [-, +, *, /]
  * @param {*} binaryen  The binaryen backend
  * @param {*} count_  the number of functions to be generated
  */
async function test_main_multi_params(binaryen, count, type) {
    return await __compiler_multi_param(binaryen, count, type);
}

async function __compiler_multi_param(binaryen, count, type) {

    var module = new binaryen.Module();

    // Generate Randon Expretions
    let expretionarray = [];
    let operatorArray = ['+','-','*','/'];
    let typeArray = [];
    let colument_array = [];
    binaryen.setFastMath(true);

    // create multiple Arguments
    for(let i = 0 ; i < count; i++) {

        let x = module.local.get(i, binaryen.f32);
        typeArray.push(binaryen.i32);
        colument_array.push(x);
        expretionarray.push(x);
    }
  
    for(let i = 0 ; i < 1 ; i++) {

        // generate the functions for adding each argument  1 -> 20  
        // no use to the compiler = no evaluation
        for( let j = 0 ; j < count ; j++) {
            
            const left  = module.local.get(0, binaryen.f32);
            const right = module.local.get(j, binaryen.f32) ;

            switch (type) {
                case '+':
                    expretionarray.push(module.local.set(0, module.i32.add(left, right))); 
                    continue;
                case '-':
                    expretionarray.push(module.local.set(0, module.i32.sub(left, right)));
                    continue;
                case '*':
                    expretionarray.push(module.local.set(0, module.i32.mul(left, right)));
                    continue;   
                case "/":
                    expretionarray.push(module.local.set(0, module.i32.div_u(left, right)));
                    continue;
                default:
                    throw 'the operator '+type+' is not implemented';
            }
           
        }
    }
    
    // Return function
    expretionarray.push(module.return(module.local.get(0, binaryen.f32)));
    // create block
    const theBlock = binaryen.Block(module.block(null, expretionarray, binaryen.none));
    theBlock.name ="main";
    theBlock.type = binaryen.i32;
    // create function
    var ff = binaryen.createType(typeArray)
    module.addFunction('main',ff,binaryen.i32, [], theBlock);
    module.addFunctionExport('main', 'main');
    //Uncomment this if you want to see the generated code
    //console.log(module.emitText())
    var binary = module.emitBinary();
    module.dispose();
    var compiler = await new WebAssembly.Module(binary);
    var wasm = new WebAssembly.Instance(compiler,{});
    return wasm.exports;
}
