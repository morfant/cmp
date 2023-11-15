/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include "Vector2.h"
#include <algorithm>

using namespace c74::min;


class bezier_tilde : public object<bezier_tilde>, public vector_operator<> {
private:
    int points[8];
    float rad = 0.5;
    vector<float> curveY, curveX;
    float min_value, max_value, diff;
    int count = 0;
    int prevXIdx = -1;
    double prevY = 0;
    
    int sampleRate, vectorSize = 64;
    

public:
    MIN_DESCRIPTION	{ "Bezier what??" };
    MIN_TAGS		{ "audio, oscillator" };
    MIN_AUTHOR		{ "giy.hands@gmail.com" };
    MIN_RELATED		{ "bezier_tilde~, saw~" };

    inlet<>  in1    {this, "() input"};
    outlet<> out1   {this, "(signal) ramp wave", "signal"};
    outlet<> out2   {this, "(anything) sequential paths of bezier curve" };

    bezier_tilde(const atoms& args = {}) {
        cout << args.size() << endl;
        if (args.size() != 8) {
            cerr << "6 arguments are required!" << endl;
        } else {
            for (int i = 0; i < args.size(); i++) {
                points[i] = args[i];
                cout << points[i] << endl;
            }
        }
    }
    
    
    void negOneToPosOne(float f)
    {
        -1 + ((f - min_value) / diff) * 2;
    }
    
    argument<number> steps_arg { this, "steps", "Initial steps.",
        MIN_ARGUMENT_FUNCTION {
            steps_attr = arg;
        }
    };
    
    attribute<number> steps_attr { this, "steps", 1024,
        description {"number of points on Bezier curve."},
    };
    
    
    message<> dspsetup {this, "dspsetup",
        MIN_FUNCTION {
            sampleRate = args[0];
            vectorSize = args[1];
            
            cout << "SampleRate: " << sampleRate << ", vectorSize: " << vectorSize << endl;
            return {};
        }
        
    };
    
    message<> hadle_list {this, "list",
        MIN_FUNCTION{
            points[0] = args[0];
            points[1] = args[1];
            points[2] = args[2];
            points[3] = args[3];
            points[4] = args[4];
            points[5] = args[5];
            points[6] = args[6];
            points[7] = args[7];
            
            return {};
        }
    };
    
    message<> bang {this, "bang",
        MIN_FUNCTION {
             
            for (int i = 0; i < curveY.size(); i++)
            {
                cout << "i / x / y: " << i << " / " << curveX.at(i) <<  " / " << curveY.at(i) << endl;
            }
            
            return {};
        }
    };
   
    message<> calcBezier {this, "calc",
        MIN_FUNCTION {
            curveY.clear();
            curveX.clear();
            
            float amt = 0.f;
                     
            for (int i = 0; i < steps_attr; i++) {
                amt = i / steps_attr; // int / float => float
                
                Vector2 p0 = Vector2(points[0], points[1]);
                Vector2 p1 = Vector2(points[2], points[3]);
                Vector2 p2 = Vector2(points[4], points[5]);
                Vector2 p3 = Vector2(points[6], points[7]);
                
                Vector2 q0 = p0 + Vector2::scalarMultiplication(amt, (p1 - p0));
                Vector2 q1 = p1 + Vector2::scalarMultiplication(amt, (p2 - p1));
                Vector2 q2 = p2 + Vector2::scalarMultiplication(amt, (p3 - p2));
                
                Vector2 r0 = q0 + Vector2::scalarMultiplication(amt, (q1 - q0));
                Vector2 r1 = q1 + Vector2::scalarMultiplication(amt, (q2 - q1));
                
                Vector2 s = r0 + Vector2::scalarMultiplication(amt, (r1 - r0));
                
                
                curveY.push_back(s.getY());
                curveX.push_back(s.getX());
                
                out2.send(
                              s.getX() - rad, (200 - s.getY()) - rad,
                              s.getX() + rad, (200 - s.getY()) + rad
                              );
            }
            
            // 최소값과 최대값 찾기
            min_value = *std::min_element(curveY.begin(), curveY.end());
            max_value = *std::max_element(curveY.begin(), curveY.end());
//            cout << min_value << " / " << max_value << endl;
            
            float diff = max_value - min_value;
            diff = 200;
            
            for (float& f : curveY)
            {
//                f = ((f - min_value) / diff) * 2 - 1; // -1.0 ~ 1.0
                f = (f / diff) * 2 - 1; // -1.0 ~ 1.0
            }
            
            for (float& f : curveX)
            {
                f = floor(steps_attr * f / 200);
//                cout << "curveX: " << f << endl;
                if (f >= vectorSize || f < 0)
                {
                    cerr << "value of curveX must be from 0 to vectorSize!!(f=" << f << ")" << endl;
                }
            }
            
            cout << "length of curveX / curveY: " << curveX.size() << " / " << curveX.size() << endl;
           
           
            
   
            return {};
        }
    };
    
//    sample operator()(sample x) {
//        count %= (int)steps_attr;
//        if (prevXIdx != curveX[count]) {
//            prevY = curveY[count];
//
//        }
//        count++;
//        return prevY;
//
//    }
    
    
    
    void operator()(audio_bundle input, audio_bundle output) {
        //        auto in = input.samples(0);    // get vector for channel 0 (first channel)
        auto out = output.samples(0);  // get vector for channel 0 (first channel)
        
        output.clear();
        
        for (int i = 0; i < vectorSize; i++)
        {
            out[size_t(curveX[i])] = curveY[i];
            if (i != curveX[i] && i > 0)
            {
                int diff = i - curveX[i];
                for (int d = 1; d < diff; d++)
                {
                    out[i - d] = curveY[i - 1];
                }
            }
            
            //            out[i] = -1.0 + ((float)rand() / (float)(RAND_MAX/2)); // white noise
        }
        
        
        out[vectorSize - 1] = curveY[steps_attr - 1];
    };
};

MIN_EXTERNAL(bezier_tilde);
