# Compact Data Structures - Setup {#setup_compact_data_structures}

<b>Learning Objective:</b><br>
In this short guide you will learn how to set up your work environment for working through this nancourse.

\tutorial_head{Easy, 30 Minutes, No prerequisites, }

[TOC]

<br>

# Software
Requirements:
  - gcc >= 7
  - cmake >= 3.4
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

Similarly you need to install cmake, e.g. `apt install cmake`.

# Directory Structure
For this project, we recommend the following directory layout:

```
nanocourse
├── build
└── source
    ├── CMakeLists.txt
    └── hello_world.cpp
```

To create these directories, you can follow this script:
```
mkdir nanocourse
cd nanocourse
mkdir source build
```

Now we need to create a file named `CMakeLists.txt` inside `source` with the following contents:

\include test/snippet/setup/CMakeLists.txt

Alternatively, you can download the file [here]
(https://raw.githubusercontent.com/seqan/learning-resources/master/test/snippet/setup/CMakeLists.txt).

# Compiling and Running

To test whether everything works, we will now compile and run a small example.

First we create the file `hello_world.cpp` in the `source` directory with the following contents:

\include test/snippet/setup/hello_world.cpp

Alternatively, you can download the file [here]
(https://raw.githubusercontent.com/seqan/learning-resources/master/test/snippet/setup/hello_world.cpp).

To compile it, we run the following commands inside the `build` directory:

```bash
cmake ../source
make
./hello_world
```

The output should be `Hello world`.

\remark Depending on the standard C++ on your system, you may need to specify the compiler via `-DCMAKE_CXX_COMPILER=`, for example:
```bash
cmake ../source -DCMAKE_CXX_COMPILER=/path/to/executable/g++-7
```

\note In some situations it can happen that the correct assembler is not found.
You will see an error during the cmake configuration that says something like: `... could not understand flag m ...`.
In this case you can try to export the Path:
```
export PATH=/util/bin:$PATH
```
and try running cmake again.

# Adding a new source file to your project

For example, to add the new `task1.cpp` file to your `CMakeLists.txt`, append the following line to your file:
```cmake
add_executable (task1 task1.cpp)
```

To compile the new file, run `cmake .` and `make` inside the `build` folder.
