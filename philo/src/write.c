/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:49:35 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/07 07:00:40 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Prints error and exits the processs

 * Arguments:
 * - Err - error text
 * 
 * Returns:
 * - Nothing
 */
void	print_error(char *err)
{
	printf("%s\n", err);
	exit(EXIT_FAILURE);
}

/* Prints the status provided with timestamp

 * Arguments:
 * - Status
 * - Philo struct
 * 
 * Returns:
 * - Nothing
 */
void	print_status(int status, t_philo *philo)
{
	long	time;

	time = gettime(MILLISEC) - philo->data->init_time;
	if (is_philo_full(philo))
		return ;
	pthread_mutex_lock(&philo->data->write_mutex);
	if ((status == FIRST_FORK || status == SECOND_FORK)
		&& !is_finished(philo->data))
		printf("%6ld %d has taken a fork\n", time, philo->id);
	if (status == EATING && !is_finished(philo->data))
		printf("%6ld %d is eating\n", time, philo->id);
	else if (status == SLEEPING && !is_finished(philo->data))
		printf("%6ld %d is sleeping\n", time, philo->id);
	else if (status == THINKING && !is_finished(philo->data))
		printf("%6ld %d is thinking\n", time, philo->id);
	else if (status == DEAD)
		printf("%6ld %d died\n", time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
}
