/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:56:48 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/07 07:08:54 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Checks is_ready flag
 * 
 * Arguments:
 * - Data structure
 * 
 * Returns:
 * - True if is_finished, false otherwhise
 */
bool	wait_is_ready(t_data *data)
{
	bool	ready;

	pthread_mutex_lock(&data->data_mutex);
	ready = data->is_ready;
	pthread_mutex_unlock(&data->data_mutex);
	return (ready);
}

/* Checks is_finished flag
 * 
 * Arguments:
 * - Data structure
 * 
 * Returns:
 * - True if is_finished, false otherwhise
 */
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

/* Checks all_threads_running flag
 * 
 * Arguments:
 * - Data structure
 * 
 * Returns:
 * - True if all the threads are running, false otherwhise
 */
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

/* Calculates if time since last meal is bigger than time_to_die
 * and if so sets the is_dead flag to true
 * 
 * Arguments:
 * - Philo structure
 * 
 * Returns:
 * - True if is_dead, false otherwhise
 */
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

/* Checks philo structure is_full flag
 * 
 * Arguments:
 * - Philo structure
 * 
 * Returns:
 * - True if is_full, false otherwhise
 */
bool	is_philo_full(t_philo *philo)
{
	bool	is_full;

	is_full = false;
	pthread_mutex_lock(&philo->philo_mutex);
	if (philo->is_full)
		is_full = true;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (is_full);
}
