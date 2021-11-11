#include <iostream>

#include "matrix_multiplier.cpp"
#include "matrix_multiplier.hpp"

#include <string>

using namespace std;

int main( int argc, char* argv[] ){
    MultiprocessMultiplier *pro_mult;

    srand(214);

    if ( argc < 4 ){
        cout << "Insufiscient number of arguments\n";
    }
    else{
        pro_mult = new MultiprocessMultiplier( argv[1], argv[2], stoi(argv[3]) );
        try{
            pro_mult->run();
        }
        catch( std::exception &ex ){
            cout << "Aborted: " << ex.what() << "\n";
            delete pro_mult;
            return -1;
        }
        delete pro_mult;
    }

    return 0;
}