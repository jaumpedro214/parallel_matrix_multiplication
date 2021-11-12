#ifndef MATRIX_MULTIPLIER_HPP
#define MATRIX_MULTIPLIER_HPP

#include "matrix_generator.hpp"
#include <string>
#include <vector>
#include <fstream>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/shm.h> // Shared memmory

#include <thread>

#include <chrono>

#define timemeasurement std::chrono::milliseconds

class BaseClassMultiplier{
    
    protected:
    usi nlin1, ncol1, nlin2, ncol2;
    usi nlin3, ncol3;

    static std::string matrix_dir, result_dir;
    std::string filename1, filename2, filename_output;
    
    std::ifstream matrix_file1, matrix_file2;
    std::ofstream matrix_output;

    long int delta_time;
    std::chrono::steady_clock::time_point begin;

    public:
    BaseClassMultiplier(char* file1, char* file2);
    void open_input_files();
};

class SequentialMultiplier: public BaseClassMultiplier{
    using BaseClassMultiplier::BaseClassMultiplier;
    
    private:
    std::vector<usi> matrix1, matrix2, matrix3;

    void read_matrices();
    void multiply();
    void open_output_files();
    usi calculate_position( unsigned int i, unsigned int j );
    void save();

    public:
    void run();
};

class MultiprocessMultiplier: public BaseClassMultiplier{
    using BaseClassMultiplier::BaseClassMultiplier;
    
    private:
    int p;

    usi m_start, m_end; // Each process' interval
    usi id; // process local identifier
    
    int matrix1_seg_id, matrix2_seg_id;
    usi *matrix1, *matrix2;
    std::vector<usi> matrix3;

    void create_shared_memmory_space();
    void create_process();
    void read_matrices();
    void multiply();
    void open_output_files();
    usi calculate_position( unsigned int i, unsigned int j );
    void save();

    public:
    MultiprocessMultiplier(char* file1, char* file2, int p)
        :BaseClassMultiplier{ file1, file2 }, p{ p }
        {;}

    void run();

};

class MultithreadMultiplier: public BaseClassMultiplier{
    using BaseClassMultiplier::BaseClassMultiplier;
    
    private:
    int p;

    usi m_start, m_end;
    usi id; 
    
    std::vector<usi> matrix1, matrix2;

    void read_matrices();
    void create_thread();
    usi  calculate_position( unsigned int i, unsigned int j );

    void thread_multiply( usi m_start, usi m_end, usi id );
    void thread_save_output_files( usi m_start, usi m_end, usi id, std::vector<usi> const &matrix3, long int delta_time );

    public:
    MultithreadMultiplier(char* file1, char* file2, int p)
        :BaseClassMultiplier{ file1, file2 }, p{ p }
        {;}

    void run();

};

#endif