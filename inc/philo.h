/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaocard <joaocard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 10:55:45 by joaocard          #+#    #+#             */
/*   Updated: 2024/05/26 14:06:50 by joaocard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_table	t_table;

typedef enum e_action
{
	EAT,
	THINK,
	SLEEP,
	TAKE_FORK_1,
	TAKE_FORK_2,
	DIE,
}			t_action;

typedef struct s_fork
{
	pthread_mutex_t	lock;
}					t_fork;

typedef struct s_philo
{
	int					id;
	size_t				meals_count;
	size_t				last_meal;
	bool				full;
	pthread_t			thread_id;
	pthread_mutex_t		philo_lock;
	t_fork				*prio_fork;
	t_fork				*sec_fork;
	t_table				*table;
}						t_philo;

typedef struct s_table
{
	size_t			nbr_philos;
	size_t			nbr_threads_running;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	size_t			nb_times_must_eat;
	size_t			start;
	bool			dinner_end;
	bool			dinner_is_synchro;
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	dinner_lock;
	pthread_mutex_t	print_lock;
	pthread_t		monitor_th;
}					t_table;

/*parsing_and_init.c*/
int		philo_init(t_table *table);
int		parse_args(t_table *table, char **av);
int		init_table(t_table	*table);
void	give_forks(t_philo *philo, t_fork *forks, int pos);
int		ft_atoi(const char *av, int *error);
int		non_digit(const char *av, int *error);
int		overflow(int integer, const char *av, int *error);
/*Diner init and routine*/
int		dinner_init(t_table *table);
int		init_threads(t_table *table, size_t i);
void	*dinner(void *arg);
void	*dinner_for_one(void *arg);
void	sync_threads(t_table *table);
bool	get_dinner_state(pthread_mutex_t *mutex, bool *is_sync);
void	set_last_meal(pthread_mutex_t *mutex, size_t	*to_set, size_t state);
void	set_dinner_state(pthread_mutex_t *mutex, bool *state, bool value);
void	sync_rout_start(t_philo *philo);

/*Time functions*/
long	gettimeofday_ms(void);
void	my_usleep(long time, t_table *table);
/*Action logs*/
void	print_status(t_philo *philo, t_action action);
char	*get_action(t_action action);
void	thinking_routine(t_philo *philo);
void	eating_routine(t_philo *philo);
void	use_forks(t_philo *philo);
void	disuse_forks(t_philo *philo);
/*Monitor functions*/
void	*monitor(void *arg);
bool	all_threads_running(pthread_mutex_t *mutex, \
						size_t *th_nbr, size_t ph_nbr);
bool	death_event(t_philo *philo);
long	get_last_meal(pthread_mutex_t *mutex, size_t *last_meal);
/*Clean function*/
void	clean_table(t_table *table);
#endif