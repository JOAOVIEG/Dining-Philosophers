/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner_utilis_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaocard <joaocard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:30:43 by joaocard          #+#    #+#             */
/*   Updated: 2024/05/20 14:49:40 by joaocard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	use_forks(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->prio_fork->lock) != 0)
		printf("Error locking mutex\n");
	print_status(philo, TAKE_FORK_1);
	if (pthread_mutex_lock(&philo->sec_fork->lock) != 0)
		printf("Error locking mutex\n");
	print_status(philo, TAKE_FORK_2);
}

void	disuse_forks(t_philo *philo)
{
	if (pthread_mutex_unlock(&philo->prio_fork->lock) != 0)
		printf("Error locking mutex\n");
	if (pthread_mutex_unlock(&philo->sec_fork->lock) != 0)
		printf("Error locking mutex\n");
}

bool	death_event(t_philo *philo)
{
	long	dt;

	if (get_dinner_state(&philo->philo_lock, &philo->full) == true || \
		get_dinner_state(&philo->table->dinner_lock, &philo->table->dinner_end))
		return (false);
	dt = gettimeofday_ms() - \
			get_last_meal(&philo->philo_lock, &philo->last_meal);
	if (dt > (philo->table->time_to_die) / 1000)
		return (true);
	return (false);
}

long	get_last_meal(pthread_mutex_t *mutex, size_t *last_meal)
{
	long	var;

	if (pthread_mutex_lock(mutex) != 0)
		printf("Error locking mutex\n");
	var = *last_meal;
	if (pthread_mutex_unlock(mutex) != 0)
		printf("Error unlocking mutex\n");
	return (var);
}

void	clean_table(t_table *table)
{
	size_t	i;
	t_philo	*philo;

	i = 0;
	while (i < table->nbr_philos)
	{
		philo = table->philos + i;
		if (pthread_mutex_destroy(&table->forks[i].lock) != 0)
			printf("Error destroying mutex");
		if (pthread_mutex_destroy(&philo->philo_lock) != 0)
			printf("Error destroying mutex");
		i++;
	}
	if (pthread_mutex_destroy(&table->dinner_lock) != 0)
		printf("Error destroying mutex");
	if (pthread_mutex_destroy(&table->print_lock) != 0)
		printf("Error destroying mutex");
	free(table->forks);
	free(table->philos);
}
