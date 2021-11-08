#include "matrix_generator.hpp"

// Static variables initialization
std::string MatrixGenerator::root_filename = "../matrix/matrix_";
int MatrixGenerator::matrix_count = 0; 

// Constructors
MatrixGenerator::MatrixGenerator(usi nlin, usi ncol){
    this->nlin = nlin;
    this->ncol = ncol;
}

void MatrixGenerator::generate(){
    
    this->open_file();

    // setting seed for reproducibility
    srand(214);

    for( usi i = 0, value ; i < this->nlin*this->ncol; i++){
        // Generating random numbers in the [0-100] interval 
        value = (usi) (rand() % 101);
        this->matrix_file << value << '\n';
    }
    
    this->log_info();
    this->close_file();

    // increase the matrix count
    MatrixGenerator::matrix_count++;
}

void MatrixGenerator::open_file(){
    // root file address
    this->filename = MatrixGenerator::root_filename;
    
    // Matrix dimensions
    this->filename += std::to_string(this->nlin) +"x"+ std::to_string(this->ncol);
    
    // unique file id
    this->filename += "_"+std::to_string( rand() % 89999 + 10000 );

    this->matrix_file.open( this->filename );
}

void MatrixGenerator::close_file(){
    this->matrix_file.close();
}

void MatrixGenerator::log_info(){
    printf( "Generating matrix %u x %u\n", this->nlin, this->ncol );
    printf( "Saving to file: %s.txt (~%.1f Kb)\n\n", 
            this->filename.c_str(),
            ((float) this->matrix_file.tellp())/1024
          );

}
