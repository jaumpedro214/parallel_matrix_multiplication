#include "matrix_multiplier.hpp"


std::string BaseClassMultiplier::matrix_dir = "../matrix/";

// BaseClassMuitliplier
BaseClassMultiplier::BaseClassMultiplier(char* file1, char* file2){
    this->filename1 = BaseClassMultiplier::matrix_dir + file1;
    this->filename2 = BaseClassMultiplier::matrix_dir + file2;
}

void BaseClassMultiplier::open_files(){
    // Open the necessary files for the operation 

    this->matrix_file1.open( this->filename1 ); 
    this->matrix_file2.open( this->filename2 );

    if (!this->matrix_file1){
        throw std::runtime_error("Could not open file: "+ this->filename1);
        return;
    }
    if (!this->matrix_file2){
        throw std::runtime_error("Could not open file: "+ this->filename2);
        return;
    }
    
    //Reading dimentions
    this->matrix_file1 >> this->nlin1 >> this->ncol1;
    this->matrix_file2 >> this->nlin2 >> this->ncol2;

    //Checking dimension
    if ( this->ncol1 != this->nlin2 ){
        throw std::runtime_error("Matrix sizes don't match.");
        return;
    }

    this->nlin3 = this->nlin1;
    this->ncol3 = this->ncol2;

    this->filename_output = BaseClassMultiplier::matrix_dir+"matrix_result_sequential_";
    this->filename_output += ( std::to_string(this->nlin3) +"x"+ std::to_string(this->ncol3) );
    
    //unique file id
    this->filename_output += ( "_"+std::to_string( rand() % 89999 + 10000 ) );

    matrix_output.open( this->filename_output );

    if (!this->matrix_output){
        throw std::runtime_error("Could not create file: "+ this->filename_output);
        return;
    }
}

// SequentialMultiplier
// ===================================================== //

void SequentialMultiplier::run(){
    // Run the process needed for
    this->read_matrices();
    this->multiply();
    this->save();
}

void SequentialMultiplier::read_matrices(){
    // read the two matrices and store them into
    // two separated vectors
    usi value;
    std::cout << "Reading the matrices from the files...\n";

    for( unsigned int i = 0; i < this->nlin1*this->ncol1; i++ ){
        this->matrix_file1 >> value;
        this->matrix1.push_back( value );
    }

    for( unsigned int i = 0; i < this->nlin2*this->ncol2; i++ ){
        this->matrix_file2 >> value;
        this->matrix2.push_back( value );
    }

    this->matrix_file1.close();
    this->matrix_file2.close();

    std::cout << "Finish.\n\n";
}

void SequentialMultiplier::multiply(){
    // Multiply the matrices
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(), end;

    std::cout << "Making the product between the matrices...\n";

    for( unsigned int i = 0; i < this->nlin3; i++ ){
        for( unsigned int j = 0 ; j < this->ncol3; j++ ){
            this->matrix3.push_back( this->calculate_position(i,j) );
        }   
    }

    end = std::chrono::steady_clock::now();
    this->delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    
    std::cout << "Product complete.\n\n";
}

usi SequentialMultiplier::calculate_position( unsigned int i, unsigned int j ){
    // Calculating matrix product using the flatten matrices
    // recieve a line (i) and a column (j)
    // and return the product
    usi value = 0;
    for( unsigned int k = 0; k < this->ncol1; k++ ){
        value += this->matrix1[ i*this->ncol1 + k ]*this->matrix2[ j + k*this->ncol2 ];
    }
    return value;
}

void SequentialMultiplier::save(){
    //Store the final info into the file:

    std::cout << "Storing final results in: " << this->filename_output << "\n";

    this->matrix_output << this->nlin3 << " " << this->ncol3 << "\n";

    for( unsigned int i = 0; i < this->nlin3; i++ ){
        for( unsigned int j = 0 ; j < this->ncol3; j++ ){
            this->matrix_output << "c"<<i<<j<<" ";
            this->matrix_output << this->matrix3[ i*this->ncol3 + j ];
            this->matrix_output << "\n";
        }   
    }
    this->matrix_output << this->delta_time <<"\n";
    this->matrix_output.close();

    std::cout << "Process finished, closing the program.\n";
}

// MultiprocessMultiplier