/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_utilis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaocard <joaocard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:59:50 by joaocard          #+#    #+#             */
/*   Updated: 2024/05/26 14:07:54 by joaocard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	non_digit(const char *av, int *error)
{
	if (*(char *)av && (*(char *)av < '0' || *(char *)av > '9'))
	{
		printf("Wrong input: only integers allowed\n");
		*error = 1;
	}
	return (*error);
}

int	overflow(int integer, const char *av, int *error)
{
	if (integer > INT_MAX / 10 || \
			(integer == INT_MAX / 10 && *(char *)av - '0' > INT_MAX % 10))
	{
		printf("Wrong input: Max int reached. Overflow is on!\n");
		*error = 1;
	}
	return (*error);
}
