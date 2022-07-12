# Getting Started

## Build

```
mkdir build
cd build
cmake ..
make -j8
```

## Execution

```
cd build
./bin/skinning ../assets/pmd/Meiko_Sakine.pmd
```

You need to provide a .pmd file to launche the skinning code. A set of PMD
files have been shipped under assets/pmd directory.

# Acknowledgement 

This bone animation code is based on the skinning project written by
Randall Smith for 2015 Spring Graphics Course.

The PMD parser library is written by
[itsuhane](https://github.com/itsuhane/libmmd), licensed under Boost Software
License.

The author of PMD models is Animasa, you can download the model with the
official editor from his webpage [VPVP](http://www.geocities.jp/higuchuu4/index_e.htm).
