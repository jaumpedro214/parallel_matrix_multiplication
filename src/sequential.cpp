#include <iostream>

#include "matrix_multiplier.cpp"
#include "matrix_multiplier.hpp"

using namespace std;

int main( int argc, char* argv[] ){
    SequentialMultiplier *seq_mult;

    srand(214);

    if ( argc < 3 ){
        cout << "Insufiscient number of arguments\n";
    }
    else{
        seq_mult = new SequentialMultiplier( argv[1], argv[2] );
        try{
            seq_mult->run();
        }
        catch( std::exception &ex ){
            cout << "Aborted: " << ex.what() << "\n";
            delete seq_mult;
            return -1;
        }
        delete seq_mult;
    }

    return 0;
}