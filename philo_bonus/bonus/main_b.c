/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 08:52:26 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/05 08:50:32 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_b/philo_b.h"

void	set_data(t_data *data)
{
	data->init_time = 0;
	data->is_running = 0;
	data->is_finished = 0;
	data->is_full_counter = data->philo_number;
	sem_unlink("/sem_print");
	sem_unlink("/sem_fork");
	sem_unlink("/sem_finished");
	sem_unlink("/sem_last_meal");
	sem_unlink("/sem_meal_counter");
	data->sem_print = sem_open("/sem_print", O_CREAT, 0644, 1);
	data->sem_fork = sem_open("/sem_fork",  O_CREAT, 0644, data->philo_number);
	data->sem_finished = sem_open("/sem_finished", O_CREAT, 0644, 1);
	data->sem_last_meal= sem_open("/sem_last_meal", O_CREAT, 0644, 1);
	data->sem_meal_counter = sem_open("/sem_meal_counter", O_CREAT, 0644, 1);
	if (!data->sem_print || !data->sem_fork || !data->sem_finished 
		|| !data->sem_last_meal || !data->sem_meal_counter)
		print_error("Semaphore open error");
}

void	*monitor_sim(void *v_philo)
{
	t_philo	*philo;
	
	philo = (t_philo *)v_philo;
	while (!philo->is_finished)
	{
		usleep_updated(100);
		if (is_dead(philo))
		{
			print_status(DEAD, philo);
			sem_post(philo->data->sem_finished);
			philo->is_dead = true;
			break;
		}
		if (is_full(philo))
		{
			philo->is_finished =true;
			break;
		}
	}
	if (philo->is_dead)
		exit(1);
	else
		exit(0);
	return (NULL);
}

void	run_philo_sim(t_philo *philo)
{
	pthread_t	monitor;

	sem_wait(philo->data->sem_last_meal);
	philo->last_meal = gettime(MILLISEC);
	sem_post(philo->data->sem_last_meal);
	if (pthread_create(&monitor, NULL, monitor_sim, (void *)philo))
		print_error("Monitor thread create error");
	desynchronize_philos(philo);
	while (1)
	{
		eating(philo);
		sleeping(philo);
		thinking(philo, false);
	}
	pthread_join(monitor, NULL);
}

void	init_sim(t_data *data, t_philo *philo)
{
	int	i;

	i = -1;
	data->init_time = gettime(MILLISEC);
	while (++i < data->philo_number)
	{
		philo[i].pid = fork();
		if (philo[i].pid == -1)
			print_error("Fork error");
		if (philo[i].pid == 0)
			run_philo_sim(&philo[i]);
	}
}

int	main(int argc, char *argv[])
{
	t_data	*data;
	t_philo	*philo = NULL;

	if (argc == 5 || argc == 6)
	{
		data = malloc(sizeof(t_data));
		if (!data)
			print_error("data malloc error");
		parser(argv, &data);
		set_data(data);
		philo = malloc(sizeof(t_philo) * data->philo_number);
		if (!philo)
			print_error("Philo malloc error");
		set_philo(&philo, data);
		init_sim(data, philo);
		free_data(data, philo);
	}
	else
		print_error("Wrong input");
	return (EXIT_SUCCESS);
}