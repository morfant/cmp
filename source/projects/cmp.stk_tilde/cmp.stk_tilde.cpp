/// @file
///	@ingroup 	cmp
///	@copyright	Copyright 2023 giy.hands@gmail.com All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include "lib/Noise.h"
#include "lib/SineWave.h"
#include "lib/FreeVerb.h"

using namespace c74::min;

class stk_tilde : public object<stk_tilde>, public sample_operator<0, 2> {
private:
    stk::Noise noise;
    stk::SineWave sin;
    stk::FreeVerb fvb;
    
    lib::adsr env;
    
    int m_sr = 48000;
    
public:
    MIN_DESCRIPTION	{ "STK library Test" };
    MIN_TAGS		{ "audio, STK, test" };
    MIN_AUTHOR		{ "giy.hands@gmail.com" };
    MIN_RELATED		{ "stk_tilde~" };

    inlet<>  in1 {this, "(list) parameters"};
    outlet<> out1 {this, "white noise out", "signal"};
    outlet<> out2 {this, "sine wave out", "signal"};
    
    stk_tilde()
    {
        
        env.mode(lib::adsr::envelope_mode::adsr);
        env.initial(0.0);
        env.peak(1.0);
        env.sustain(0.8);
        env.end(0.0);
        
        env.attack(100, m_sr);
        env.decay(100, m_sr);
        env.release(2000, m_sr);
        env.active();
    };
     
    
    message<> dspsetup {this, "dspsetup",
        MIN_FUNCTION {
            m_sr = args[0];
            return {};
        }
    };
    
    
    message<> bang {this, "bang",
        MIN_FUNCTION {
            
            /*
            // stages of env
             
            inactive,
            attack,
            decay,
            sustain,
            release,
            retrigger,      // start a new envelope while the current one is still active
            early_release,  // release before the sustain has been reached
            */
 
            cout << "env stage: " << env.stage() << endl;
            if (env.stage() == c74::min::lib::adsr::adsr_stage::sustain) {
                cout << "env trigger(false)" << endl;
                env.trigger(false);
            } else {
                cout << "env trigger(true)" << endl;
                env.trigger(true);
            }
           
            return {};
        }
    };
    
    message<> setParams {this, "list", "Set parameters",
        MIN_FUNCTION {
            cout << args[0] << " / " << args[1] << " / " << args[2] << " / " << args[3] << " / " <<args[4] << " / " <<args[5] << endl;
            
            sin.setFrequency(args[0]);
            fvb.setEffectMix(args[1]); // dry(0.0 ~ wet(1.0)
             
            env.attack(args[2], m_sr);
            env.decay(args[3], m_sr);
            env.sustain(args[4]);
            env.release(args[5], m_sr);
            
            return {};
        }
    };
    
    
    samples<2> operator()() {
        
        float noiseSample =  (float)noise.tick();
        float sinSample = (float)sin.tick();
        
        return {fvb.tick(env() * noiseSample), env() * sinSample};
    }
    
};

MIN_EXTERNAL(stk_tilde);



