/// @file
///	@ingroup 	cmp
///	@copyright	Copyright 2023 giy.hands@gmail.com All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"

using namespace c74::min;

class lnoise_tilde : public object<lnoise_tilde>, public sample_operator<0, 1> {
private:
    char axiom = 'A';
    string current = "";
    string next = "";
    
public:
    MIN_DESCRIPTION	{ "Clip noise generator" };
    MIN_TAGS		{ "audio, noise, oscillator" };
    MIN_AUTHOR		{ "giy.hands@gmail.com" };
    MIN_RELATED		{ "lnoise_tilde~" };

    inlet<>  in1 {this, "bang"};
    outlet<> out1 {this, "audio out", "signal"};
    
    void generate()
    {
        cout << current.size() << endl;
        next = "";
        
        for (int i = 0; i < current.size(); i++)
        {
            char c = current[i];
            
            if (c == 'A')
            {
                next = next + "DBF";
            }
            else if (c == 'B')
            {
                next = next + "CAC";
            }
            else if (c == 'C')
            {
                next = next + "EDA";
            }
            else if (c == 'D')
            {
                next = next + "EAB";
            }
            else if (c == 'E')
            {
                next = next + "ACF";
            }
            else if (c == 'F')
            {
                next = next + "DBC";
            }
            else
            {
                next = next + c;
            }
            
        }
        
        current = next;
        cout << current << endl;
        
    }
   
    message<> bang { this, "bang",
        MIN_FUNCTION {
            
            generate();
            
            return {};
        }
    };
    
    message<> init { this, "init",
        MIN_FUNCTION {
            
            current = axiom;
            cout << current << endl;
            
            return {};
        }
    };
    
    sample operator()()
    {
       
        return 0;
    };
    
};

MIN_EXTERNAL(lnoise_tilde);



