****
****
# Pre requirements for the Tests:

## emcc
to install this tool use this [link](https://emscripten.org/docs/getting_started/downloads.html). Inside the installation page you can find all the neccessary instruction for compiling and executing a WebAssembly project.  Please dont forget to include the Enviroments Variables (in Linux and Windows), to be able to call the emsdk commands (emrun, emcc, emmake ...) globally.

## binary files
The column stored tables are binary files and they can be found in the following [link](http://www-db.in.tum.de/~leis/tpch1binary.tgz). For the browsers tests there are some extrenel files as String files, they are neccessary for some tests. These files should be included inside the folder "tpch1binary/". They can be found in the folder "StringFiles" and all the files end with "_text", e.g., "c_mktsegment_text". If you dont finde the files inside "StringFiles", please generate them using "parser.cpp". This is because they may be too big to upload them to github.

## wasi-sdk
This toolchain is neccessary for compiling the C++ code to WebAssembly to be able to run it on Bare Metal. wasi-sdk can be found in the [link](https://github.com/WebAssembly/wasi-sdk/releases). Currently it support linux based OS or Windows using [subsystems](https://docs.microsoft.com/en-us/windows/wsl/install-win10).

## wasmtime
Tool used to Run WebAssembly compiled using <u><b>wasi-sdk</b></u>. This tool can be downloaded using the [link](https://wasmtime.dev/). Please follow the instruction for downloading and running.

## chrome version 86 or newer
To run both the Tests "Client" and "Inside the browser" the version 86 or newer of Chrome must be provided, because the [API](https://developer.mozilla.org/en-US/docs/Web/API/FileSystemDirectoryHandle) curently support just both these versions.

## tbb version 2020
For the experemental paralle query using tbb please use the latest version of tbb, because the ["task_scheduler" is deprecated](https://software.intel.com/content/www/us/en/develop/documentation/tbb-documentation/top/intel-threading-building-blocks-developer-reference/appendices/deprecated-features/outdated-and-redundant-api/taskschedulerinit-class.html). It is just an experemental code and not neccesserly needed to be run. 

## linux os
WebAssembly currently is stable on Linux. It can be compiled and executed on windows, however its adviced to do so on linux, because wasmtime does work only on linux to execute WebAssembly (Bare Metal).

## PerfEvent
If you want to profile using PerfEvent please [download and configure it](https://github.com/viktorleis/perfevent). It hasn't been used in the tests, but I included it. If you not willing to use it just comment it from the imports and inside the code.
``` C++
#include "PerfEvent.hpp"
...
.

PerfEvent e;
e.startCounters();
...

e.stopCounters();
e.printReport(std::cout, 1);
std::cout << std::endl;
```   
****

****
# Runing the tests:
Diffrent tests are seperated inside folders with there names. The following shows the Tests and how to execute them:

## Bare metal:
The implemented tests on bare metal are listed inside the folder bare metal. To compile the tests g++ must be used. Bare metal tests includes the follwoing:
<br/>
<br/>
<br/>

#### Simple Queries:
TPC-H Queries 1,2,3,4 and 5. Compile them saparetly using g++ and execute them. The folder "tpch1binary/" must be in same folder of the compiled queries. 
<br/>
<br/>
<br/>

#### Parallel Query (tbb, pthread and loading using malloc):
The TPC-H Query is implemented to be executed in parallel for experementing the execution in WebAssembly (Inside the browser and Bare Metal) and C++ on Bare Metal. If you want to run using wasi-sdk follow the next instruction, currently wasi-sdk doens't fully support POSIX, therefoure the compiling using wasi-sdk for threads will fail. See the following error:
![wasi-sdk Threads](/Tests_Output/pthread_support_wasi.PNG)

To compile on Bare Metal just add the flag ["-lpthread" or "-pthread"](https://askubuntu.com/questions/1015274/simple-c-thread-program-cant-be-compiled) for both tbb and pthreads to link the libraries. Compile them saparetly using g++ and execute them. The folder "tpch1binary/" must be in same folder of the compiled query.
<br/>
<br/>
<br/>

#### WebAssembly (wasi-sdk):
To compile and run queries in WebAssembly on Bare Metal or Native OS just compile using wasi-sdk and run using wasm time [the full commands for compiling and running](https://github.com/bytecodealliance/wasmtime/blob/main/docs/WASI-tutorial.md) are:

``` Bash
/opt/wasi-sdk/bin/clang++ -D_WASI_EMULATED_MMAN -lwasi-emulated-mman  -fno-exceptions q3.cpp -o q3.wasm
``` 
and 

``` Bash
wasmtime q3.wasm --dir=.
``` 

If you forget to add the following flags "-D_WASI_EMULATED_MMAN -lwasi-emulated-mman" and "-fno-exceptions" you will get the following errors:
<br/>

![wasi-sdk error1](/Tests_Output/need_to_link_nmap.PNG)
<br/>
<br/>
![wasi-sdk erroe2](/Tests_Output/exception_support.PNG)

If you get this error "fd!=-1: No such file or directory" then you forgot to add "--dir=." or the folder "tpch1binary/" is not in the same directory of the query. The successful execution will give an output like the following output:
![wasi-sdk sucsess](/Tests_Output/wasi-sdk_ok.PNG)
<br/>
<br/>
<br/>
<br/>

## Inside the Browser (JavaScript vs WebAssembly):
To test in Inside the Browser, for JavaScript you need to run a local server inside the same directory and include the binary and the string files inside the folder "tpch1binary/". The string files can be found in the folder "StringFiles" and all the files end with "_text". The easiest way to run the server is to use "emrun" from the "emsdk" installed from this [link](https://emscripten.org/docs/getting_started/downloads.html). The following shows the local server inside the same folder of the queries:
![wasi-sdk server](/Tests_Output/local_server.PNG)

Then call the address "http://localhost:8080/" and click on the test.html and click on the button "Run Q1" it will call all the tests. you can also modefiy the test.html file and call each test alone. If the server is created and all the files exists then you will get the following output:
![wasi-sdk javascript](/Tests_Output/in_browser_JavaScript.PNG) 

It will then iterate over all the queries. 


For WebAssembly biside creating a local server you need to compile from C++ to WebAssembly and preload the files. For preloading the files please include just the files needed for the query. The smaller the sizeof a file the faster the browser download it. to run The queries you need the same server running inside the folder. The following shows how to compile, listen a server and run the queries in WebAssembly.

* first, compile the query and preload the files:

 ``` Bash
emcc q3.cpp -s ALLOW_MEMORY_GROWTH=1 --preload-file q3files/ -o q3.js
``` 
![wasi-sdk emcc](/Tests_Output/compile_wasm.PNG)
-s ALLOW_MEMORY_GROWTH=1  : This allow the memory to grow if needed \
--preload-file q3files/  : This package all the files inside q3files to use inside the browser if you change the name of the folder for preloading, change also the name inside the code
``` C++
Vector<str> l_shipdate("q3files/l_shipdate");
``` 
-o q3.js This generates a helper JavaScript files to run WebAssembly please give the names q1.js, q2.js, ... q5.js to be able to run them, because i prepared an html file includes these JavaScript files. In the file indexq3.html you see the following:

``` JavaScript
<html>

    <head>

    </head>
    <script async type="text/javascript" src="q3.js"></script>

    <body>
    </body>

</html>
```
* second, run the local server inside the folder :

![wasi-sdk emcc](/Tests_Output/server_wasm.PNG)

* Finally, open the corresponding html file and it will automatically run the query and you will get an output like this:

![wasi-sdk emcc](/Tests_Output/query4_output.PNG)
<br/>
<br/>
<br/>
<br/>

## Client vs Server:

The Server is same as the last steps in WebAssembly. To run from the client the you need to use the [file API](https://developer.mozilla.org/en-US/docs/Web/API/FileSystemDirectoryHandle) in chrome and change the generated JavaScript file to reading from the Client insted of the server. This was just an Experement to find out if Reading from the client will be faster from the server, therefour you dont need to run it. It is faster to read from the client if the server is not set localy, this means if it is remot server then you need to read the data from the network. There are two files inside The folder "Client vs Server" "q3Client.js" and "q3.js". q3 uses the ajax request and q3Client uses the file API. The following steps shows how to do this: 

* Use the file API:

``` JavaScript
        // Check for the support
        navigator.storage.getDirectory ??= () => FileSystemDirectoryHandle.getSystemDirectory({
            type: 'sandbox'
        });
        FileSystemDirectoryHandle.prototype.getDirectoryHandle ??=
            FileSystemDirectoryHandle.prototype.getDirectory;
        FileSystemDirectoryHandle.prototype.getFileHandle ??=
            FileSystemDirectoryHandle.prototype.getFile;
        FileSystemDirectoryHandle.prototype.values ??= function () {
            return this.getEntries()[Symbol.asyncIterator]();
        };
        globalThis.showDirectoryPicker ??= () => chooseFileSystemEntries({
            type: 'open-directory'
        });
        if (!('kind' in FileSystemHandle.prototype)) {
            Object.defineProperty(FileSystemHandle.prototype, 'kind', {
                get() {
                    return this.isFile ? 'file' : 'directory';
                }
            });
        }
            
        let src = await showDirectoryPicker(); // popup to select the sandbox
        await _start_module(src); // called from q3Client 
```
* Read the data from the client:

``` JavaScript
// This wrapper avoids calling the function automaticaly
async function _start_module(src) {
...
.
// This function reads from the server usually
// If you change its original content to the following content, then it will read
// from the client
async function fetchRemotePackage(src,packageName, packageSize, callback, errback) {
            
            const newFileHandle = await src.getFileHandle(packageName, { create: false });
            const file = await newFileHandle.getFile();
            const content = await file.arrayBuffer();

            if (!Module.dataFileDownloads) Module.dataFileDownloads = {};

            Module.dataFileDownloads[packageName] = {
                loaded: true,
                total: content.byteLength
            }
            
            if (Module["setStatus"]) Module["setStatus"]("Downloading data...")

            callback(content);
        }

...
.
}
```

This is the original function:

``` JavaScript
// This wrapper avoids calling the function automaticaly
async function _start_module(src) {
...
.
// The original function using ajax requests
function fetchRemotePackage(packageName, packageSize, callback, errback) {
            var xhr = new XMLHttpRequest;
            xhr.open("GET", packageName, true);
            xhr.responseType = "arraybuffer";
            xhr.onprogress = function(event) {
                var url = packageName;
                var size = packageSize;
                if (event.total) size = event.total;
                if (event.loaded) {
                    if (!xhr.addedTotal) {
                        xhr.addedTotal = true;
                        if (!Module.dataFileDownloads) Module.dataFileDownloads = {};
                        Module.dataFileDownloads[url] = {
                            loaded: event.loaded,
                            total: size
                        }
                    } else {
                        Module.dataFileDownloads[url].loaded = event.loaded
                    }
                    var total = 0;
                    var loaded = 0;
                    var num = 0;
                    for (var download in Module.dataFileDownloads) {
                        var data = Module.dataFileDownloads[download];
                        total += data.total;
                        loaded += data.loaded;
                        num++
                    }
                    total = Math.ceil(total * Module.expectedDataFileDownloads / num);
                    if (Module["setStatus"]) Module["setStatus"]("Downloading data... (" + loaded + "/" + total + ")")
                } else if (!Module.dataFileDownloads) {
                    if (Module["setStatus"]) Module["setStatus"]("Downloading data...")
                }
            };
            xhr.onerror = function(event) {
                throw new Error("NetworkError for: " + packageName)
            };
            xhr.onload = function(event) {
                if (xhr.status == 200 || xhr.status == 304 || xhr.status == 206 || xhr.status == 0 && xhr.response) {
                    var packageData = xhr.response;
                    callback(packageData)
                } else {
                    throw new Error(xhr.statusText + " : " + xhr.responseURL)
                }
            };
            xhr.send(null)
        }
...
.
}
```
<br/>
<br/>
<br/>
<br/>

## Code Generation for Sub Scripts (+, -, / and *):

To use code generation  you need to run the local server inside the folder and call the address "http://localhost:8080/". The output of testing code generation will be created automaticaly and itl will look like the folowing:  
![wasi-sdk code](/Tests_Output/codegenetarion_file.PNG)

