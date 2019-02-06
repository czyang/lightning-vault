# ⚡ Lightning Vault 
<!-- [![Build Status](https://travis-ci.org/czyang/lightning-vault.svg?branch=master)](https://travis-ci.org/czyang/lightning-vault) -->

An experimental lightning fast non-traditional web browser. 

## Build Instructions

Use cmake to generate project file or build the project.

### Ubuntu

Requirement: 

clang version >= 6.0 or gcc >= 4.8 and cmake >= 3.0

``` bash
# clang >= 6.0 or gcc >= 4.8
sudo apt-get install xorg-dev libglu1-mesa-dev

mkdir build
cd build
cmake ..

make
```

### Windows

Requirement: 

Visual Studio Community 2017. cmake >= 3.0

### Mac
``` bash
# clang >= 6.0 or gcc >= 4.8
mkdir build
cd build
cmake ..

make
```


## Reference

CMake files and project structure: http://www.opengl-tutorial.org/

OpenGL API and relative graphics API: https://learnopengl.com/

Test Font Source Han Serif: https://github.com/adobe-fonts/source-han-serif/tree/release