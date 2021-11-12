#include <iostream>

#include "matrix_multiplier.cpp"
#include "matrix_multiplier.hpp"

#include <string>

using namespace std;

int main( int argc, char* argv[] ){
    MultithreadMultiplier *thr_mult;

    srand(214);

    if ( argc < 4 ){
        cout << "Insufiscient number of arguments\n";
    }
    else{
        thr_mult = new MultithreadMultiplier( argv[1], argv[2], stoi(argv[3]) );
        try{
            thr_mult->run();
        }
        catch( std::exception &ex ){
            cout << "Aborted: " << ex.what() << "\n";
            delete thr_mult;
            return -1;
        }
        delete thr_mult;
    }

    return 0;
}