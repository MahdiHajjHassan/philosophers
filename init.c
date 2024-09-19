#include "philo.h"

void    data_init(t_table *table)
{
    int i;

    i = -1;
    table->end_simulation = false;
    table->philos = safe_malloc(sizeof(t_philo) * table->philo_nbr);
    table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
    while (i++ < table->philo_nbr)
    {

    }
}