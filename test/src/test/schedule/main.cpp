

#include <hscheduler.h>
#include <iostream>
#include <unistd.h>

using namespace HUICPP;

HSingleScheduler HINIT_PRIORITY_LOW schedule;

void fun1();
void fun0() {

    std::cout << "this is fun0" << std::endl;

    schedule.ScheduleDelayTask(3000000, fun1);

}

void fun1() {

    std::cout << "this is fun1" << std::endl;

    schedule.ScheduleDelayTask(1000000, fun0);

}


void fun2() {

    std::cout << "this is fun2" << std::endl;

    schedule.ScheduleDelayTask(1000000, fun2);

}

void fun3() {

    std::cout << "this is fun3" << std::endl;

}


int main(int argc, const char* argv[]) {

    schedule.ScheduleDelayTask(5000000, fun0);

    schedule.ScheduleDelayTask(1000000, fun2);

    schedule.CreateTrigger(fun3);

    schedule.DoEventLoop();
    
    std::cout << "scheduler test OK" << std::endl;

    return 0;
}


