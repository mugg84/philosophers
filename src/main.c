/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:26:10 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/27 19:05:14 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &data->forks[i];
		philo->second_fork = &data->forks[(i + 1) % data->philo_number];
	}
	else
	{
		philo->first_fork = &data->forks[(i + 1) % data->philo_number];
		philo->second_fork = &data->forks[i];
	}
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
		set_philo_forks(philo, data, i);
	}
}

void	handle_one_philo(t_data *data)
{
	;
}

void	wait_threads(t_data *data)
{
	while (!data->is_ready)
		;
}

void	*run_sim(void *v_philo)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	wait_threads(philo->data);
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
	pthread_mutex_unlock(data->data_mutex);
	data->is_ready = true;
	pthread_mutex_lock(data->data_mutex);

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
	data->is_running = false;
	data->is_ready = false;
	data->forks = malloc(sizeof(t_fork) * data->philo_number);
	data->philos = malloc(sizeof(t_philo) * data->philo_number);
	data->data_mutex = malloc(sizeof(t_mutex));
	if (!data->forks || !data->philos || !data->data_mutex)
			print_error("Malloc error");
	if (pthread_mutex_init(data->data_mutex, NULL) != 0)
			print_error("Mutex init error");
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