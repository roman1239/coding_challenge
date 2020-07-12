#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <windows.h>

#include "lock_guard.h"

std::mutex m;
int resource = 0;

void test( std::string name, int num )
{
    for( int i = 0; i < num; i++ )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );

        my_namespace::lock_guard<std::mutex> lg1( m );
        resource++;
        std::cout << name << ":" << resource << std::endl;
    }
}

int main()
{
    SetConsoleCtrlHandler( NULL, false );

    std::thread t1( test, "thread1", 1000 );
    std::thread t2( test, "thread2", 1000 );

    t1.join();
    t2.join();

    return 0;
}