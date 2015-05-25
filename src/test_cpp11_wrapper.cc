/**
 *  function/method call wrapper with c++11, supporting return value.
 *  
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2015 peixu.zhu
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 * */

#include    "cpp11_wrapper.h"

using namespace std;
using namespace aop;

struct  Foo{
    Foo(){ cout << "Foo::Foo" << endl; }
    void bar(int v){ cout << "Foo::bar " << v << endl; }
};

int foo_int(int a) { cout << "foo_int: "  << a << endl; return a * a; }


int main(){
    typedef     Wrapper<int(*)(int), int>   T1;
    T1          w4(&foo_int, 100, 
                [](int v){ cout << "w4<100> before" << endl; },
                [](int v){ cout << "w4<100> after" << endl; });
    cout << w4(400) << endl;
    typedef     Wrapper<void(Foo::*)(int), Foo*, const char*> T2;
    Foo         f;
    T2          w5(&Foo::bar, &f, "method",
                [](const char* v){ cout << "w5<method> before" << endl;},
                [](const char* v){ cout << "w5<method> after" << endl;});
    w5(500);
    Wrapper<T1*, int>  w6(&w4, 200,
                       [](int v){ cout << "w6<200>before" << endl;},
                       [](int v){ cout << "w6<200>after" << endl;});
    Wrapper<T2*, int>  w7(&w5, 300,
                       [](int v){ cout << "w7<300> before" << endl;},
                       [](int v){ cout << "w7<300> after" << endl;} );
    cout << w6(600) << endl;
    w7(700);
}

