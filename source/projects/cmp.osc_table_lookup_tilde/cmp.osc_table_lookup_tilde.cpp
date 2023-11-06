/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
//#define TBL_LEN 8192
#define TBL_LEN 128

using namespace c74::min;


class osc_tablelookup_tilde : public object<osc_tablelookup_tilde>, public vector_operator<> {
private:
    double* m_lookuptable = NULL;
    double m_sr; // sample rate
    double m_vs; // signal vector size
    double m_idxincr = 0.0;
    double m_freq = 0.0;
    double m_curPhase = 0.0;
public:
    MIN_DESCRIPTION	{"Post to the Max Console."};
    MIN_TAGS		{"utilities"};
    MIN_AUTHOR		{"Cycling '74"};
    MIN_RELATED		{"print, jit.print, dict.print"};

    inlet<>  input	{ this, "(signal) freq" };
    outlet<> output	{ this, "(signal) output", "signal" };
    
    
    osc_tablelookup_tilde(const atoms& args = {}) {
        cout << "init" << endl;
        m_lookuptable = new double[TBL_LEN];
        
        for (size_t i = 0; i < TBL_LEN; i++) {
            m_lookuptable[i] = sin(TWOPI * (i / (double)TBL_LEN));
//            cout << m_lookuptable[i] << endl;
        }
    }
    
    ~osc_tablelookup_tilde() {
        if (m_lookuptable != NULL) {
            delete[] m_lookuptable;
        }
    }


    message<> dspsetup { this, "dspsetup",
        MIN_FUNCTION {
            m_sr = args[0];
            m_vs = args[1];
            cout << "SampleRate: " << m_sr << " / Signal Vector Size: " << m_vs << endl;
            return {};
        }
    };
    
    void operator()(audio_bundle input, audio_bundle output){
        auto out = output.samples(0);
        auto freq = (int)*input.samples(0);
        
        if (freq != m_freq) {
            m_freq = freq;
            m_idxincr = m_freq * (TBL_LEN / m_sr);
        }
        
        for (size_t i = 0; i < m_vs; i++) {
            
            int curIdx = (int)m_curPhase;
            int nextIdx = curIdx + 1;
            
            if (nextIdx >= TBL_LEN) nextIdx = nextIdx - TBL_LEN;
            
            double curVal = m_lookuptable[curIdx];
            double nextVal = m_lookuptable[nextIdx];
            
            double frac = m_curPhase - curIdx;
            
            out[i] = curVal + (nextVal - curVal) * frac;
            
            m_curPhase += m_idxincr;
            
            if (m_curPhase >= TBL_LEN) {
                m_curPhase -= TBL_LEN;
            }
            if (m_curPhase < 0) {
                m_curPhase += TBL_LEN;
            }
            
        }
        
    }

};


MIN_EXTERNAL(osc_tablelookup_tilde);
