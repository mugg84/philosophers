/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:18:15 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/06 12:38:23 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_b.h"

/* Sets last_meal
 * 
 * Arguments:
 * - philo structure
 * 
 * Returns:
 * - Nothing
 */
void	set_last_meal(t_philo *philo)
{
	sem_wait(philo->data->sem_last_meal);
	philo->last_meal = gettime(MILLISEC);
	sem_post(philo->data->sem_last_meal);
}

/* Sets meal_counter
 * 
 * Arguments:
 * - philo structure
 * 
 * Returns:
 * - Nothing
 */
void	set_meal_counter(t_philo *philo)
{
	sem_wait(philo->data->sem_meal_counter);
	philo->meals_counter++;
	sem_post(philo->data->sem_meal_counter);
}

/* Sets is_finished flag
 * 
 * Arguments:
 * - philo structure
 * 
 * Returns:
 * - Nothing
 */
void	set_is_finished(t_philo *philo)
{
	sem_wait(philo->data->sem_finished);
	philo->is_finished = true;
	sem_post(philo->data->sem_finished);
}

/* Sets is_dead flag
 * 
 * Arguments:
 * - philo structure
 * 
 * Returns:
 * - Nothing
 */
void	set_is_dead(t_philo *philo)
{
	sem_wait(philo->data->sem_last_meal);
	philo->is_dead = true;
	sem_post(philo->data->sem_last_meal);
}
