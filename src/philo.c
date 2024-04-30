/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:54:30 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/30 12:51:40 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_philo_forks(t_philo *philo, t_data *data, int i)
{
	philo->first_fork = &data->forks[(i + 1) % data->philo_number];
	philo->second_fork = &data->forks[i];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &data->forks[i];
		philo->second_fork = &data->forks[(i + 1) % data->philo_number];
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
		philo->meals_counter = 0;
		if (pthread_mutex_init(&philo->philo_mutex, NULL) != 0)
			print_error("Philo mutex error");
		set_philo_forks(philo, data, i);
	}
}

void	*one_philo(void *v_philo)
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
