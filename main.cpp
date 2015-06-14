//
//  main.cpp
//  Actor1
//
//  Created by zhaoAllen on 15/6/14.
//  Copyright (c) 2015年 zhaoAllen. All rights reserved.
//

#include <iostream>
#include "Actor.h"

class Log
{
public:
    void print_log(Buffer*& pb)
    {
        
    }
};
int main(int argc, const char * argv[]) {
    // insert code here...
    //std::cout << "Hello, World!\n";
    //Thread thread;
    Log log;
    Active* pa = Active::CreateActive(std::bind(&Log::print_log, log, std::placeholders::_1), 10, 10);
    pa->Start();
    pa->Stop();
    return 0;
}
