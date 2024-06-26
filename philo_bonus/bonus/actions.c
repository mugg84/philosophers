/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 08:19:02 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/07 07:02:09 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_b.h"

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
	sem_wait(philo->data->sem_fork);
	print_status(FIRST_FORK, philo);
	sem_wait(philo->data->sem_fork);
	print_status(SECOND_FORK, philo);
	print_status(EATING, philo);
	usleep_updated(philo->data->time_to_eat);
	set_last_meal(philo);
	set_meal_counter(philo);
	sem_post(philo->data->sem_fork);
	sem_post(philo->data->sem_fork);
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
	usleep_updated(philo->data->time_to_sleep);
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
	usleep_updated(t_think * 0.3);
}
