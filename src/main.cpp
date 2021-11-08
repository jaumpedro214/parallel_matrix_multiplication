#include <vector>
#include <iostream>
#include <string>

#include "matrix_generator.cpp"
#include "matrix_generator.hpp"

using namespace std;

int main( int argc, char* argv[] ){
    // Matrix dimensions
    int lin1, col1, lin2, col2;
    vector<int> alo;
    MatrixGenerator mat_gen = MatrixGenerator( 10,10 );

    if ( argc < 5 ){
        cout << "Insufiscient number of arguments";
    }
    else{
        // Matrix dimensions
        lin1 = stoi(argv[1]);
        col1 = stoi(argv[2]);
        lin2 = stoi(argv[3]);
        col2 = stoi(argv[4]);
        MatrixGenerator( 10,10 );
    }

    return 0;
}