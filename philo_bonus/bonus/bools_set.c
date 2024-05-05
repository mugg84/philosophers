/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 08:18:35 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/05 08:47:39 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_b.h"

bool	is_dead(t_philo *philo)
{
	long	time;
	bool	is_dead;

	is_dead =false;
	sem_wait(philo->data->sem_last_meal);
	time = gettime(MILLISEC);
	if (time - philo->last_meal >= philo->data->time_to_die / 1000)
		is_dead=true;
	sem_post(philo->data->sem_last_meal);
	return(is_dead);
}

bool	is_full(t_philo *philo)
{
	bool	is_full;

	is_full =false;
	sem_wait(philo->data->sem_meal_counter);
	if (philo->meals_target >= 0 && philo->meals_target == philo->meals_counter)
		is_full = true;
	sem_post(philo->data->sem_meal_counter);
	return(is_full);
}

void	set_last_meal(t_philo *philo)
{
	sem_wait(philo->data->sem_last_meal);
	philo->last_meal = gettime(MILLISEC);
	sem_post(philo->data->sem_last_meal);
}

void	set_meal_counter(t_philo *philo)
{
	sem_wait(philo->data->sem_meal_counter);
	philo->meals_counter++;
	sem_post(philo->data->sem_meal_counter);
}
