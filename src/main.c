/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:26:10 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/30 09:15:12 by mmughedd         ###   ########.fr       */
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
		if(pthread_mutex_init(&philo->philo_mutex, NULL) != 0)
			print_error("Philo mutex error");
		set_philo_forks(philo, data, i);
	}
}

bool	wait_is_ready(t_data *data)
{
	bool	ready;

	pthread_mutex_lock(&data->data_mutex);
	ready = data->is_ready;
	pthread_mutex_unlock(&data->data_mutex);
	return (ready);
}

bool	is_finished(t_data *data)
{
	bool	finished;

	finished = false;
	pthread_mutex_lock(&data->data_mutex);
	if (data->is_finished)
		finished = true;
	pthread_mutex_unlock(&data->data_mutex);
	return (finished);
}

bool	all_threads_running(t_mutex *mutex, t_data *data)
{
	bool	all_running;

	all_running = false;
	pthread_mutex_lock(mutex);
	if (data->running_threads == data->philo_number)
		all_running = true;
	pthread_mutex_unlock(mutex);
	return (all_running);
}

bool	is_philo_dead(t_philo *philo)
{
	long	time;
	long	time_to_die;
	bool	is_dead;

	is_dead = false;
	pthread_mutex_lock(&philo->philo_mutex);
	if (!philo->is_full)
	{
		time = gettime(MILLISEC) - philo->last_meal_timer;
		time_to_die = philo->data->time_to_die / 1000;
		if (time > time_to_die)
			is_dead = true;
	}
	pthread_mutex_unlock(&philo->philo_mutex);
	return (is_dead);
}

void	print_status(int status, t_philo * philo)
{
	long	time;

	time = gettime(MILLISEC) - philo->data->init_time;
	pthread_mutex_lock(&philo->philo_mutex);
	if (philo->is_full)
	{
		pthread_mutex_unlock(&philo->philo_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_lock(&philo->data->write_mutex);
	if ((status == FIRST_FORK || status == SECOND_FORK) && !is_finished(philo->data))
		printf("%6ld %d has taken a fork\n", time, philo->id);
	if (status == EATING && !is_finished(philo->data))
		printf("%6ld %d is eating\n", time, philo->id);
	else if (status == SLEEPING && !is_finished(philo->data))
		printf("%6ld %d is sleeping\n", time, philo->id);
	else if (status == THINKING  && !is_finished(philo->data))
		printf("%6ld %d is thinking\n", time, philo->id);
	else if (status == DEAD)
		printf("%6ld %d died\n", time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
}

void	*philo_monitor(void *v_data)
{
	t_data	*data;
	int		i;

	data = (t_data *)v_data;
	while (!all_threads_running(&data->data_mutex, data))
		;
	while (!is_finished(data))
	{
		i = -1;
		while (++i < data->philo_number && !is_finished(data))
		{
			if (is_philo_dead(data->philos + i))
			{
				pthread_mutex_lock(&data->data_mutex);
				data->is_finished = true;
				pthread_mutex_unlock(&data->data_mutex);
				print_status(DEAD, data->philos + i);
			}
		}
	}
	return (NULL);
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
		if (is_finished(data))
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

void	eating(t_philo * philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	print_status(FIRST_FORK, philo);

	pthread_mutex_lock(&philo->second_fork->fork);
	print_status(SECOND_FORK, philo);

	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal_timer = gettime(MILLISEC);
	pthread_mutex_unlock(&philo->philo_mutex);
	philo->meals_counter++;

	print_status(EATING, philo);
	usleep_updated(philo->data->time_to_eat, philo->data);

	if (philo->data->meals_number > 0 && philo->meals_counter == philo->data->meals_number)
	{
		pthread_mutex_lock(&philo->philo_mutex);
		philo->is_full = true;
		pthread_mutex_unlock(&philo->philo_mutex);
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

void	*handle_one_philo(void *v_philo)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	wait_threads(philo->data);
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->data->running_threads++;
	philo->last_meal_timer = gettime(MILLISEC);
	pthread_mutex_unlock(&philo->data->data_mutex);
	print_status(FIRST_FORK, philo);
	while (!is_finished(philo->data))
		usleep_updated(60, philo->data);
	return (NULL);
}

void	*run_sim(void *v_philo)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	wait_threads(philo->data);

	pthread_mutex_lock(&philo->data->data_mutex);
	philo->data->running_threads++;
	philo->last_meal_timer = gettime(MILLISEC);
	pthread_mutex_unlock(&philo->data->data_mutex);
	
	while (!is_finished(philo->data))
	{
		if (philo->is_full)
			break;
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

void	sim_init(t_data *data)
{
	int	i;

	i = -1;
	if (data->meals_number == 0)
		return ;
	else if (data->philo_number == 1)
		pthread_create(&data->philos->thread_id, NULL, handle_one_philo, (void *)data->philos);
	else
	{
		while (++i < data->philo_number)
			pthread_create(&data->philos[i].thread_id, NULL, run_sim, (void *)&data->philos[i]);
	}
	if (pthread_create(&data->monitor, NULL, philo_monitor, (void*)data) != 0)
		print_error("Monitor pthread create error");
	data->init_time = gettime(MILLISEC);
	pthread_mutex_lock(&data->data_mutex);
	data->is_ready = true;
	pthread_mutex_unlock(&data->data_mutex);
	i = -1;
	while (++i < data->philo_number)
		pthread_join(data->philos[i].thread_id, NULL);

	pthread_mutex_lock(&data->data_mutex);
	data->is_finished = true;
	pthread_mutex_unlock(&data->data_mutex);
	
	pthread_join(data->monitor, NULL);
}


void	free_data(t_data *data)
{
	int	i;
	
	i = -1;
	while (++i < data->philo_number)
	{
		pthread_mutex_destroy(&(data->philos + i)->philo_mutex);
		pthread_mutex_destroy(&(data->forks + i)->fork);
	}
	pthread_mutex_destroy(&(data->data_mutex));
	pthread_mutex_destroy(&(data->write_mutex));
	free(data->forks);
	free(data->philos);
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
	if (!data->forks || !data->philos)
		print_error("Malloc error");
	if (pthread_mutex_init(&data->data_mutex, NULL) != 0)
		print_error("Data mutex init error");
	if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
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
		free_data(data);
	}
	else
		print_error("Wrong input\n");
	return (EXIT_SUCCESS);
}