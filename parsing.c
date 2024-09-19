#include "philo.h"

static inline bool  is_digit(char c)
{
    return (c >= '0' && c <='9');
}

static inline bool is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c
    == '\r' || c == '\v' || c == '\f');
}

static  const char  *valid_input(const char *str)
{
    int         len;
    const char  *number;

    len = 0;
    while (is_space(*str))
        str++;
    if(*str == '+')
        str++;
    else if (*str == '-')
        error_exit("only positive values allowed");
    if (!is_digit(*str))
        error_exit("Input not good");
    number = str;
    while (is_digit(*str++))
        len++;
    if (len > 10)
        error_exit("Input too big");
    return (number);
}

static  long ft_atol(const char *str)
{
    long    num;

    num = 0;
    str = valid_input(str);
    while (is_digit(*str))
        num = num * 10 + (*str++ - '0');
    if (num > INT_MAX)
        error_exit("Value too big");
    return (num);
}

void parse_input(t_table *table, char **av)
{
    table->philo_nbr = ft_atol(av[1]);
    table->time_to_die = ft_atol(av[2]) * 1000;
    table->time_to_eat = ft_atol(av[3]) * 1000;
    table->time_to_sleep = ft_atol(av[4]) * 1000;
    if(table->time_to_die < 60000
        || table->time_to_eat < 60000
        || table->time_to_sleep < 60000)
        error_exit("Time not good");
    if (av[5])
        table->nbr_limit_meals = ft_atol(av[5]);
    else
        table->nbr_limit_meals = -1;
}