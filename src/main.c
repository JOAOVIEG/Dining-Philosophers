/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaocard <joaocard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 10:55:29 by joaocard          #+#    #+#             */
/*   Updated: 2024/05/26 13:56:55 by joaocard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_threads(t_table *table, size_t i)
{
	if (table->nbr_philos == 1)
	{
		if (pthread_create(&table->philos[0].thread_id, NULL, \
							dinner_for_one, &table->philos[0]) != 0)
			return (printf("ERROR creating thread for philo nr %ld\n", i));
	}
	else
	{
		while (i < table->nbr_philos)
		{
			if (pthread_create(&table->philos[i].thread_id, \
							NULL, dinner, &table->philos[i]) != 0)
				return (printf("ERROR creating thread for philo nr %ld\n", i));
			i++;
		}
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_table		table;

	if (ac < 5 || ac > 6)
		return (printf("Wrong number of arguments\n"));
	else if (ac == 5 || ac == 6)
	{
		if (parse_args(&table, av) != 0)
			return (printf("Error parsing arguments\n"));
		if (init_table(&table) != 0)
			return (printf("Error initializing table\n"));
		if (dinner_init(&table) != 0)
			return (printf("Error initializing dinner\n"));
		clean_table(&table);
	}
	return (0);
}
