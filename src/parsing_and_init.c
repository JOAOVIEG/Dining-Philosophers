/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_and_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaocard <joaocard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 11:12:38 by joaocard          #+#    #+#             */
/*   Updated: 2024/05/26 14:06:21 by joaocard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *av, int *error)
{
	int		sign;
	int		integer;

	sign = 1;
	integer = 0;
	if (*(char *)av == '-' || *(char *)av == '+')
	{
		if (*(char *)av == '-')
			sign = -1;
		av++;
	}
	if (non_digit(av, error) != 0)
		return (1);
	while (*(char *)av && *(char *)av >= '0' && *(char *)av <= '9')
	{
		if (overflow(integer, av, error) != 0)
			return (1);
		integer = (integer * 10) + *(char *)av - '0';
		av++;
		if (non_digit(av, error) != 0)
			return (1);
	}
	return (sign * integer);
}

int	parse_args(t_table *table, char **av)
{
	int	error;

	error = 0;
	table->nbr_philos = ft_atoi(av[1], &error);
	if (error != 0)
		return (1);
	if ((int)table->nbr_philos <= 0 || (int)table->nbr_philos > 200)
		return (printf("Set at least 1 philo and less than 200\n"));
	table->time_to_die = ft_atoi(av[2], &error) * 1e3;
	table->time_to_eat = ft_atoi(av[3], &error) * 1e3;
	table->time_to_sleep = ft_atoi(av[4], &error) * 1e3;
	if (table->time_to_die < 60e3 || table->time_to_eat < 60e3 \
							||table->time_to_sleep < 60e3)
		return (printf("Wrong input: time has to more than 60 ms\n"));
	if (av[5])
	{
		table->nb_times_must_eat = ft_atoi(av[5], &error);
		if (error != 0)
			return (error);
		if ((int)table->nb_times_must_eat <= 0)
			return (printf("Wrong input: Set a positive for nr meals\n"));
	}
	else
		table->nb_times_must_eat = -1;
	return (0);
}

int	philo_init(t_table *table)
{
	t_philo	*philo;
	size_t	pos;

	pos = 0;
	while (pos < table->nbr_philos)
	{
		philo = &table->philos[pos];
		philo->id = pos + 1;
		philo->meals_count = 0;
		philo->full = false;
		if (pthread_mutex_init(&philo->philo_lock, NULL) != 0)
			return (printf("ERROR initializing philo_lock"));
		philo->table = table;
		give_forks(philo, table->forks, pos);
		pos++;
	}
	return (0);
}

void	give_forks(t_philo *philo, t_fork *forks, int pos)
{
	size_t	nbr_forks;

	nbr_forks = philo->table->nbr_philos;
	if (philo->id % 2 == 0)
	{
		philo->prio_fork = &forks[pos];
		philo->sec_fork = &forks[(pos + 1) % nbr_forks];
	}
	else if (philo->id % 2)
	{
		philo->prio_fork = &forks[(pos + 1) % nbr_forks];
		philo->sec_fork = &forks[pos];
	}
}

int	init_table(t_table	*table)
{
	size_t	i;

	i = 0;
	table->nbr_threads_running = 0;
	table->dinner_end = false;
	table->dinner_is_synchro = false;
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->nbr_philos);
	if (table->philos == NULL)
		return (printf("Error initializing with malloc, philos array"));
	table->forks = (t_fork *)malloc(sizeof(t_fork) * table->nbr_philos);
	if (table->forks == NULL)
		return (printf("Error initializing with malloc forks array"));
	while (i < table->nbr_philos)
	{
		if (pthread_mutex_init(&table->forks[i].lock, NULL) != 0)
			return (printf("Error initializing with mutex, forks lock array"));
		i++;
	}
	if (pthread_mutex_init(&table->dinner_lock, NULL) != 0)
		return (printf("Error initializing with mutex, sim_mutex"));
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
		return (printf("Error initializing with mutex, print_mutex"));
	if (philo_init(table) != 0)
		return (printf("Error with philo initialization\n"));
	return (0);
}
