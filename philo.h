#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>

typedef pthread_mutex_t t_mtx;

typedef struct s_fork
{
    t_mtx   fork;
    int     fork_id;
}                   t_fork;

typedef struct s_philo
{
    int         id;
    long        meals_counter;
    bool        full;
    long        last_meal_time;
    t_fork      *left_fork;
    t_fork      *right_fork;
    pthread_t   thread_id;
}               t_philo;

typedef struct s_table
{
    long    philo_nbr;
    long    time_to_die;
    long    time_to_eat;
    long    time_to_sleep;
    long    
    long    
}