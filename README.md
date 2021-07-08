# MC_DC_Coverage_Calculating
This is a tool that can analyze MC/DC coverage of a program.
Only my homework for Software Testing, so don't expect it being fast.

Method to calculate coverage:

$$

Coverage = \frac{\text{the number of valid test cases}}{2 * \text{the number of conditions}}\\

\text{"valid" means in this test case, one condition can infact result of the decision independently}

$$


## File structure
```shell
.
├── build
│   ├── CMakeCache.txt
│   ├── CMakeFiles
│   ├── cmake_install.cmake
│   ├── compile_commands.json
│   ├── Makefile
│   └── src
├── cfg_func.png
├── CMakeLists.txt
├── libruntime.so
├── LICENSE
├── README.md
├── run.sh
├── runtime.cpp
├── src
│   ├── CMakeLists.txt
│   ├── ConditionCoverage.cpp
│   ├── demoPass1.cpp
│   ├── helloPass.cpp
│   ├── MCDCCoverage.cpp
│   ├── originalSample.cpp
│   └── ProjHeaders.h
├── test
├── test.c
├── test.instrumented.ll
└── test.ll
```

## How to run this code

Below I will use the pass `MCDCCoverage` for the example. Make sure that the pass to be compiled is that pass.

### Compile the Pass

In the directory `./build` run the commands below

```shell
cmake ..
make
```

### For codes that needs instrumentation

Run the  commands in the directory `.` (project root dir)

`test.c` is the module to be tested

`MCDCCoverage` is the name of the Pass

`runtime.cpp` contains external functions to be instrumented

```shell
g++ runtime.cpp -fPIC -shared -o libruntime.so
clang -emit-llvm -S -fno-discard-value-names -c test.c test.ll
opt -load build/src/libMCDCCoverage.so -MCDCCoverage -S test.ll -o test.instrumented.ll
clang -o test libruntime.so test.instrumented.ll
./test
```
You can also run `run.sh` directly


### For codes that do not need instrumentation

Take the pass `origonalSample` for example
Run the  commands in the directory `.` (project root dir)

```shell
clang -Xclang -load -Xclang build/src/liboriginalSample.so test.c
```
