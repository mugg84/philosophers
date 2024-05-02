/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:38:04 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/30 13:29:01 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>
# include <time.h>

# define SECONDS 0
# define MILLISEC 1
# define MICROSEC 2

# define EATING 0
# define SLEEPING 1
# define THINKING 2
# define FIRST_FORK 3
# define SECOND_FORK 4
# define DEAD 5

typedef struct s_data	t_data;
typedef pthread_mutex_t	t_mutex;

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
	pthread_t	t_id;
	t_data		*data;
	t_mutex		philo_mutex;
}	t_philo;

struct s_data
{
	long		philo_number;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		meals_number;
	long		running_threads;
	long		init_time;
	bool		is_running;
	bool		is_ready;
	bool		is_finished;
	t_mutex		data_mutex;
	t_mutex		write_mutex;
	t_fork		*forks;
	pthread_t	monitor;
	t_philo		*philos;
};

char	*check_input(char *n);
long	ft_atol(char *n);
long	gettime(int time);
void	print_status(int status, t_philo *philo);
void	print_error(char *err);
void	usleep_updated(long usec, t_data *data);
void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	thinking(t_philo *philo, bool for_desync);
void	free_data(t_data *data);
void	*philo_monitor(void *v_data);
void	*one_philo(void *v_philo);
void	philo_init(t_data *data);
void	set_philo_forks(t_philo *philo, t_data *data, int i);
void	wait_threads(t_data *data);
void	set_is_finished(t_data *data);
void	set_is_ready(t_data *data);
void	create_threads(t_data *d);
void	*run_sim(void *v_philo);
void	desynchronize_philos(t_philo *philo);
bool	wait_is_ready(t_data *data);
bool	is_finished(t_data *data);
bool	all_threads_running(t_mutex *mutex, t_data *data);
bool	is_philo_dead(t_philo *philo);
bool	is_philo_full(t_philo *philo);

#endif