#include <iostream>
#include <thread>
#include <windows.h>
#include <chrono>

void fcn( std::string name )
{
    while ( 1 )
    {
        std::cout << name << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }
}

int main()
{
    SetConsoleCtrlHandler( NULL, false );

    std::thread t1( fcn, "thread 1" );
    std::thread t2( fcn, "thread 2" );
    std::thread t3( fcn, "thread 3" );

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
