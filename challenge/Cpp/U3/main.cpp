#include <iostream>
#include <thread>
#include <windows.h>
#include <chrono>
#include <mutex>
#include <condition_variable>

std::condition_variable cv3_1;
std::condition_variable cv1_2;
std::condition_variable cv2_3;

bool thread1_ready = false;
bool thread2_ready = false;
bool thread3_ready = false;

std::mutex m;

void fcn1( std::string name )
{
        std::cout << name << ": starting, waiting" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

        while( 1 )
        {
            std::unique_lock<std::mutex> ul( m );
            cv3_1.wait( ul, []{ return thread1_ready; } );
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            std::cout << name << ": signal received, doing work" << std::endl;
            thread1_ready = false;
            thread2_ready = true;
            std::cout << name << ": done, notifying next thread" << std::endl;
            ul.unlock();
            cv1_2.notify_one();
        }
}

void fcn2( std::string name )
{
        std::cout << name << ": starting, waiting" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

        while( 1 )
        {
            std::unique_lock<std::mutex> ul( m );
            cv1_2.wait( ul, []{ return thread2_ready; } );
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            std::cout << name << ": signal received, doing work" << std::endl;
            thread2_ready = false;
            thread3_ready = true;
            std::cout << name << ": done, notifying next thread" << std::endl;
            ul.unlock();
            cv2_3.notify_one();
        }
}

void fcn3( std::string name )
{
        std::cout << name << ": starting, waiting" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

        while( 1 )
        {
            std::unique_lock<std::mutex> ul( m );
            cv2_3.wait( ul, []{ return thread3_ready; } );
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            std::cout << name << ": signal received, doing work" << std::endl;
            thread3_ready = false;
            thread1_ready = true;
            std::cout << name << ": done, notifying next thread" << std::endl;
            ul.unlock();
            cv3_1.notify_one();
        }
}

int main()
{
    SetConsoleCtrlHandler( NULL, false );

    std::cout << "main: starting all threads" << std::endl;

    std::thread t1( fcn1, "thread 1" );
    std::thread t2( fcn2, "thread 2" );
    std::thread t3( fcn3, "thread 3" );

    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    std::cout << "main: notifying thread 1" << std::endl;
    thread1_ready = true;
    cv3_1.notify_one();

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
