/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner_utilis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaocard <joaocard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:45:48 by joaocard          #+#    #+#             */
/*   Updated: 2024/05/20 14:34:06 by joaocard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sync_threads(t_table *table)
{
	while (get_dinner_state(&table->dinner_lock, \
				&table->dinner_is_synchro) == false)
		;
}

void	set_last_meal(pthread_mutex_t *mutex, size_t *to_set, size_t state)
{
	if (pthread_mutex_lock(mutex) != 0)
		printf("Error locking mutex\n");
	*to_set = state;
	if (pthread_mutex_unlock(mutex) != 0)
		printf("Error unlocking mutex\n");
}

bool	get_dinner_state(pthread_mutex_t *mutex, bool *is_sync)
{
	bool	value;

	if (pthread_mutex_lock(mutex) != 0)
		printf("Error locking mutex\n");
	value = *is_sync;
	if (pthread_mutex_unlock(mutex) != 0)
		printf("Error unlocking mutex\n");
	return (value);
}

void	*monitor(void *arg)
{
	size_t	i;
	t_table	*table;

	table = (t_table *)arg;
	while (all_threads_running(&table->dinner_lock, \
			&table->nbr_threads_running, table->nbr_philos) == false)
		;
	while (get_dinner_state(&table->dinner_lock, \
			&table->dinner_end) == false)
	{
		i = 0;
		while (i < table->nbr_philos \
				&& get_dinner_state(&table->dinner_lock, \
				&table->dinner_end) == false)
		{
			if (death_event(&table->philos[i]) == true)
			{
				print_status(&table->philos[i], DIE);
				set_dinner_state(&table->dinner_lock, &table->dinner_end, true);
			}
			i++;
		}
	}
	return (NULL);
}

bool	all_threads_running(pthread_mutex_t *mutex, \
						size_t *th_nbr, size_t ph_nbr)
{
	bool	value;

	value = false;
	if (pthread_mutex_lock(mutex) != 0)
		printf("Error locking mutex\n");
	if (*th_nbr == ph_nbr)
		value = true;
	if (pthread_mutex_unlock(mutex) != 0)
		printf("Error unlocking mutex\n");
	return (value);
}
