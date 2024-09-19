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
        return ;
}
 void   safe_mutex_handle(t_mtx *mutex, t_opcode opcode)
 {
    if (opcode == LOCK)
        pthread_mutex_lock(mutex);
    else if (opcode == UNLOCK)
        pthread_mutex_unlock(mutex);
    else if (opcode == INIT)
        pthread_mutex_init(mutex, NULL);
    else if (opcode == DESTROY)
        pthread_mutex_destroy(mutex);
    else
        error_exit("Wrong opcode for mutex");
 }