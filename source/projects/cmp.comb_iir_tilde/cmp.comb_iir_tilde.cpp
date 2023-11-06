/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"

using namespace c74::min;


class comb_iir_tilde : public object<comb_iir_tilde>, public vector_operator<> {
private:
    int DEL_LEN = 0;
    double* m_delayline = NULL;
    double m_sr;
    int m_vs;
    int m_wp = 0;
    double m_maxDelayTime = 1.0;
    
public:
    MIN_DESCRIPTION	{"Post to the Max Console."};
    MIN_TAGS		{"utilities"};
    MIN_AUTHOR		{"Cycling '74"};
    MIN_RELATED		{"print, jit.print, dict.print"};

    inlet<>  input1	{ this, "(signal) input source" };
    inlet<>  input2 { this, "(signal) delay time(sec)" };
    inlet<>  input3 { this, "(signal) fdb coef" };
    outlet<> output	{ this, "(signal) output", "signal" };
    
    comb_iir_tilde(const atoms& args = {}) {
        if (args.size() > 0) {
            m_maxDelayTime = args[0];
        }
    }
    
    ~comb_iir_tilde() {
        if (m_delayline != NULL) {
            delete[] m_delayline;
        }
    }

    message<> dspsetup { this, "dspsetup",
        MIN_FUNCTION {
            m_sr = args[0];
            m_vs = args[1];
            cout << "SampleRate: " << m_sr << " / Signal Vector Size: " << m_vs << endl;
            
            if (m_delayline != NULL) {
                delete [] m_delayline;
            }
            
            DEL_LEN = (int)m_sr * m_maxDelayTime;
            
            m_delayline = new double[DEL_LEN]{};
//            memset(m_delayline, 0, sizeof(double) * DEL_LEN);
            
            for (size_t i = 0; i < DEL_LEN - 1; i++) {
                cout << m_delayline[i] << endl;
            }
            cout << "Delayline prepared: len of " << sizeof(double) * DEL_LEN << endl;

            return {};
        }
    };
    
    
    void operator()(audio_bundle input, audio_bundle output){
        auto in = input.samples(0);
        auto out = output.samples(0);
        double fdb = (double)*input.samples(2);
        double dt = (double)*input.samples(1);
        if (dt >= m_maxDelayTime) {
            cerr << "delaytime(" << dt << ") must be set shorter than maxdelaytime(" << m_maxDelayTime << ")" << endl;
            dt = MIN(m_maxDelayTime, dt);
        }
        int k = MIN(DEL_LEN - 1, (int)(m_sr * dt)); // k sample
        
//        if (m_rp >= DEL_LEN) m_rp -= DEL_LEN;
//        if (m_rp < 0) m_rp += DEL_LEN;
//        cout << "k: " << k << " / m_rp: " << m_rp << endl;
        
        for (size_t i = 0; i < m_vs; i++) {
            int rp = m_wp - k;
            if (rp >= DEL_LEN) rp -= DEL_LEN;
            else if (rp < 0) rp += DEL_LEN;
            out[i] = in[i] + fdb * m_delayline[rp];
            
            m_delayline[m_wp] = out[i];
            m_wp = m_wp == DEL_LEN - 1 ? 0 : m_wp + 1;
        }
    }

};


MIN_EXTERNAL(comb_iir_tilde);
