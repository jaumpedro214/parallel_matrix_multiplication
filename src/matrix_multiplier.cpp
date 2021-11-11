#include "matrix_multiplier.hpp"


std::string BaseClassMultiplier::matrix_dir = "../matrix/";
std::string BaseClassMultiplier::result_dir = "../result/";

// BaseClassMuitliplier
BaseClassMultiplier::BaseClassMultiplier(char* file1, char* file2){
    this->filename1 = BaseClassMultiplier::matrix_dir + file1;
    this->filename2 = BaseClassMultiplier::matrix_dir + file2;
}

void BaseClassMultiplier::open_input_files(){
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
}

// SequentialMultiplier
// ===================================================== //

void SequentialMultiplier::run(){
    // Run the process needed for the multiplication
    this->open_input_files();
    this->read_matrices();
    this->multiply();
    this->open_output_files();
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

void SequentialMultiplier::open_output_files(){
    this->filename_output = BaseClassMultiplier::result_dir+"matrix_result_sequential_";
    this->filename_output += ( std::to_string(this->nlin3) +"x"+ std::to_string(this->ncol3) );
    
    //unique file id
    this->filename_output += ( "_"+std::to_string( rand() % 89999 + 10000 ) );

    this->matrix_output.open( this->filename_output );

    if (!this->matrix_output){
        throw std::runtime_error("Could not create file: "+ this->filename_output);
        return;
    }
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
void MultiprocessMultiplier::run(){
    // Run the process needed for the multiplication
    this->open_input_files();
    this->create_shared_memmory_space();
    this->read_matrices();
    this->create_process();
    this->multiply();
    this->open_output_files();
    this->save();
}

void MultiprocessMultiplier::create_shared_memmory_space(){
    // Creating the matrices in the shared memmory space    
    this->matrix1_seg_id = shmget(IPC_PRIVATE, 
                                  this->nlin1*this->ncol1*sizeof(usi), 
                                  IPC_CREAT | 0666 );

    this->matrix2_seg_id = shmget(IPC_PRIVATE, 
                                  this->nlin2*this->ncol2*sizeof(usi), 
                                  IPC_CREAT | 0666 );

}

void MultiprocessMultiplier::read_matrices(){
    usi value;
    std::cout << "Reading the matrices from the files...\n";
    
    // Getting the shared memmory space 
    this->matrix1 = (usi*)shmat(this->matrix1_seg_id, NULL, 0);
    this->matrix2 = (usi*)shmat(this->matrix2_seg_id, NULL, 0);

    for( unsigned int i = 0; i < this->nlin1*this->ncol1; i++ ){
        this->matrix_file1 >> value;
        this->matrix1[i] = value;
    }

    for( unsigned int i = 0; i < this->nlin2*this->ncol2; i++ ){
        this->matrix_file2 >> value;
        this->matrix2[i] = value;
    }

    this->matrix_file1.close();
    this->matrix_file2.close();

    std::cout << "Finish.\n\n";
}

void MultiprocessMultiplier::create_process(){
    usi total = this->nlin3*this->ncol3;
    usi process_number = total/this->p + ( (usi) total%this->p>0 );
    pid_t pid = 1;

    printf("Total size of result matrix: %d elements\n", total);
    printf("Number of elements per process: %d\n", this->p);
    printf("Total number of process: %d\n", process_number);

    // Separating each interval to a son
    this->m_start = 0;
    this->m_end = (usi) total%this->p>0? (usi) total%this->p: this->p;
    this->id = 0;

    //printf("Eu sou o pai (%d) e vou calcular os valores %4d %4d\n", this->id, this->m_start, this->m_end); 

    // The father creates a total of process_number-1 sons

    for( usi i = 0; i<process_number-1 and pid !=0; i++ ){
        if( pid>0 ){
            this->m_start = this->m_end;
            this->m_end = this->m_start+this->p;
            this->id++;
            pid = fork();
        }
        if( pid==0 ){
            // Getting the shared memmory space 
            this->matrix1 = (usi*)shmat(this->matrix1_seg_id, NULL, 0);
            this->matrix2 = (usi*)shmat(this->matrix2_seg_id, NULL, 0);

            //printf("Eu sou o filho %d e vou calcular os valores %4d %4d\n", this->id, this->m_start, this->m_end);    
            //break;
        }
    }

    // Recalculating the father's values
    if( pid!=0 ){
        this->m_start = 0;
        this->m_end = (usi) total%this->p>0? (usi) total%this->p: this->p;
        this->id = 0;
    }

}

void MultiprocessMultiplier::multiply(){
    // Multiply the matrices
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(), end;

    printf( "(%d) Calculating [%d %d) elements\n", this->id, this->m_start, this->m_end );

    for ( unsigned int i, j, k=this->m_start; k<this->m_end; k++ ){
        // calculating the atual line and column
        i = k/this->ncol3;
        j = k%this->ncol3;
        this->matrix3.push_back( this->calculate_position(i,j) );
    }

    end = std::chrono::steady_clock::now();
    this->delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    
    printf( "(%d) Complete\n", this->id );
}

usi MultiprocessMultiplier::calculate_position( unsigned int i, unsigned int j ){
    // Calculating matrix product using the flatten matrices
    // recieve a line (i) and a column (j)
    // and return the product
    usi value = 0;
    for( unsigned int k = 0; k < this->ncol1; k++ ){
        value += this->matrix1[ i*this->ncol1 + k ]*this->matrix2[ j + k*this->ncol2 ];
    }
    return value;
}

void MultiprocessMultiplier::open_output_files(){
    //Name for each file
    // prefix + dimentions (a x b) + process local id + unique random id
    this->filename_output = BaseClassMultiplier::result_dir+"matrix_result_process_";
    this->filename_output += ( std::to_string(this->nlin3) +"x"+ std::to_string(this->ncol3) );
    this->filename_output += "_"+std::to_string(this->id);

    //unique file id
    this->filename_output += ( "_"+std::to_string( rand() % 89999 + 10000 ) );

    this->matrix_output.open( this->filename_output );

    if (!this->matrix_output){
        throw std::runtime_error("Could not create file: "+ this->filename_output);
        return;
    }
}

void MultiprocessMultiplier::save(){
    //Store the final info into the file:

    printf( "(%d) Storing final results in: %s\n", this->id, this->filename_output.c_str() );
    
    this->matrix_output << this->nlin3 << " " << this->ncol3 << "\n";
    
    for ( unsigned int i, j, k=this->m_start; k<this->m_end; k++ ){
        // calculating the atual line and column
        i = k/this->ncol3;
        j = k%this->ncol3;
        this->matrix_output << "c"<<i<<j<<" ";
        this->matrix_output << this->matrix3[ k-this->m_start ];
        this->matrix_output << "\n";
    }
    this->matrix_output << this->delta_time <<"\n";
    this->matrix_output.close();

    std::cout << "Process finished, closing the program.\n";
}