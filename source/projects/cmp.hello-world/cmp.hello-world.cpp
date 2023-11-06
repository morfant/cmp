/* 코드에 관한 전체 설명 */
/// @file
///	@ingroup    cmp
///	@copyright	Copyright 2023 giy.hands@gmail.com All rights reserved.
///	@license	         Use of this source code is governed by the MIT License found in the License.md file.

/* 헤더파일, define, 전역 변수 등 */
#include "c74_min.h"

using namespace c74::min;


/* 클래스 정의 */
class hello_world : public object<hello_world> { // 원하는 클래스 이름으로 변경 필요
public:
    /* 오브젝트에 관한 설명 */
    MIN_DESCRIPTION	{"Post to the Max Console."}; // 간단한 기능 설명
    MIN_TAGS		{"utilities"}; // 오브젝트에 대한 tag
    MIN_AUTHOR		{"Cycling '74"}; // 저자에 대한 설명
    MIN_RELATED		{"print, jit.print, dict.print"}; // 관련 Max/MSP 오브젝트들 설명

    /* 인렛과 아웃렛 설정 */
    /* 인렛, 아웃렛에 마우스를 올렸을 때 팝업되는 내용 */
    inlet<>  input	{ this, "(bang) post greeting to the max console" };
    outlet<> output	{ this, "(anything) output the message which is posted to the max console" };


    /* 생성자 (Constructor) */
    // 오브젝트에 직접 입력하는 아규먼트들이 args 라는 배열로 들어온다
    hello_world(const atoms& args = {})
    {
        if (args.size() > 0)
        {
            for (auto e : args)
            {
                cout << e << endl;
            }
        }
    }
    
    
    /* 오브젝트에 직접 넣어주는 아규먼트들 관련 정의 */
    /* 아규먼트를 변경할 때마다 실행 됨 */
    /* 그러나 생성자 함수가 있으면 실행이 되지 않는 듯 */
    // define an optional argument for setting the message
    argument<symbol> greeting_arg { this, "greeting", "Initial value for the greeting attribute.",
        MIN_ARGUMENT_FUNCTION {
            cout << "arg: " << arg << endl;
            greeting2 = arg;
        }
    };
    
    

    /* 오브젝트가 가지게 될 attribute(Max가 사용가능한 일종의 변수) 관련 정의 */
    // the actual attribute for the message
    attribute<symbol> greeting2 {
        this, "greeting", "hello world!!!", /* this, Max에서 보일 attribute 이름, 기본값 */
        description {// inspector 창에서 해당 attribute의 정보를 확인 할 때 팝업되는 설명
            "Greeting to be posted. "
            "The greeting will be posted to the Max console when a bang is received."
        }
    };
   
    
    /* 메시지에 반응하는 함수들을 정의하는 부분 */
    // respond to the bang message to do something
    message<> bang { this, "bang", "Post the greeting.",
        MIN_FUNCTION {
            symbol the_greeting = greeting2;    // fetch the symbol itself from the attribute named greeting

            cout << the_greeting << endl;    // post to the max console
            output.send(the_greeting);       // send out our outlet
            return {};
        }
    };


    /* 오브젝트가 최초 로드 될 때 한 번 실행되는 함수 */
    // post to max window == but only when the class is loaded the first time
    message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "hello world" << endl;
            return {};
        }
    };

};


MIN_EXTERNAL(hello_world);
