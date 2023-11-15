/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include <stdint.h>

using namespace c74::min;


class noise : public object<noise>, public sample_operator<1, 1> {
private:
    float prevR = 0;
    int mCounter = 0.f;

public:
    MIN_DESCRIPTION	{ "A non-bandlimited sawtooth oscillator (a noise~ in MSP parlance)."
                      "This <a href='https://en.wikipedia.org/wiki/Sawtooth_wave'>sawtooth wave</a>"
                      "is typically used as a control signal for <a "
                      "href='https://en.wikipedia.org/wiki/Phase_(waves)'>phase</a> ramping." };
    MIN_TAGS		{ "audio, oscillator" };
    MIN_AUTHOR		{ "Cycling '74" };
    MIN_RELATED		{ "noise~, saw~" };

    inlet<>  in1 {this, "(signal) amplitude ", "signal"};
    outlet<> out1 {this, "(signal) white noise", "signal"};

    attribute<number> amp_attr { this, "amplitude", 1.0,
        description {"Amplitude of noise."}
    };
    
    attribute<number> step_attr { this, "step", 0.2,
        description {"step for brown noise."}
    };

    argument<number> amp_arg { this, "amplitude", "Initial ampitude.",
        MIN_ARGUMENT_FUNCTION {
            amp_attr = arg;
        }
    };

    message<> m_number { this, "number", "Set the amplitude.",
        MIN_FUNCTION {
            amp_attr = args;
            return {};
        }
    };
    
    message<> bang {this, "bang",
        MIN_FUNCTION {
            auto r = -1.0 + (float)(rand()) / (float)(RAND_MAX/2);
            auto d = r - prevR;
            cout << "r: " << r << " abs(d): " << abs(d) << endl;
            while (abs(d) > step_attr) {
                r = -1.0 + (float)(rand()) / (float)(RAND_MAX/2);
                d = r - prevR;
            };
            cout << "nextR: " << r << endl;
            prevR = r;
            return {};
        }
    };
    
   
    sample operator()(sample x) {
        // white noise
//      float r = -1.0 + (float)(rand()) / (float)(RAND_MAX/2);
//      return r;
        
        // brown noise
//        auto r = -1.0 + (float)(rand()) / (float)(RAND_MAX/2);
//        r *= step_attr;
//        prevR += r;
//        if (prevR > 1.0)
//        {
//            prevR = 2.0 - prevR;
//        }
//        else if (prevR < -1.0)
//        {
//            prevR = prevR = -2.0 - prevR;
//        };
//
//        return prevR;
        
        // GrayNoise
        /*
         trand(s1, s2, s3) => make random 32 bit value
             & 31 => get random value 0 ~ 31
                 1L << => get power of 2 ranged in 2**0 ~ 2**31
                     counter ^= => XOR flip(toggle) a bit of counter variable, and stores it to the counter itself
         4.65661287308e-10f => 1 / (2**31) : make counter( ~ 2**31) value as 0.0 ~ 1.0
         */
        
//        auto r = rand();
//
//        int counter = mCounter;
//        counter ^= 1L << (r & 31);
//        mCounter = counter;
//        return counter * 4.65661287308e-10f;
        
        // ClipNoise
        auto r = rand() % 2;
        int rslt = r == 0 ? -1.0 : 1.0;
        return rslt;
        
        
    }
};

MIN_EXTERNAL(noise);
