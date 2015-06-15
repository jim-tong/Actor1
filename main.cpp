//
//  main.cpp
//  Actor1
//
//  Created by zhaoAllen on 15/6/14.
//  Copyright (c) 2015年 zhaoAllen. All rights reserved.
//

#include <iostream>
#include "Actor.h"
#include <string>
#include "unistd.h"
#include "sys/types.h"

class Log
{
public:
    void print_log(Buffer*& pb)
    {
        
        std::string str(pb->GetCurData(), pb->GetDataLength());
        printf("threadid:%lu %s\n", pthread_self(), str.c_str());
    }
};
int main(int argc, const char * argv[]) {
    
    Log log;
    printf("maintheadid:%lu\n", pthread_self());
    Active* pa = Active::CreateActive(std::bind(&Log::print_log, log, std::placeholders::_1), 10, 1024);
    pa->Start();
    Buffer* pb = pa->GetBuffer();
    std::string test = "test";
    pb->append(test);
    pa->Send(pb);
    //pa->Stop();
    while (true) {
        sleep(1);
    }
    return 0;
}
