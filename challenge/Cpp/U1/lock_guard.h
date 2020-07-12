#ifndef LOCK_GUARD_H
#define LOCK_GUARD_H

#include <mutex>

namespace my_namespace
{
    template <class T>
    class lock_guard
    {
        private:
            T* p_mutex;

        public:
            lock_guard( T& t_mutex )
            {
                p_mutex = &t_mutex;
                t_mutex.lock();
            }

            ~lock_guard()
            {
                p_mutex->unlock();
            }

            lock_guard operator=( lock_guard ) = delete;
    };
}
#endif // LOCK_GUARD_H
