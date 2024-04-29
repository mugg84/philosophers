/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:26:10 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/28 15:43:18 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	gettime(int	time)
{
	struct timeval	tv;

	if(gettimeofday(&tv, NULL))
		print_error("Gettimeofday error");
	if (time == SECONDS)
		return (tv.tv_sec + (tv.tv_usec / 1000000));
	else if (time == MILLISEC)
		return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
	else if (time == MICROSEC)
		return (tv.tv_sec * 1000000 + (tv.tv_usec));
	else
		print_error("Wrong gettimeofday input");
	return (EXIT_FAILURE);
}

void	parser(char *argv[], t_data **data)
{
	(*data)->philo_number = ft_atol(argv[1]);
	(*data)->time_to_die = ft_atol(argv[2]) * 1000;
	(*data)->time_to_eat = ft_atol(argv[3]) * 1000;
	(*data)->time_to_sleep = ft_atol(argv[4]) * 1000;
	if ((*data)->time_to_die < 6000 || (*data)->time_to_eat < 60000 || (*data)->time_to_sleep < 60000)
		print_error("Inputs bigger than 60 milliseconds");
	else if ((*data)->philo_number == 0)
		print_error("At least one philospher needed");
	if (argv[5])
		(*data)->meals_number = ft_atol(argv[5]);
	else
		(*data)->meals_number = -1;
}
void	set_philo_forks(t_philo *philo, t_data *data, int i)
{
	philo->first_fork = &data->forks[(i + 1) % data->philo_number];
	philo->second_fork = &data->forks[i];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &data->forks[i];
		philo->second_fork = &data->forks[(i + 1) % data->philo_number];
		printf("philo num %d, first fork %d, second fork %d\n", philo->id, data->forks[i].fork_id, data->forks[(i + 1) % data->philo_number].fork_id);
	}
	else
		printf("philo num %d, first fork %d, second fork %d\n", philo->id, data->forks[(i + 1) % data->philo_number].fork_id, data->forks[i].fork_id);

}

void	philo_init(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < data->philo_number)
	{
		philo = data->philos + i;
		philo->id = i + 1;
		philo->is_full = false;
		philo->data = data;
		philo->philo_mutex = malloc(sizeof(t_mutex));
		if (!philo->philo_mutex)
			print_error("Mutex malloc error");
		if(pthread_mutex_init(philo->philo_mutex, NULL) != 0)
			print_error("Philo mutex error");
		set_philo_forks(philo, data, i);
	}
}

void	handle_one_philo(t_data *data)
{
	;
}

bool	wait_is_ready(t_data *data)
{
	bool	ready;

	pthread_mutex_lock(data->data_mutex);
	ready = data->is_ready;
	pthread_mutex_unlock(data->data_mutex);
	return (ready);
}

bool	all_threads_running(t_mutex *mutex, long running_threads, long philo_number)
{
	bool	all_running;

	all_running = false;
	pthread_mutex_lock(mutex);
	if (running_threads == philo_number)
		all_running = true;
	pthread_mutex_unlock(mutex);
	return (all_running);
}

void	*philo_monitor(void *v_data)
{
	t_data	*data;

	data = (t_data *)v_data;
	while (!all_threads_running(data->data_mutex, data->running_threads, data->philo_number))
		;
}

void	wait_threads(t_data *data)
{
	while (!wait_is_ready(data))
		;
}

void	usleep_updated(long usec, t_data *data)
{
	long	start;
	long	elapsed;
	long	rem;

	start = gettime(MICROSEC);
	while (gettime(MICROSEC) - start < usec)
	{
		if (data->is_finished)
			break;
		elapsed = gettime(MICROSEC) - start;
		rem = usec - elapsed;
		if (rem > 1000)
			usleep(rem / 2);
		else
		{
			while (gettime(MICROSEC) - start < usec)
				;
		}
	}
}

void	print_status(int status, t_philo * philo)
{
	long	time;

	time = gettime(MICROSEC) - philo->data->init_time;
	if (philo->is_full)
		return;
	pthread_mutex_lock(philo->data->write_mutex);
	if ((status == FIRST_FORK || status == SECOND_FORK) && !philo->data->is_finished)
		printf("%6ld %d has taken a fork\n", time, philo->id);
	// if (philo->id % 2 == 0)
	// {
	// 	if (status == FIRST_FORK && !philo->data->is_finished)
	// 		printf("%6ld %d has taken first fork number %d\n", time, philo->id, philo->data->forks[((philo->id)-1)].fork_id);
	// 	else if(status == SECOND_FORK && !philo->data->is_finished)
	// 		printf("%6ld %d has taken second fork number %d\n", time, philo->id, philo->data->forks[(philo->id) % philo->data->philo_number].fork_id);
	// }
	// else
	// {
	// 	if (status == FIRST_FORK && !philo->data->is_finished)
	// 		printf("%6ld %d has taken first fork number %d\n", time, philo->id,  philo->data->forks[(philo->id) % philo->data->philo_number].fork_id);
	// 	else if(status == SECOND_FORK && !philo->data->is_finished)
	// 		printf("%6ld %d has taken second fork number %d\n", time, philo->id, philo->data->forks[((philo->id)-1)].fork_id);
	// }
	if (status == EATING && !philo->data->is_finished)
		printf("%-6ld %d is eating\n", time, philo->id);
	else if (status == SLEEPING && !philo->data->is_finished)
		printf("%-6ld %d is sleeping\n", time, philo->id);
	else if (status == THINKING  && !philo->data->is_finished)
		printf("%-6ld %d is thinking\n", time, philo->id);
	else if (status == DEAD)
		printf("%-6ld %d died\n", time, philo->id);
	pthread_mutex_unlock(philo->data->write_mutex);
}

void	eating(t_philo * philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	print_status(FIRST_FORK, philo);

	pthread_mutex_lock(&philo->second_fork->fork);
	print_status(SECOND_FORK, philo);

	pthread_mutex_lock(philo->philo_mutex);
	philo->last_meal_timer = gettime(MILLISEC);
	pthread_mutex_unlock(philo->philo_mutex);
	philo->meals_counter++;

	print_status(EATING, philo);
	usleep_updated(philo->data->time_to_eat, philo->data);

	if (philo->data->meals_number > 0 && philo->meals_counter == philo->data->meals_number)
	{
		pthread_mutex_lock(philo->philo_mutex);
		philo->is_full = true;
		pthread_mutex_unlock(philo->philo_mutex);
	}
	pthread_mutex_unlock(&philo->first_fork->fork);
	pthread_mutex_unlock(&philo->second_fork->fork);
}

void	sleeping(t_philo * philo)
{
	print_status(SLEEPING, philo);
	usleep_updated(philo->data->time_to_sleep, philo->data);
}

void	thinking(t_philo * philo)
{
	print_status(THINKING, philo);
}

void	*run_sim(void *v_philo)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	wait_threads(philo->data);
	pthread_mutex_lock(philo->data->data_mutex);
	philo->data->running_threads++;
	pthread_mutex_unlock(philo->data->data_mutex);
	philo->last_meal_timer = philo->data->init_time;
	while (!philo->data->is_finished)
	{
		if (philo->is_full)
			break;
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
}



void	sim_init(t_data *data)
{
	int	i;

	i = -1;
	if (data->meals_number == 0)
		return ;
	else if (data->philo_number == 1)
		handle_one_philo(data);
	else
	{
		while (++i < data->philo_number)
			pthread_create(&data->philos[i].thread_id, NULL, run_sim, (void *)&data->philos[i]);
	}
	if (pthread_create(&data->monitor, NULL, philo_monitor, (void*)data) != 0)
		print_error("Monitor pthread create error");
	data->init_time = gettime(MILLISEC);
	pthread_mutex_lock(data->data_mutex);
	data->is_ready = true;
	pthread_mutex_unlock(data->data_mutex);
	i = -1;
	while (++i < data->philo_number)
		pthread_join(data->philos[i].thread_id, NULL);
	//full
}


void	free_data(t_data **data)
{
	free(*data);
}

void	init(t_data *data)
{
	int	i;

	i = -1;
	data->init_time = 0;
	data->running_threads = 0;
	data->is_running = false;
	data->is_ready = false;
	data->is_finished = false;
	data->forks = malloc(sizeof(t_fork) * data->philo_number);
	data->philos = malloc(sizeof(t_philo) * data->philo_number);
	data->data_mutex = malloc(sizeof(t_mutex));
	data->write_mutex = malloc(sizeof(t_mutex));
	//data->monitor = malloc(sizeof(pthread_t));
	if (!data->forks || !data->philos || !data->data_mutex || !data->write_mutex)
		print_error("Malloc error");
	if (pthread_mutex_init(data->data_mutex, NULL) != 0)
		print_error("Data mutex init error");
	if (pthread_mutex_init(data->write_mutex, NULL) != 0)
		print_error("Write mutex init error");
	while (++i < data->philo_number)
	{
		if (pthread_mutex_init(&data->forks[i].fork, NULL) != 0)
			print_error("Mutex init error");
		data->forks[i].fork_id = i;
	}
	philo_init(data);
}



int	main(int argc, char *argv[])
{
	t_data	*data;
	
	if (argc == 5 || argc == 6)
	{
		data = malloc(sizeof(t_data));
		if (!data)
			print_error("Malloc error");
		parser(argv, &data);
		init(data);
		sim_init(data);
		free_data(&data);
	}
	else
		print_error("Wrong input\n");
	return (EXIT_SUCCESS);
}