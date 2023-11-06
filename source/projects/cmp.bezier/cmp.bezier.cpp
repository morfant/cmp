/// @file
///	@ingroup     bezier
///	@copyright   Copyright 2023 giy.hands@gmail.com. All rights reserved.
///	@license            Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include "Vector2.h"

using namespace c74::min;


class bezier : public object<bezier> {
private:
    int points[6];
    float resolution = 4800.0f;
    float rad = 0.5;
    
public:
    MIN_DESCRIPTION	{"Implement Bezier curve."};
    MIN_TAGS		{"utilities"};
    MIN_AUTHOR		{"giy.hands@gmail.com"};
    MIN_RELATED		{"print, jit.print, dict.print"};
    
    inlet<>  input_0	{ this, "start point (x)" };
    inlet<>  input_1    { this, "start point (y)" };
    inlet<>  input_2    { this, "control point (x)" };
    inlet<>  input_3    { this, "control point (y)" };
    inlet<>  input_4    { this, "end point (x)" };
    inlet<>  input_5    { this, "end point (y)" };
    outlet<> output_0 	{ this, "(anything) init path of bezier curve" };
    outlet<> output_1   { this, "(anything) sequential paths of bezier curve" };
    
    
    
    bezier(const atoms& args = {}) {
        cout << args.size() << endl;
        if (args.size() != 6) {
            cerr << "6 arguments are required!" << endl;
        } else {
            for (int i = 0; i < args.size(); i++) {
                points[i] = args[i];
                cout << points[i] << endl;
            }
            
            draw();
        }
    }
    
    
    
        
    // respond to the bang message to do something
    message<> bang { this, "bang",
        MIN_FUNCTION {
            
            for (int i = 0; i < args.size(); i++) {
                points[i] = args[i];
                cout << points[i] << endl;
            }
            
//            output_0.send(points[0], points[1]);
            
            return {};
        }
    };
    
    
    // post to max window == but only when the class is loaded the first time
    message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "hello world" << endl;
            return {};
        }
    };
    
    
    message<> handle_list {this, "list",
        MIN_FUNCTION {
//            cout << "list: " <<
//            args[0] << ", " << args[1] << ", " << args[2] << ", " <<
//            args[3] << ", " << args[4] << ", " << args[5] << endl;
            
            for (int i = 0; i < 6; i++) {
                points[i] = args[i];
            }
            
            draw();
            
            return {};
        }
    };
    
    message<> handle_float { this, "float",
        MIN_FUNCTION {
            switch(inlet) {
                case 0:
                    cout << "inlet 0: " << args[0] << endl;
                    break;
                    
                case 1:
                    cout << "inlet 1: " << args[0] << endl;
                    break;
                    
                case 2:
                    cout << "inlet 2: " << args[0] << endl;
                    break;
                    
                case 3:
                    cout << "inlet 3: " << args[0] << endl;
                    break;
                    
                case 4:
                    cout << "inlet 4: " << args[0] << endl;
                    break;
                    
                case 5:
                    cout << "inlet 5: " << args[0] << endl;
                    break;
                
                default:
                    break;
            }
            
            return {};
        }
    };
    
    message<> draw { this, "draw",
        MIN_FUNCTION {
            
            float amt = 0.f;
            
            
            for (int i = 0; i < resolution; i++) {
                amt = i / resolution; // int / float => float
//                cout << "amt: " << amt << endl;
                
                Vector2 p0 = Vector2(points[0], points[1]);
                Vector2 p1 = Vector2(points[2], points[3]);
                Vector2 p2 = Vector2(points[4], points[5]);

                Vector2 q0 = p0 + Vector2::scalarMultiplication(amt, (p1 - p0));
                Vector2 q1 = p1 + Vector2::scalarMultiplication(amt, (p2 - p1));
                
                Vector2 r = q0 + Vector2::scalarMultiplication(amt, (q1 - q0));
                
                output_1.send(
                              r.getX() - rad, r.getY() - rad,
                              r.getX() + rad, r.getY() + rad
                              );
                
            }
            
            return {};
        }
    };
    
};


MIN_EXTERNAL(bezier);
