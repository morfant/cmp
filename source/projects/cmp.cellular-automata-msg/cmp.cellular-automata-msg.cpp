/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.


/// @file
///    @ingroup     cmp4
///    @copyright    Gangil Yi (giy.hands@gmail.com) All rights reserved.
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

Xoshiro256PlusPlus rng(222);

class cellular_automata_msg : public object<cellular_automata_msg> {
public:
    MIN_DESCRIPTION    {"Run cellular automat iteration."};
    MIN_TAGS        {"utilities"};
    MIN_AUTHOR        {"giy for 2023 cmp4 class"};
    MIN_RELATED        {"jit.matrix, lcd"};

//    inlet<>  input    { this, "(bang) Run next iteration.", "matrix" };
//    outlet<> output    { this, "(anything) output the message which is posted to the max console", "matrix" };
//
    inlet<>  input    { this, "Input" };
    outlet<> output    { this, "Output" };



    // define an optional argument for setting the message
    argument<symbol> greeting_arg { this, "greeting", "Initial value for the greeting attribute.",
        MIN_ARGUMENT_FUNCTION {
            greeting = arg;
        }
    };


    // the actual attribute for the message
    attribute<symbol> greeting { this, "greeting", "hello world!!",
        description {
            "Greeting to be posted. "
            "The greeting will be posted to the Max console when a bang is received."
        }
    };


    // respond to the bang message to do something
    message<> bang { this, "bang", "Post the greeting.",
        MIN_FUNCTION {
            symbol the_greeting = greeting;    // fetch the symbol itself from the attribute named greeting

            cout << the_greeting << endl;    // post to the max console
//            output.send("setcell", 20, 0, "val", 255, 255, 10, 100);       // send out our outlet
            return {};
        }
    };
    
    bool densityRand (float den) {
        std::uniform_int_distribution<int> dist(0, 100);
//        auto r = lib::math::random(0, 100); // slower
        auto r = dist(rng);
        
        if (r > den) return 1;
        else return 0;
    }
    
 
    message<> drawCurrent {this, "draw",
        MIN_FUNCTION{
           for (int x = 0; x < WIDTH; x++) {
                for (int y = 0; y < HEIGHT; y++) {
                    int r = current[x][y];
                    output.send("setcell", x, y, "val", 255, 255 * r, 255 * r, 255 * r);
                }
            }
            
            return {};
        }
    };
    
    message<> randGrid { this, "init",
        MIN_FUNCTION {
            float density = args[0];
            
            for (int x = 0; x < WIDTH; x++) {
                for (int y = 0; y < HEIGHT; y++) {
                    int r = densityRand(density);
                    current[x][y] = r;
                }
            }
            
            drawCurrent();
            
            return {};
            
        }
    };
   
    message<> nextGen {this, "next",
        MIN_FUNCTION {
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
            
            drawCurrent();

            std::swap(current, next);   // current = next;

            return {};
        }
    };
   
    // post to max window == but only when the class is loaded the first time
    message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "hello world" << endl;
            return {};
        }
    };

};

MIN_EXTERNAL(cellular_automata_msg);
