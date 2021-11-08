#ifndef MATRIX_GENERATOR_HPP
#define MATRIX_GENERATOR_HPP

#include <string>
#include <vector>

#include <stdlib.h>
#include <time.h>

#include <fstream>

// Using short integers to minimize memory cost
using usi = unsigned short int;

class MatrixGenerator{
    
    private:
    usi nlin, ncol;
    long filesize;

    std::ofstream matrix_file;
    std::string filename;

    static std::string root_filename;
    static int matrix_count; 

    void open_file();
    void close_file();

    public:
    MatrixGenerator(usi nlin, usi ncol);
    void generate();
    void log_info();

};

#endif