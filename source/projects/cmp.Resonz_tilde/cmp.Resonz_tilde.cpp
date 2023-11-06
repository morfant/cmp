/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"

using namespace c74::min;


class Resonz_tilde : public object<Resonz_tilde>, public vector_operator<> {
private:
    size_t m_sr;
    size_t m_vs;
    double m_piOnSR = 0;
    double m_inputDelay[2] = {};
    double m_outputDelay[2] = {};
    int m_method = 0;
    // 0 : Direct Form I,
    // 1 : Direct Form II, The Audio Programming Book, p.487
    // 2 : Direct Form II, SuperCollider Resonz source code
    
public:
    MIN_DESCRIPTION	{"Post to the Max Console."};
    MIN_TAGS		{"utilities"};
    MIN_AUTHOR		{"Cycling '74"};
    MIN_RELATED		{"print, jit.print, dict.print"};

    inlet<>  input1	{ this, "(signal) input source" };
    inlet<>  input2 { this, "(signal) center freq" };
    inlet<>  input3 { this, "(signal) reciprocal of Q" };
    inlet<>  input4 { this, "(int) select method (0 ~ 2)", "int" };
    outlet<> output	{ this, "(signal) output", "signal" };

    message<> handle_int {this, "int",
        MIN_FUNCTION {
            if (0 <= (int)args[0] && (int)args[0] < 3) {
                m_method = args[0];
                cout << "Method selected : " << m_method << endl;
            } else {
                cerr << "Method has to be 0, 1 or 2" << endl;
            }
            return {};
        }
    };
    
    message<> dspsetup { this, "dspsetup",
        MIN_FUNCTION {
            m_sr = args[0];
            m_vs = args[1];
            cout << "SampleRate: " << m_sr << " / Signal Vector Size: " << m_vs << endl;
            
            m_piOnSR = TWOPI/m_sr;
            return {};
        }
    };
    
    void operator()(audio_bundle input, audio_bundle output){
        auto out = output.samples(0);
        auto in = input.samples(0);
        double freq = (double)*input.samples(1);
        double rq = (double)*input.samples(2); // rq = bw / center freq
        
        double ffreq = freq * m_piOnSR;
        double bw = ffreq * rq;
        double R = 1 - bw * 0.5;
        double R2 = 2 * R;
        double RR = R * R;
        double a0, b1, b2, a2;
        a0 = (1.0 - RR) * 0.5;
        b1 = R2 * (R2 / (1 + RR)) * cos(ffreq);
        b2 = RR;
        a2 = R;
        
        for (auto i = 0; i < m_vs; i++) {
            double w = 0;
            
            switch (m_method) {
                case 0:
                    out[i] =
                    a0 * in[i] + a2 * m_inputDelay[1] +
                    b1 * m_outputDelay[0] - b2 * m_outputDelay[1];
                    
                    m_outputDelay[1] = m_outputDelay[0];
                    m_outputDelay[0] = out[i];
                    
                    m_inputDelay[1] = m_inputDelay[0];
                    m_inputDelay[0] = in[i];
                    break;
                    
                case 1:
                    w = a0 * in[i] + b1 * m_outputDelay[0] - b2 * m_outputDelay[1];
                    out[i] = (w - R * m_outputDelay[1]);
                    // a0 * in[i] + b1 * m_outputDelay[0] - b2 * m_outputDelay[1] - R * m_outputDelay[1];
                    
                    m_outputDelay[1] = m_outputDelay[0];
                    m_outputDelay[0] = w;
                    break;
                    
                case 2:
                    w = in[i] + b1 * m_outputDelay[0] - b2 * m_outputDelay[1];
                    out[i] = a0 * (w - m_outputDelay[1]);
                    // a0 * in[i] + a0 * b1 * m_outputDelay[0] - a0 * b2 * m_outputDelay[1] - a0 * m_outputDelay[1];
                    
                    m_outputDelay[1] = m_outputDelay[0];
                    m_outputDelay[0] = w;
                    break;
                    
                default:
                    break;
            }
            
            
            /*
            // Direct Form I
            {
                out[i] =
                a0 * in[i] +
                b1 * m_outputDelay[0] - b2 * m_outputDelay[1];
                
                m_outputDelay[1] = m_outputDelay[0];
                m_outputDelay[0] = out[i];
            }
            
            // Direct From II
            // The Audio Programming Book, p.487
            {
                double w = a0 * in[i] + b1 * m_outputDelay[0] - b2 * m_outputDelay[1];
                out[i] = (w - R * m_outputDelay[1]);
                // a0 * in[i] + b1 * m_outputDelay[0] - b2 * m_outputDelay[1] - R * m_outputDelay[1];
                
                m_outputDelay[1] = m_outputDelay[0];
                m_outputDelay[0] = w;
            }

            // Direct From II
            // SuperCollider Resonz code
            {
                double w = in[i] + b1 * m_outputDelay[0] - b2 * m_outputDelay[1];
                out[i] = a0 * (w - m_outputDelay[1]);
                // a0 * in[i] + a0 * b1 * m_outputDelay[0] - a0 * b2 * m_outputDelay[1] - a0 * m_outputDelay[1];
                
                m_outputDelay[1] = m_outputDelay[0];
                m_outputDelay[0] = w;
            }
            */
            
        }
        
    }
 
};


MIN_EXTERNAL(Resonz_tilde);
