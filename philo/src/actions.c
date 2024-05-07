/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:52:00 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/07 07:02:09 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Takes two forks, eating, put back forks
 * 
 * Arguments:
 * - Philo structure
 * 
 * Returns:
 * - Nothing
 */
void	eating(t_philo *philo)
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
	if (philo->data->meals_number > 0
		&& philo->meals_counter == philo->data->meals_number)
	{
		pthread_mutex_lock(&philo->philo_mutex);
		philo->is_full = true;
		pthread_mutex_unlock(&philo->philo_mutex);
	}
	pthread_mutex_unlock(&philo->first_fork->fork);
	pthread_mutex_unlock(&philo->second_fork->fork);
}

/* Prints sleep status and waits
 * 
 * Arguments:
 * - Philo structure
 * 
 * Returns:
 * - Nothing
 */
void	sleeping(t_philo *philo)
{
	print_status(SLEEPING, philo);
	usleep_updated(philo->data->time_to_sleep, philo->data);
}

/* Handles thinking action
 * 
 * Arguments:
 * - Philo structure
 * - For_desync - flag for thinking action or desync philosphers
 * 
 * Returns:
 * - Nothing
 */
void	thinking(t_philo *philo, bool for_desync)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!for_desync)
		print_status(THINKING, philo);
	if (philo->data->philo_number % 2 == 0)
		return ;
	t_eat = philo->data->time_to_eat;
	t_sleep = philo->data->time_to_sleep;
	t_think = (t_eat * 2) - t_sleep;
	if (t_think < 0)
		t_think = 0;
	usleep_updated(t_think * 0.42, philo->data);
}
