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

#ifndef AOP_CPP11_WRAPPER_H
#define AOP_CPP11_WRAPPER_H

#include <iostream>
#include <functional>

namespace   aop{

using namespace std;

enum{
    WRAPPER_FUNCTION    = 0,
    WRAPPER_METHOD      = 1,
};

template<typename...T>
struct Wrapper{
};

template<typename T, typename Probe>
struct Wrapper<T*, Probe>{
    typedef function<void(Probe)>   WrapFuncType;
    T*                      _func;
    Probe                   _probe;
    WrapFuncType            _after,
                            _before;
    const static  int wrapper_category    = WRAPPER_FUNCTION;
    struct WrapInternal{
        Wrapper<T*, Probe>*  _wrapper;
        WrapInternal(Wrapper<T*,Probe>* w) : _wrapper(w){ 
            _wrapper->_before( _wrapper->_probe );
        }
        ~WrapInternal(){ 
            _wrapper->_after( _wrapper->_probe );
        }
    };
    Wrapper(T*              func, 
            Probe           p, 
            WrapFuncType    callBefore = [](Probe p){}, 
            WrapFuncType    callAfter  = [](Probe p){}) : _probe(p){
        _func   = func;
        _after  = callAfter;
        _before = callBefore;
    }
    template<typename ...Args>
    auto operator()(Args && ...args)
    ->decltype( (*_func)(args...) ){
        WrapInternal   _w(this);
        return  (*_func)(args...);
    }
};

template<typename T, typename Object, typename Probe>
struct Wrapper<T, Object, Probe>{
    typedef function<void(Probe)>   WrapFuncType;
    T               _method;
    Object          _obj;
    Probe           _probe;
    WrapFuncType    _after,
                    _before;
    const static  int wrapper_category    = WRAPPER_METHOD;
    struct WrapInternal{
        Wrapper<T, Object, Probe>*  _wrapper;
        WrapInternal(Wrapper<T, Object, Probe>* w) : _wrapper(w){ 
            _wrapper->_before( _wrapper->_probe );
        }
        ~WrapInternal(){ 
            _wrapper->_after( _wrapper->_probe );
        }
    };
    Wrapper(T&&             method, 
            Object&&        obj, 
            Probe           p,
            WrapFuncType    callBefore  = [](Probe p){},
            WrapFuncType    callAfter   = [](Probe p){} ) : _probe(p){
        _method = method;
        _obj    = obj;
        _after  = callAfter;
        _before = callBefore;
    }

    template<typename ...Args>
    auto operator()(Args && ...args)
    ->decltype(bind(_method, _obj, args...)(args...)) {
        WrapInternal   _w( this );
        auto    b = bind(_method, _obj, args...);
        return  b(args...);
    }
};

}   // namespace aop.

#endif  // AOP_CPP11_WRAPPER_H

