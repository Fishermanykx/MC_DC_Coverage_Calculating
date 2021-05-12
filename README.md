# MC-DC_Coverage_Calculating
This is a tool that can analyse MC/DC coverage of a program.
Only my homework for Software Testing, so don't expect it being fast.

## How to run this code

Below I will use the pass `originalSample` for the example. Make sure that the pass to be compiled is that pass.

### Compile the Pass

In the directory `./build` run the commands below

```shell
cmake ..
make
```

### For codes that needs instrumentation

Run the  commands in the directory `.` (project root dir)

`test.c` is the module to be tested

`originalSample` is the name of the sample Pass

`runtime.cpp` contains external functions to be instrumented

```shell
g++ runtime.cpp -fPIC -shared -o libruntime.so
clang -emit-llvm -S -fno-discard-value-names -c test.c test.ll
opt -load bulid/src/liboriginalSample.so -originalSample -S test.ll test.instrumented.ll
clang -o test libruntime.so test.instrumented.ll 
./test
```

### For codes that do not need instrumentation

Run the  commands in the directory `.` (project root dir)

```shell
clang -Xclang -load -Xclang build/src/liboriginalSample.so test.c
```

Now it's far from being finished, please wait for 1~2 weeks before all the things are done.