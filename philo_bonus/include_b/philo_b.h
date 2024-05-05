/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_b.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 09:09:38 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/05 08:32:49 by mmughedd         ###   ########.fr       */
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
# include <sys/wait.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>

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
	long		meals_target;
	long		init_time;
	long		is_full_counter;
	bool		is_running;
	bool		is_finished;
	sem_t		*sem_print;
	sem_t		*sem_fork;
	sem_t		*sem_finished;
	sem_t		*sem_last_meal;
	sem_t		*sem_meal_counter;

} t_data;

typedef struct s_philo
{
	long		meals_target;
	long		last_meal;
	int			index;
	bool		is_dead;
	bool		is_finished;
	long		meals_counter;
	t_data		*data;
	pid_t		pid;
} t_philo;

char	*check_input(char *n);
void	parser(char *argv[], t_data **data);
void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	thinking(t_philo *philo, bool for_desync);
void	set_last_meal(t_philo *philo);
void	set_meal_counter(t_philo *philo);
void	set_data(t_data *data);
void	set_philo(t_philo **philo, t_data *data);
void	usleep_updated(long usec);
void	desynchronize_philos(t_philo *philo);
void	free_data(t_data *data, t_philo *philo);
void	kill_processes(t_data *data, t_philo *philo);
void	print_error(char *err);
void	print_status(int status, t_philo *philo);
bool	is_dead(t_philo *philo);
bool	is_full(t_philo *philo);
long	gettime(int time);
long	ft_atol(char *n);



#endif