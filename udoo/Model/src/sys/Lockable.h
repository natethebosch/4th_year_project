//
//  Lockable.hpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-19.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef Lockable_hpp
#define Lockable_hpp

#include <stdio.h>
#include "Mutex.h"

template<typename T>
class Lockable {
    Mutex _mutex;
    T _value;
    
public:
    void set(T value){
        _mutex.lock();
        _value = value;
        _mutex.release();
    }
    
    T get(){
        T value;
        _mutex.lock();
        value = _value;
        _mutex.release();
        return value;
    }
};

#endif /* Lockable_hpp */
