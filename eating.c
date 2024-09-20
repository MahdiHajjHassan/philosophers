#include "philo.h"

void    *eat_simulation(void *data)
{
    t_philo *philo;

    philo = (t_philo *)data;
    wait_all_threads(philo->table);
}

void    eating_start(t_table *table)
{
    int     i;

    i = -1;
    if (table->nbr_limit_meals)
        return ;
    else if (table->philo_nbr == 1)
        ;//
    else
    {
        while (++i < table->philo_nbr)
            safe_thread_handle(&table->philos[i].thread_id, eat_simulation,
                                &table->philos[i], CREATE);
    }
}