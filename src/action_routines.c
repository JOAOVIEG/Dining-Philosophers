/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_routines.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaocard <joaocard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:48:26 by joaocard          #+#    #+#             */
/*   Updated: 2024/05/20 14:30:55 by joaocard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*get_action(t_action action)
{
	char	*p_action;

	if (action == EAT)
		p_action = " is eating\n";
	else if ((action == TAKE_FORK_1 || action == TAKE_FORK_2))
		p_action = " has taken a fork\n";
	else if (action == THINK)
		p_action = " is thinking\n";
	else if (action == SLEEP)
		p_action = " is sleeping\n";
	else if (action == DIE)
		p_action = " died\n";
	else
		p_action = NULL;
	return (p_action);
}

void	print_status(t_philo *philo, t_action action)
{
	long		delta_t;
	char		*philo_action;

	delta_t = gettimeofday_ms() - philo->table->start;
	if (get_dinner_state(&philo->philo_lock, &philo->full))
	{
		philo_action = NULL;
		return ;
	}
	if (pthread_mutex_lock(&philo->table->print_lock) != 0)
		printf("Error locking mutex\n");
	philo_action = get_action(action);
	if (philo_action && get_dinner_state(&philo->table->dinner_lock, \
			&philo->table->dinner_end) == false)
		printf("%ld %d %s", delta_t, philo->id, philo_action);
	if (pthread_mutex_unlock(&philo->table->print_lock) != 0)
		printf("Error unlocking mutex\n");
}

void	thinking_routine(t_philo *philo)
{
	long	dt_think;

	if (philo->table->nbr_philos % 2 == 0)
		return ;
	dt_think = (philo->table->time_to_eat * 2) - philo->table->time_to_sleep;
	if (dt_think < 0)
		dt_think = 0;
	my_usleep(dt_think * 0.42, philo->table);
}

void	eating_routine(t_philo *philo)
{
	use_forks(philo);
	set_last_meal(&philo->philo_lock, &philo->last_meal, gettimeofday_ms());
	philo->meals_count++;
	print_status(philo, EAT);
	my_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->nb_times_must_eat > 0 \
		&& philo->meals_count == philo->table->nb_times_must_eat)
		set_dinner_state(&philo->philo_lock, &philo->full, true);
	disuse_forks(philo);
}
