# Parallel Matrix Multiplication

Implementation of parallel matrix multiplication in C++ using process and threads.

## About

The aim of this project is to implement the usual [matrix multiplication](https://en.wikipedia.org/wiki/Matrix_multiplication) using multiprogramming.
The multiplication is done in parallel, with theads and process, and also in sequential.

The purpose is to compare how the multiplication time changes over the differents parameters, such as the number of threads and the matrix size.

## How to run

### Dependencies

- ```g++```
- ```makefile```
- ```cmake```
- ```Python 3.7+``` (just to run the experiments)

### Compiling
In a Linux machine, clone this repository with `git clone` or by direct download
In the project directory, do:

```
$ mkdir build && cd build
```
```
$ cmake ../
```
```
$ make
```

### Executing
The binaries are generated in the folder ```bin/```.
The project creates four binaries: auxiliar, sequential, process and thread

To execute one of them, just type:
```
$ ./bin/<exe_name>
```

## Instructions on how to use

### auxiliar
The `auxiliar` program recives 4 arguments in command line:  
```
$ ./bin/auxiliar nlin1 ncol1 nlin2 ncol2 
```

They are the dimentions of two different matrices that will be created.
The matrices are stored into the 📂 **matrix**, on the root directory, with the names: matrix_<nlin1>x<ncol1>_0 matrix_<nlin2>x<ncol2>_1
  
### sequential
The `sequential` program reads two matrices and makes the product of them.
  
The `sequential` program recives 2 arguments in command line:  
```
$ ./bin/auxiliar matrix1 matrix2
```
They are the names of the matrices to be multiplied.
The matrices are readed from the  📂 **matrix** folder.
  
The product result is stored in the 📂 **results** folder, with name: matrix_result_sequential_<nlin3>x<ncol3>
Where nlin3 and ncol3 are the result matrix dimentions.
  
### process and threads
The `process` and the `threads` programs work the same way: they read the two matrices and also an integer p, which represents the number of elements in the final matrix that each of the process/threads should calculate.
The number of process/threads is calculated in the execution.
  
The program recives 3 arguments in command line:  
```
$ ./bin/<process or thread> matrix1 matrix2 p
```
The matrices are readed from the  📂 **matrix** folder.
  
The product result is stored in the 📂 **results** folder. 
Each one of the process/threads creates its own result file, containing the elements that it's responsible for, with name: matrix_result_<process/threads>_<nlin3>x<ncol3>_<id>
Where nlin3 and ncol3 are the result matrix dimentions, and <id> a number representing the process/threads that created the file.
  
 
## The experiments
As mentioned before, the purpose of the project is to compare the implementations. To generate and read all the data from the programs' executions, there are two python scripts in the 📂 **experiments**.
  
### experiment_01.py
This script executes the programs `sequential`, `process`, and `thread` with monotonically growing size square matrices (generated automatically by `auxiliar`) and stores (in a .json file) the time each algorithm takes to fully complete the multiplications. 
  
The program stops when the time taken by the sequential algoritm is more than 2 mins.

The result filename can be passed via command line.
  
```
$ python experiment_01.py <result filename>
```
  
### experiment_02.py
This script executes the programs `process`, and `thread` with especific square matrices and different p values and stores (in a .json file) the time each algorithm takes to fully complete the multiplications. 

The result filename can be passed via command line.
  
```
$ python experiment_02.py <result filename>
```

*Obs.* There also a makefile used to execute the experiments multiple times, just type `make experiment_01` or `make experiment_02` in the 📂 **experiments**.
