/// @file
///	@ingroup    cmp4
///	@copyright	Copyright 2022 Gangil Yi(giy.hands@gmail.com). All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"

using namespace c74::min;


class iir_comb_tilde : public object<iir_comb_tilde>, public vector_operator<> {
private:
    size_t  m_sr;
    size_t  m_vs;
    double m_fdb = 0.5;
    double m_maxDelayTime = 1.0;
    double * m_delayLine = NULL;
    
    int m_wp = 0;
    int m_rp = 0;
    
    int DEL_LEN = 0;
    
    
public:
    MIN_DESCRIPTION	{" "};
    MIN_TAGS		{"utilities"};
    MIN_AUTHOR		{"Gangil Yi"};
    MIN_RELATED		{" "};

    inlet<>  input1	{ this, "(signal) input", "signal" };
    inlet<>  input2 { this, "(signal) delay time", "signal"};
    inlet<>  input3 { this, "(float) fdb coef", "float" };
    outlet<> output { this, "(signal) output", "signal" };

    // 생성자
    iir_comb_tilde(const atoms& args = {}){
        if (args.size() > 0) m_maxDelayTime = args[0];
//        if (args.size() > 1) m_fdb = args[1];
    }
    
    // 파괴자
    ~iir_comb_tilde(){
        if (m_delayLine != NULL) delete [] m_delayLine;
    }
    
    message<> handle_float{ this, "float",
        MIN_FUNCTION {
            m_fdb = args[0];
            cout << "m_fdb: " << m_fdb << endl;
            return {};
        }
    };
 
    message<> dspsetup{ this, "dsp_setup",
        MIN_FUNCTION {
            m_sr = args[0]; // sample rate
            m_vs = args[1]; // signal vector size
            cout << "Sample Rate: " << m_sr << " / Signal Vector Size: " << m_vs << endl;
            
            
            if (m_delayLine != NULL) {
                delete [] m_delayLine;
            }
            
            // 1
//            m_delayLine = new double[m_maxDelayTime * m_sr];
//            memset(m_delayLine, 0, sizeof(double) * m_maxDelayTime * m_sr);
            
            // 2
            DEL_LEN = m_maxDelayTime * m_sr;
            m_delayLine = new double[DEL_LEN]{};
            
            return {};
        }
    };
    
    void operator()(audio_bundle input, audio_bundle output){
        
        auto in = input.samples(0);
        auto out = output.samples(0);
        
        auto delayTime = (double)*input.samples(1);
        if (delayTime > m_maxDelayTime) {
            cerr << "delayTime(" << delayTime << ")은 maxDelayTime("
            << m_maxDelayTime << ") 보다 작아야 합니다" << endl;
        }
        delayTime = MIN(delayTime, m_maxDelayTime);
        
        auto k = delayTime * m_sr; // 1초 = sr, 0.1초 = sr * 0.1
        
        for(auto i = 0; i < m_vs; i++) {
            m_rp = m_wp - k;
            if (m_rp < 0) m_rp = m_rp + (DEL_LEN - 1);
            
            out[i] = in[i] + m_fdb * m_delayLine[m_rp];
            
            m_wp++;
            if (m_wp > DEL_LEN - 1) m_wp = m_wp - DEL_LEN;
            m_delayLine[m_wp] = out[i];
        }
    }
};


MIN_EXTERNAL(iir_comb_tilde);
