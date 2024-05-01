/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_b.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 09:09:38 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/01 10:59:06 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_B_H
# define PHILO_B_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>
# include <sys/types.h>
# include <time.h>
# include "semaphore.h"

# define MILLISEC 0
# define MICROSEC 1

# define EATING 0
# define SLEEPING 1
# define THINKING 2
# define FIRST_FORK 3
# define SECOND_FORK 4
# define DEAD 5

typedef struct s_data
{
	long		philo_number;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		meals_number;
	long		init_time;
	long		index;
	bool		is_running;
	bool		is_finished;
	bool		is_dead;
	sem_t		*sem_print;
	sem_t		*sem_fork;
	pid_t		*pid;
	pthread_t	monitor;	
} t_data;

#endif