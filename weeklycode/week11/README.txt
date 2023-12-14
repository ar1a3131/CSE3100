/* Primitives
    * Core primitives
        * POSIX mutexes
        * POSIX conditions
            * Motivation: communication between threads
            * Examples in real-life: GUI refresh; Real-time data gathering and analysis

            * Typical scenario: 1 or more threads producing some data and one or more threads consuming the data
                * Thread 1 informs Thread 2 that something is "ready" for consumption

            * Example: Producer consumer with bounded buffer
                * Mutual exclusion - mutex
                * Synchronization - condition variable
                * Boolean predicate
                * Two scenarios - eager producer ; eager consumer

            * POSIX conditions APIs
                * int pthread_cond_init(pthread_cond_t * cond,const pthread_condattr_t * attr);
                * int pthread_cond_destroy(pthread_cond_t *cond);
                * int pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex); //best to have within while loop
                * int pthread_cond_signal(pthread_cond_t *cond); //signal does one
                * int pthread_cond_broadcast(pthread_cond_t *cond); //broadcast does all
                * int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime); //we won't use this


    * Non-core primitives - can be implemented using mutexes and conditions
        * Semphores
            * sema_xxx used to sync processes - available in UNIX systems.
            * Can be implemented using mutex/conditions.
            * What do we need?
                * A counter for the semaphore per se
                * A counter for the sleeping threads (trying to decrease the counter)
                * A POSIX condition to wait when counter is 0
                * A POSIX mutex to protect the state

        * Read-write locks
            * Multiple read operations can take place at the same time
            * A write operation is not compatible with any other operation
            * Aim: achieve fairness and correctness
            * Implementation using mutexes and conditions are not straight forward

        * Barriers
            * Have worker threads wait for their group to be fully done before proceeding
            * Number of workers known apriori
            * APIs needed - create a barrier of size k, destroy a barrier, wait for the worker crew to all enter the barrier