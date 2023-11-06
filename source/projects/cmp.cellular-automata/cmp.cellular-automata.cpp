/// @file
///    @ingroup     cmp
///    @copyright    Copyright 2023 giy.hands@gmail.com All rights reserved.
///    @license    Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
# include <random>
# include "XoshiroCpp.hpp"

using namespace c74::min;
using namespace XoshiroCpp;

 
const int WIDTH = 400;
const int HEIGHT = 400;
bool current[WIDTH][HEIGHT];
bool next[WIDTH][HEIGHT];
bool isBang = false;

// https://github.com/Reputeless/Xoshiro-cpp
Xoshiro256PlusPlus rng(222);

class cellular_automata : public object<cellular_automata>, public matrix_operator<> {
public:
   
    MIN_DESCRIPTION     { "Cellular Automata implemention as jit object." };
    MIN_TAGS            { "math" };
    MIN_AUTHOR          { "Cycling '74" };
    MIN_RELATED         { "jit.clip" };

    inlet<>  input      { this, "(matrix) Input", "matrix" };
    outlet<> output     { this, "(matrix) Output", "matrix" };
    
    /*
    cellular_automata(const atoms& args = {}) {
        
    };
    */
    
    
    // This object process each cell independently
    // So we define "calc_cell" instead of "calc_matrix"
    template<typename T>
    T calc_cell(T input, const matrix_info& info, matrix_coord& position) {
        T output;
        
        return output;
    }


    // We override the case for the char type to use the cached attribute values in the 0-255 range
    pixel calc_cell(pixel input, const matrix_info& info, matrix_coord& position) {
        pixel output;
       
        if (isBang) {
            cout << "width: " << info.width() << endl;
            cout << "height: " << info.height() << endl;
            cout << "plane count: " << info.plane_count()<< endl;
            cout << "dimension count: " << info.dim_count() << endl;
            isBang = false;
        }
       
        auto x = position.x();
        auto y = position.y();
        int r = current[x][y];
        auto v = r * 255;
        
        output[alpha] = 255;
        output[red] = output[green] = output[blue] = v;

        return output;
    }
    
    
    bool densityRand (float den) {
        std::uniform_int_distribution<int> dist(0, 100);
//        auto r = lib::math::random(0, 100); // default random function (slower)
        auto r = dist(rng);
        
        if (r > den) return 1;
        else return 0;
    }
    
   
    message<> randGrid { this, "init",
        MIN_FUNCTION {

            cout << "init()" << endl;
            isBang = true;
            
            float density = args[0];

            for (int x = 0; x < WIDTH; x++) {
                for (int y = 0; y < HEIGHT; y++) {
                    current[x][y] = densityRand(density);
                }
            }
            return {};
        }
    };

    
    message<> nextGen {this, "next",
        MIN_FUNCTION {
//            cout << "next()" << endl;
            for (int x = 1; x < WIDTH - 1; x++) {
                for (int y = 1; y < HEIGHT - 1; y++) {
                   int neighbourWallCount =
                    current[x - 1][y - 1] +
                    current[x    ][y - 1] +
                    current[x + 1][y - 1] +
                    current[x - 1][y    ] +
//                    current[x    ][y    ] +
                    current[x + 1][y    ] +
                    current[x - 1][y + 1] +
                    current[x    ][y + 1] +
                    current[x + 1][y + 1];

                    if (neighbourWallCount >= 4) {
                        next[x][y] = 1;
                    } else {
                        next[x][y] = 0;
                    }
                }
            }

            std::swap(current, next);   // current = next;

            return {};
        }
    };


private:
    
};

MIN_EXTERNAL(cellular_automata);
