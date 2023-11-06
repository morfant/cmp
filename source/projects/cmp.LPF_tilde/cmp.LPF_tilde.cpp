/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#define ROOT2 1.4142135623

using namespace c74::min;


class LPF_tilde : public object<LPF_tilde>, public vector_operator<> {
private:
    size_t m_sr;
    size_t m_vs;
    double m_inputDelay[2] = {};
    double m_outputDelay[2] = {};
    
public:
    MIN_DESCRIPTION	{"Post to the Max Console."};
    MIN_TAGS		{"utilities"};
    MIN_AUTHOR		{"Cycling '74"};
    MIN_RELATED		{"print, jit.print, dict.print"};

    inlet<>  input1	{ this, "(signal) input source" };
    inlet<>  input2 { this, "(signal) cutoff freq(Hz)" };
    outlet<> output	{ this, "(signal) output", "signal" };

    message<> dspsetup { this, "dspsetup",
        MIN_FUNCTION {
            m_sr = args[0];
            m_vs = args[1];
            cout << "SampleRate: " << m_sr << " / Signal Vector Size: " << m_vs << endl;
            
            return {};
        }
    };
    
    void operator()(audio_bundle input, audio_bundle output){
        auto in = input.samples(0);
        auto out = output.samples(0);
        double freq = (double)*input.samples(1);
        
        double c = 1 / tan(PI * freq / m_sr);
        double cc = c * c;
        double a0 = 1 / (1 + ROOT2 * c + cc);
        double a1 = 2 * a0;
        double a2 = a0;
        double b1 = 2 * a0 * (1 - cc);
        double b2 = a0 * (1 - ROOT2 * c + cc);
        
        for (auto i = 0; i < m_vs; i++) {
            
            out[i] =
            a0 * in[i] +
            a1 * m_inputDelay[0] + a2 * m_inputDelay[1] -
            b1 * m_outputDelay[0] - b2 * m_outputDelay[1];
            
            m_outputDelay[1] = m_outputDelay[0];
            m_outputDelay[0] = out[i];
            
            m_inputDelay[1] = m_inputDelay[0];
            m_inputDelay[0] = in[i];
            
        }
        
    }

};


MIN_EXTERNAL(LPF_tilde);
