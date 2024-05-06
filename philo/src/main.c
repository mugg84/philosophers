/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:26:10 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/06 12:53:23 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	parser(char *argv[], t_data **data)
{
	(*data)->philo_number = ft_atol(argv[1]);
	(*data)->time_to_die = ft_atol(argv[2]) * 1000;
	(*data)->time_to_eat = ft_atol(argv[3]) * 1000;
	(*data)->time_to_sleep = ft_atol(argv[4]) * 1000;
	if ((*data)->time_to_die < 60000
		|| (*data)->time_to_eat < 60000 || (*data)->time_to_sleep < 60000)
		print_error("Inputs bigger than 60 milliseconds");
	else if ((*data)->philo_number == 0)
		print_error("At least one philospher needed");
	if (argv[5])
		(*data)->meals_number = ft_atol(argv[5]);
	else
		(*data)->meals_number = -1;
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
	desynchronize_philos(philo);
	while (!is_finished(philo->data))
	{
		if (philo->is_full)
			break ;
		eating(philo);
		sleeping(philo);
		thinking(philo, false);
	}
	return (NULL);
}

void	sim_init(t_data *d)
{
	int	i;

	i = -1;
	if (d->meals_number == 0)
		return ;
	else if (d->philo_number == 1)
		pthread_create(&d->philos->t_id, NULL, one_philo, (void *)d->philos);
	else
		create_threads(d);
	if (pthread_create(&d->monitor, NULL, philo_monitor, (void *)d) != 0)
		print_error("Monitor pthread create error");
	d->init_time = gettime(MILLISEC);
	set_is_ready(d);
	i = -1;
	while (++i < d->philo_number)
		pthread_join(d->philos[i].t_id, NULL);
	set_is_finished(d);
	pthread_join(d->monitor, NULL);
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
		print_error("Wrong input");
	return (EXIT_SUCCESS);
}
