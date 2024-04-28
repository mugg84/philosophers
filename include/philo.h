/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:38:04 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/28 10:32:59 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
	#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/time.h>
#include <time.h>

# define SECONDS 0
# define MILLISEC 1
# define MICROSEC 2

# define EATING 0
# define SLEEPING 1
# define THINKING 2
# define FIRST_FORK 3
# define SECOND_FORK 4
# define DEAD 5

long	ft_atol(char *n);
void	print_error(char *err);

typedef struct s_data t_data;
typedef pthread_mutex_t t_mutex;

typedef struct s_fork
{
	t_mutex	fork;
	int		fork_id;
}	t_fork;

typedef struct s_philo
{
	int			id;
	long		meals_counter;
	long		last_meal_timer;
	bool		is_full;
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;
	t_data		*data;
	t_mutex		*philo_mutex;
}	t_philo;

struct s_data
{
	long	philo_number;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	meals_number;
	long	init_time;
	bool	is_running;
	bool	is_ready;
	bool	is_finished;
	t_mutex	*data_mutex;
	t_mutex	*write_mutex;
	t_fork	*forks;
	t_philo	*philos;
};

#endif