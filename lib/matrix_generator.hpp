#ifndef MATRIX_GENERATOR_HPP
#define MATRIX_GENERATOR_HPP

#include <string>
#include <vector>

#include <stdlib.h>
#include <time.h>

#include <fstream>

using usi = unsigned int;

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