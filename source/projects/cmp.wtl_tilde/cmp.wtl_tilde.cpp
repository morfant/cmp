/// @file
///	@ingroup    cmp package
///	@copyright	Copyright 2022 giy (giy.hands@gmail.com). All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#define TABLEN 1024

using namespace c74::min;


class wtl_tilde : public object<wtl_tilde>, public vector_operator<> {
private:
    size_t m_sr; // sample rate
    size_t m_vs; // signal vector size
    double m_curPhase = 0.0;
    double m_incr = 0.0;
    double m_table[TABLEN] = {};
    
public:
    MIN_DESCRIPTION	{"Basic Wave Table Lookup oscillator."};
    MIN_TAGS		{"oscillator"};
    MIN_AUTHOR		{"Gang il, Yi"};
    MIN_RELATED		{"Osc"};

    inlet<>  input	{ this, "(signal) freq" };
    outlet<> output	{ this, "(signal) output", "signal" };
    
    // 생성자
    wtl_tilde(const atoms& args = {}) {
        double step = TWOPI / (double)TABLEN;
        
        for (size_t i = 0; i < TABLEN; i++) {
            m_table[i] = sin(step * i);
        }
    }
    
    // 파괴자
    ~wtl_tilde(){
        
    }

    message<> dspsetup { this, "dsp_setup",
        MIN_FUNCTION {
            m_sr = args[0];
            m_vs = args[1];
            return {};
        }
    };
    
    
    void operator()(audio_bundle input, audio_bundle output){
        auto out = output.samples(0);
        auto freq = (double)*input.samples(0);
        m_incr = freq * TABLEN / m_sr;
        
        for (size_t i = 0; i < m_vs; i++) {
           
            int index = (int)m_curPhase;
            double frac = m_curPhase - index;
            int nextIndex = index < (TABLEN - 1) ? index + 1 : index - (TABLEN - 1);
            double slope = m_table[nextIndex] - m_table[index];
            out[i] = m_table[index] + (slope * frac);
            m_curPhase += m_incr;
            
            if (m_curPhase >= TABLEN) m_curPhase -= TABLEN;
            else if (m_curPhase < 0) m_curPhase += TABLEN;
            
        }
    }

};


MIN_EXTERNAL(wtl_tilde);
