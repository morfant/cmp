/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#define DEL_LEN 1024

using namespace c74::min;


class comb_tilde : public object<comb_tilde>, public vector_operator<> {
private:
    int     m_sr;
    int     m_vectorsize;
    double  m_samps[DEL_LEN];
    
public:
    MIN_DESCRIPTION	{"Post to the Max Console."};
    MIN_TAGS		{"utilities"};
    MIN_AUTHOR		{"Cycling '74"};
    MIN_RELATED		{"print, jit.print, dict.print"};

    inlet<>  input_sig	    { this, "(signal) sig to be filtered", "signal" };
    inlet<>  input_order    { this, "(signal) order", "signal" };
    inlet<>  input_coef     { this, "(signal) coef", "signal" };
    outlet<> output	        { this, "(signal) filtered output", "signal" };
    
    comb_tilde(const atoms& args = {}) {
        
    }
    
    ~comb_tilde(){}


    message<> dspsetup { this, "dspsetup",
        MIN_FUNCTION {
            m_sr = args[0];
            m_vectorsize = args[1];
            cout << "SampleRate: " << m_sr << ", Signal Vector Size: " << m_vectorsize << endl;
            return {};
        }
    };
    
    void operator()(audio_bundle input, audio_bundle output) {
        auto in = input.samples(0);
        auto out = output.samples(0);
        auto order = (int)*input.samples(1);
        auto coef = (double)*input.samples(2);
        
        for (auto i = 0; i < m_vectorsize; i++) {
            auto val = in[i];
            
            out[i] = val + m_samps[order] * coef;
            
            for (auto j = order; j > 0; j--) {
                m_samps[j] = m_samps[j - 1];
            }
            m_samps[0] = val;
        }
    }

};


MIN_EXTERNAL(comb_tilde);
