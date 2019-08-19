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
    //��Ʊ�߳�1
    void ThreadTest::Thread1();
    //��Ʊ�߳�2
    void ThreadTest::Thread2();


    ThreadTest();
    ~ThreadTest();

private:
    //Ʊ��ʣ����Ŀ
    int Sum;

    mutex Mutex;//�߳���

};




void ThreadTest::Thread1(){

    for (;;){
        Mutex.lock();//����
        Sleep(10);
        --Sum;

        if (Sum < 0){
            printf("Thrad1����Ʊ������\n", Sum);
            break;
        }
        printf("Thrad1����ʣ��Ʊ����%d\n", Sum);
        Mutex.unlock();//����

    }

    Mutex.unlock();//����

}


void  ThreadTest::Thread2(){

    for (;;){

        Mutex.lock();//����
        Sleep(10);
        --Sum;

        if (Sum < 0){
            printf("Thrad2����Ʊ������\n");
            break;
        }
        printf("Thrad2����ʣ��Ʊ����%d\n", Sum);

        Mutex.unlock();//����
    }

    Mutex.unlock();//����


}

//���캯��
ThreadTest::ThreadTest()
{
    Sum = 50;
    thread t1(&ThreadTest::Thread1,this);
    t1.detach();
    thread t2(&ThreadTest::Thread2,this);
    t2.detach();


}
//��������
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

