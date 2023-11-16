/// @file
///	@ingroup 	cmp
///	@copyright	Copyright 2023 giy.hands@gmail.com All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"

using namespace c74::min;

class clipnoise_tilde : public object<clipnoise_tilde>, public sample_operator<1, 1> {
private:
    int a = 10;
    
public:
    MIN_DESCRIPTION	{ "Clip noise generator" };
    MIN_TAGS		{ "audio, noise, oscillator" };
    MIN_AUTHOR		{ "giy.hands@gmail.com" };
    MIN_RELATED		{ "clipnoise_tilde~" };

    inlet<>  in1 {this, "amplitude", "signal"};
    outlet<> out1 {this, "audio out", "signal"};
    
    sample operator()(sample x) {
        
        float v = 0;
        int r = rand() % 2;
        
        if (r == 0)
        {
            v = -1.0;
        }
        else if (r == 1)
        {
            v = 1.0;
        }
        
        return v * x;
    }
};

MIN_EXTERNAL(clipnoise_tilde);



