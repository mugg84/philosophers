/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 08:18:35 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/06 12:20:24 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_b.h"

bool	is_dead(t_philo *philo)
{
	bool	is_dead;

	is_dead = false;
	sem_wait(philo->data->sem_last_meal);
	if (philo->is_dead)
		is_dead = true;
	sem_post(philo->data->sem_last_meal);
	return (is_dead);
}

bool	check_is_dead(t_philo *philo)
{
	long	time;
	bool	is_dead;

	is_dead = false;
	sem_wait(philo->data->sem_last_meal);
	time = gettime(MILLISEC);
	if (time - philo->last_meal >= philo->data->time_to_die / 1000)
		is_dead = true;
	sem_post(philo->data->sem_last_meal);
	return (is_dead);
}

bool	is_full(t_philo *philo)
{
	bool	is_full;

	is_full = false;
	sem_wait(philo->data->sem_meal_counter);
	if (philo->meals_target >= 0 && philo->meals_target == philo->meals_counter)
		is_full = true;
	sem_post(philo->data->sem_meal_counter);
	return (is_full);
}

bool	is_finished(t_philo *philo)
{
	bool	is_finished;

	is_finished = false;
	sem_wait(philo->data->sem_finished);
	if (philo->is_finished)
		is_finished = true;
	sem_post(philo->data->sem_finished);
	return (is_finished);
}
