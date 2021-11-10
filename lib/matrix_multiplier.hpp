#ifndef MATRIX_MULTIPLIER_HPP
#define MATRIX_MULTIPLIER_HPP

#include "matrix_generator.hpp"
#include <string>
#include <vector>
#include <fstream>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>


class BaseClassMultiplier{
    
    protected:
    usi nlin1, ncol1, nlin2, ncol2;
    usi nlin3, ncol3;

    static std::string matrix_dir;
    std::string filename1, filename2, filename_output;
    
    std::ifstream matrix_file1, matrix_file2;
    std::ofstream matrix_output;

    long int delta_time;

    public:
    BaseClassMultiplier(char* file1, char* file2);
    void open_files();
};

class SequentialMultiplier: public BaseClassMultiplier{
    using BaseClassMultiplier::BaseClassMultiplier;
    
    private:
    std::vector<usi> matrix1, matrix2, matrix3;

    public:
    void run();
    void read_matrices();
    void multiply();
    usi calculate_position( unsigned int i, unsigned int j );
    void save();

};

class MultiprocessMultiplier: public BaseClassMultiplier{
    using BaseClassMultiplier::BaseClassMultiplier;

};

#endif