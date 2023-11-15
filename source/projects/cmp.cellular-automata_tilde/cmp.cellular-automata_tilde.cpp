/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include "XoshiroCpp.hpp"

using namespace c74::min;
const int WIDTH = 210;
const int HEIGHT = 210;

// https://github.com/Reputeless/Xoshiro-cpp
XoshiroCpp::Xoshiro256PlusPlus rng(222);

class cellular_automata_tilde :
        public object<cellular_automata_tilde>,
        public vector_operator<>
{
private:
    bool current[WIDTH][HEIGHT];
//    int current[WIDTH][HEIGHT];
    vector<float> currentFlat;
    bool next[WIDTH][HEIGHT];
//    int next[WIDTH][HEIGHT];

    int vectorCount = 0;
    int sampleRate, vectorSize;
    

public:
    MIN_DESCRIPTION	{ "Bezier what??" };
    MIN_TAGS		{ "audio, oscillator" };
    MIN_AUTHOR		{ "giy.hands@gmail.com" };
    MIN_RELATED		{ "cellular_automata_tilde~, saw~" };

    inlet<>  in1    {this, "(matrix) matrix in", "matrix"};
    outlet<> out1   {this, "(msg) msg out"};
    outlet<> out2   {this, "(signal) signal out", "signal"};
   
    
//    cellular_automata_tilde(const atoms& args = {}) {
//    }
    
     
    float intToVal(int i)
    {
        float rslt = 0.f;
        rslt = (i - 4) * 0.25;
        return rslt;
    }
   

    float boolToVal(bool b)
    {
        float rslt = 0.f;
        if (b == true)
        {
            rslt = 1.0;
        }
        else
        {
            rslt = -1.0;

        }
        return rslt;
    }
   
       
    bool densityRand (float den) {
        std::uniform_int_distribution<int> dist(0, 100);
//        auto r = lib::math::random(0, 100); // default random function (slower)
        auto r = dist(rng);
        
        if (r > den) return 1;
        else return 0;
    }
   
    
    message<> dspsetup {this, "dspsetup",
        MIN_FUNCTION {
            sampleRate = args[0];
            vectorSize = args[1];
            
            cout << "SampleRate: " << sampleRate << ", vectorSize: " << vectorSize << endl;
            return {};
        }
        
    };
   
    message<> randGrid { this, "init",
        MIN_FUNCTION {

            cout << "init()" << endl;
            
            float density = args[0];

            for (int x = 0; x < WIDTH; x++) {
                for (int y = 0; y < HEIGHT; y++) {
                    current[x][y] = densityRand(density);
                }
            }
            
             
            // 2차원 배열을 1차원 배열로 펼치기
            currentFlat.clear();
            
            for (int x = 0; x < WIDTH; ++x) {
                for (int y = 0; y < HEIGHT; ++y) {
                    currentFlat.push_back(boolToVal(current[x][y]));
//                    currentFlat.push_back(intToVal(current[x][y]));
                }
            }
           
            return {};
        }
    };

    message<> drawCurrent {this, "draw",
        MIN_FUNCTION{
           for (int x = 0; x < WIDTH; x++) {
                for (int y = 0; y < HEIGHT; y++) {
//                    int r = ceil(current[x][y]/8.0);
                    int r = current[x][y];
                    out1.send("setcell", x, y, "val", 255, 255 * r, 255 * r, 255 * r);
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

//                    ceil((current[x - 1][y - 1])/8.0) +
//                    ceil((current[x    ][y - 1])/8.0) +
//                    ceil((current[x + 1][y - 1])/8.0) +
//                    ceil((current[x - 1][y    ])/8.0) +
////                    current[x    ][y    ] +
//                    ceil((current[x + 1][y    ])/8.0) +
//                    ceil((current[x - 1][y + 1])/8.0) +
//                    ceil((current[x    ][y + 1])/8.0) +
//                    ceil((current[x + 1][y + 1])/8.0);


                    if (neighbourWallCount >= 4) {
                        next[x][y] = 1;
//                        next[x][y] = neighbourWallCount;
                    } else {
                        next[x][y] = 0;
                    }
                }
            }
            
            drawCurrent();

            std::swap(current, next);   // current = next;
            
            
            // 2차원 배열을 1차원 vector로 펼치기
            currentFlat.clear();
            
            for (int x = 0; x < WIDTH; ++x) {
                for (int y = 0; y < HEIGHT; ++y) {
                    currentFlat.push_back(boolToVal(current[x][y]));
//                    currentFlat.push_back(intToVal(current[x][y]));

                }
            }
            
            vectorCount = 0;
            
//            for (auto v : currentFlat) {
//                cout << "currentFlat: " << v << endl;
//            }


            return {};
        }
    };

 
    message<> bang {this, "bang",
        MIN_FUNCTION {
           
            return {};
        }
    };
   
    
    void operator()(audio_bundle input, audio_bundle output) {
        auto out = output.samples(0);  // get vector for channel 0 (first channel)
        
        if (vectorCount * vectorSize >= WIDTH * (HEIGHT - 1))
        {
            vectorCount = 0;
        }
        
        for (int i = 0; i < vectorSize; i++)
        {
            out[i] = currentFlat[i + (vectorSize * vectorCount)];
        }
        
        vectorCount++;
        
    };
};

MIN_EXTERNAL(cellular_automata_tilde);
