/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include <algorithm>

using namespace c74::min;


class l_noise_tilde : public object<l_noise_tilde>, public sample_operator<0, 1> {
private:
    int count = 0;
    char axiom = 'A';
//    char axiom = 'B';
//    char axiom = 'C';
//    char axiom = 'D';
//    char axiom = 'E';
//    char axiom = 'F';
    string current;
    vector<float> values;
    int genCount = 0;
    

public:
    MIN_DESCRIPTION	{ "L-System based noise generator." };
    MIN_TAGS		{ "audio, noise" };
    MIN_AUTHOR		{ "giy.hands@gmail.com" };
    MIN_RELATED		{ "l_noise_tilde~, noise~" };

    inlet<>  in1    {this, "(number) frequency"};
    outlet<> out1   {this, "(signal) audio signal", "signal"};
    outlet<> out2   {this, "(anything) values" };
    
    l_noise_tilde(const atoms& args = {}) {
        current = axiom;
        cout << "current in constructor: " << current << endl;
    }
    
    float randf(float scale)
    {
        float r = -1 + ((float)rand() / ((float)RAND_MAX/2) ); // -1.0 ~ 1.0
        return r * scale;
    }
 
    
    void charToAmp()
    {
        values.clear();
        
        for (char c : current) {
            float d = 0;
                
            if (c == 'A') {
//                float d = randf(0.1);
                values.push_back(0.3 + d);
            } else if (c == 'B') {
//                float d = randf(0.1);
                values.push_back(-0.3 + d);
            } else if (c == 'C') {
//                float d = randf(0.1);
                values.push_back(0.5 + d);
            } else if (c == 'D') {
//                float d = randf(0.1);
                values.push_back(-0.5 + d);
            } else if (c == 'E') {
//                float d = randf(0.1);
                values.push_back(-0.9 + d);
            } else if (c == 'F') {
//                float d = randf(0.1);
                values.push_back(0.9 + d);
            } else if (c == 'G') {
                float r = -1 + ((float)rand() / ((float)RAND_MAX/2) );
                values.push_back(r);
            } else {
                values.push_back(0);
            }
        }
        
        count = 0;
        
        
        // Post values
//        cout << "size of values: " << values.size() << endl;
//        for (float f : values)
//        {
//            out2.send(f);
//        }
//        cout << "--------------------------" << endl;
        
        
    }
    
    void generate()
    {
        genCount++;
        cout << "Gen count: " << genCount << endl;

        // A->AB
        // B->A

//        cout << "current in generate(): " << current << endl;
        string next;

        for (int i = 0; i < current.size(); i++)
        {

            char c = current[i];

            if (c == 'A') {
                next = next + "ACD";
            } else if (c == 'B') {
                next = next + "BDB";
            } else if (c == 'C') {
                next = next + "DEC";
            } else if (c == 'D') {
                next = next + "BED";
            } else if (c == 'E') {
                next = next + "FEG";
            } else if (c == 'F') {
                next = next + "CGF";
            } else if (c == 'G') {
                next = next + "AFG";
            } else {
                next = next + c; // bypass
            }

        }

        current = next;
//        cout << "current after generate(): " << current << endl;

    }
    
    argument<number> steps_arg { this, "steps", "Initial steps.",
        MIN_ARGUMENT_FUNCTION {
            steps_attr = arg;
        }
    };
    
    attribute<number> steps_attr { this, "steps", 1024,
        description {"number of points on Bezier curve."},
    };
     
       
    message<> bang {this, "bang",
        MIN_FUNCTION {
            
            generate();
            charToAmp();
   
            return {};
        }
    };
    
    message<> reset {this, "reset",
        MIN_FUNCTION {
            genCount = 0;
            current = axiom;
            cout << "RESET!" << endl;
            return {};
        }
    };

    message<> getAxiom {this, "axiom",
        MIN_FUNCTION {
            int iaxiom = args[0];
            axiom = (char)iaxiom;
            cout << axiom << endl;
            current = axiom;
            return {};
        }
    };
    
    sample operator()() {
        count %= values.size();
//        return values[count++];
        return values.at(count++);
    }
};

MIN_EXTERNAL(l_noise_tilde);












