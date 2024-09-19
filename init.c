#include "philo.h"

void    data_init(t_table *table)
{
    table->end_simulation = false;
    table->philos = safe_malloc(table->philo_nbr);
}