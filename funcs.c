#include "philo.h"

void    *safe_malloc(size_t bytes)
{
    void *ret;

    ret = malloc(bytes);
    if (ret == NULL)
        error_exit("Malloc error");
    return (ret);
}

static void handle_mutex_error(int status, t_opcode opcode)
{
    if (status == 0)
        return 0;
    if (status == EINVAL && (LOCK == opcode || UNLOCK == opcode))
        error_exit("The value specified by mutex is invalid.");
    else if (status == EINVAL && INIT == opcode)
        error_exit("The value specified by attr is invalid.");
    else if (status == EDEADLK)
        error_exit("A deadlock would occur if the thread blocked waiting for mutex");
    else if (status == EPERM)
        error_exit("The current thread does not hold a lock on mutex.");
    else if (status == ENOMEM)
        error_exit("The process cannot allocate enough memory to create another mutex.");
    else if (status == EBUSY)
        error_exit("Mutex is locked");
}
 void   safe_mutex_handle(t_mtx *mutex, t_opcode opcode)
 {
    if (opcode == LOCK)
        handle_mutex_error(pthread_mutex_lock(mutex), opcode);
    else if (opcode == UNLOCK)
        handle_mutex_error(pthread_mutex_unlock(mutex), opcode);
    else if (opcode == INIT)
        handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode);
    else if (opcode == DESTROY)
        handle_mutex_error(pthread_mutex_destroy(mutex), opcode);
    else
        error_exit("Wrong opcode for mutex");
 }

static void handle_thread_error(int status, t_opcode opcode)
{
    if (0 == status)
        return ;
    if (EAGAIN == status)
        error_exit("No resources to create another thread");
    else if (EPERM == status)
        error_exit("The caller does not have appropriate permission\n");
    else if (EINVAL == status && CREATE == opcode)
        error_exit("The value specified by attr is invalid.");
    else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
        error_exit("The value specified by thread is not joinable\n");
    else if (ESRCH == status)
        error_exit("No thread found corresponding to that thread ID, thread.");
    else if (EDEADLK == status)
        error_exit("A deadlock was detected or the value of "
            "thread specifies the calling thread.");
}

void safe_thread_handle(pthread_t *thread, void *(*foo)(void *), 
                        void *data, t_opcode opcode)
{
    if (CREATE == opcode)
        handle_thread_error(pthread_create(thread, NULL, foo, data), opcode);
    else if (JOIN == opcode)
        handle_thread_error(pthread_join(*thread, NULL), opcode);
    else if (DETACH == opcode)
        handle_thread_error(pthread_detach(*thread), opcode);
    else
        error_exit("Wrong opcode for thread handle: "
            "use <CREATE> <JOIN> <DETACH>\n");
}
