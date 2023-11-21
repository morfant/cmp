
#include <ctime>
#include "c74_min.h"
#include "XoshiroCpp.hpp"

using namespace c74::min;
const int WIDTH = 210;
const int HEIGHT = 210;

// https://github.com/Reputeless/Xoshiro-cpp
XoshiroCpp::Xoshiro256PlusPlus rng(time(0));

class cellular_automata_tilde :
        public object<cellular_automata_tilde>,
        public sample_operator<0, 1>
{
private:
    int count = 0;
    int sampleRate, vectorSize;
    
    bool current[WIDTH][HEIGHT];
    bool next[WIDTH][HEIGHT];
    vector<float> currentFlat;
    

public:
    MIN_DESCRIPTION	{ "Cellular-automata based noise generator" };
    MIN_TAGS		{ "audio, noise, oscillator" };
    MIN_AUTHOR		{ "giy.hands@gmail.com" };
    MIN_RELATED		{ "cellular_automata_tilde~, saw~" };

    inlet<>  in1    {this, "messages (init num, next)"};
    outlet<> out1   {this, "(msg) msg out"};
    outlet<> out2   {this, "(signal) signal out", "signal"};
   
    
    cellular_automata_tilde(const atoms& args = {}) {
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
        auto r = dist(rng);
        
        if (r > den) return 1;
        else return 0;
    }
    
    void flatCurrentVector(bool arr[][HEIGHT])
    {
        // 2차원 배열을 1차원 배열로 펼치기
        currentFlat.clear();
        
        for (int x = 1; x < WIDTH - 1; ++x) {
            for (int y = 1; y < HEIGHT - 1; ++y) {
                currentFlat.push_back(boolToVal(arr[x][y]));
            }
        }
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
            
            drawCurrent();
            flatCurrentVector(current);
            
            return {};
        }
    };

    message<> drawCurrent {this, "draw",
        MIN_FUNCTION{
           for (int x = 0; x < WIDTH; x++) {
                for (int y = 0; y < HEIGHT; y++) {
                    int r = current[x][y];
                    out1.send("setcell", x, y, "val", 255, 255 * r, 255 * r, 255 * r);
                }
            }
            
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
            
            flatCurrentVector(next);
            std::swap(current, next);   // current = next;
            drawCurrent();
            
            return {};
        }
    };

    sample operator()() {
        count %= currentFlat.size();
        return currentFlat.at(count++);
    }
    
    
};

MIN_EXTERNAL(cellular_automata_tilde);
