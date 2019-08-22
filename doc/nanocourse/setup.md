# Compact Data Structures - Setup {#setup_compact_data_structures}

<b>Learning Objective:</b><br>
In this short guide you will learn how to set up your work environment for working through this nancourse.

\tutorial_head{Easy, 30 Minutes, No prerequisites, }

[TOC]

<br>

# Software
Requirements:
  - gcc >= 7
  - wget

We will briefly explain how to install GCC-7 on some popular operating systems, but we recommend using the latest version of GCC available. For more information refer to your operating system's documentation.

**Ubuntu >= 18.04**
```
sudo apt install g++
```
**Ubuntu < 18.04**
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install g++-7
```
**MacOS** using [Homebrew](https://brew.sh/)
```
brew install gcc@7
```

**MacOS** using [MacPorts](https://www.macports.org/)
```
sudo port install gcc-7
```

**Linux** using [conda](https://conda.io)
```
conda create -n gcc7 -c quantstack gcc-7 libgcc-7
conda activate gcc7
```
This will put GCC-7 in a separate environment called `gcc7` which can be activated via `conda activate gcc7` and deactivated via `conda deactivate gcc7`.

\note \htmlonly <div class=\"assignment\"> <details><summary><b>Known Issue:</b></summary> \endhtmlonly If you encounter the error <code>/usr/lib/x86_64-linux-gnu/libstdc++.so.6: version 'CXXABI_1.3.11' not found</code>, you have to set the LD_LIBRARY_PATH:
```
export LD_LIBRARY_PATH=/home/user/miniconda3/envs/gcc7/lib/
```
where `/home/user/miniconda3/` is the path to your conda installation. \htmlonly </details> </div> \endhtmlonly

\attention After installing, `g++ --version` should print the desired version. If not, you may have to use, for example, `g++-7 --version` or even specify the full path to your executable.

Furthermore `echo $CXX` should print the executable of the desired `g++` version. Otherwise run
`export CXX=<executable>` (replace `<executable>` witht the path to `g++`).

# Directory Structure
For this project, we recommend the following directory layout:

```
nanocourse
├── source
└── Makefile
```

To create these directories, you can follow this script:
```
mkdir nanocourse
cd nanocourse
mkdir source
```

Now we need to create a file named `Makefile` inside `nanocourse` with the following contents:
```make
SOURCES=$(shell find . -name source/*.cpp)
OBJECTS=$(SOURCES:%.cpp=%)

all: $(OBJECTS)
ifndef CXX
	$(error CXX is undefined)
endif

%: %.cpp
	$(CXX) -std=c++17 -O3 -march=native -pedantic -Wall -Wextra -Werror -c $< -o $(notdir $@)
```

# Compiling and Running

To test whether everything works, we will now compile and run a small example.

First we create the file `hello_world.cpp` in the `source` directory with the following contents:

\include test/snippet/setup/hello_world.cpp

To compile it we run `make` inside the `nanocourse` directory and then execute the binary.

``
make
./hello_world
``

The output should be `Hello world`.

# Adding a new source file to your project

All files inside the `source` directory that end with `cpp` are automatically compiled when running `make`.
