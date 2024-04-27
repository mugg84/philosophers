/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:38:04 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/27 15:03:38 by mmughedd         ###   ########.fr       */
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
	t_fork	*forks;
	t_philo	*philos;
};

#endif