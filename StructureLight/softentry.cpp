#include <Windows.h>
#include <iostream>
#include "softEntry.h"


#include <stdio.h>
#include <thread>
#include <mutex>
#include <Windows.h>

using namespace std;

class ThreadTest
{
public:
    //卖票线程1
    void ThreadTest::Thread1();
    //卖票线程2
    void ThreadTest::Thread2();


    ThreadTest();
    ~ThreadTest();

private:
    //票的剩余数目
    int Sum;

    mutex Mutex;//线程锁

};




void ThreadTest::Thread1(){

    for (;;){
        Mutex.lock();//加锁
        Sleep(10);
        --Sum;

        if (Sum < 0){
            printf("Thrad1——票卖完了\n", Sum);
            break;
        }
        printf("Thrad1——剩余票数：%d\n", Sum);
        Mutex.unlock();//解锁

    }

    Mutex.unlock();//解锁

}


void  ThreadTest::Thread2(){

    for (;;){

        Mutex.lock();//加锁
        Sleep(10);
        --Sum;

        if (Sum < 0){
            printf("Thrad2——票卖完了\n");
            break;
        }
        printf("Thrad2——剩余票数：%d\n", Sum);

        Mutex.unlock();//解锁
    }

    Mutex.unlock();//解锁


}

//构造函数
ThreadTest::ThreadTest()
{
    Sum = 50;
    thread t1(&ThreadTest::Thread1,this);
    t1.detach();
    thread t2(&ThreadTest::Thread2,this);
    t2.detach();


}
//析构函数
ThreadTest::~ThreadTest()
{

}








void realtine_soft_entry(bool loop)
{
    while(loop)
    {
         Sleep(30);
    }
}






void high_soft_entry(bool loop)
{
    my_main();
    while(loop)
    {
        std::cout<<"high_soft_entry"<<std::endl;
        Sleep(200);
    }
}

void low_soft_entry(bool loop)
{
    while(loop)
    {
        std::cout<<"low_soft_entry"<<std::endl;
        Sleep(1000);
    }
}

void my_main()
{


}

