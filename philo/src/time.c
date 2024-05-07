/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:48:21 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/07 07:01:12 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Gets the time

 * Arguments:
 * - Time flag - milliseconds or microsecond
 * 
 * Returns:
 * - Time in format requested on success, EXIT_FAILURE otherwise
 */
long	gettime(int time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		print_error("Gettimeofday error");
	if (time == SECONDS)
		return (tv.tv_sec + (tv.tv_usec / 1000000));
	else if (time == MILLISEC)
		return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
	else if (time == MICROSEC)
		return (tv.tv_sec * 1000000 + (tv.tv_usec));
	else
		print_error("Wrong gettimeofday input");
	return (EXIT_FAILURE);
}

/* More precise versione of function usleep()

 * Arguments:
 * - Usec - time to wait
 * 
 * Returns:
 * - Nothing
 */
void	usleep_updated(long usec, t_data *data)
{
	long	start;
	long	elapsed;
	long	rem;

	start = gettime(MICROSEC);
	while (gettime(MICROSEC) - start < usec)
	{
		if (is_finished(data))
			break ;
		elapsed = gettime(MICROSEC) - start;
		rem = usec - elapsed;
		if (rem > 1000)
			usleep(rem / 2);
		else
		{
			while (gettime(MICROSEC) - start < usec)
				;
		}
	}
}

/* Descynchronize philos, depending on total number and if ID odd or even

 * Arguments:
 * - Philo struct
 * 
 * Returns:
 * - Nothing
 */
void	desynchronize_philos(t_philo *philo)
{
	if (philo->data->philo_number % 2 == 0)
	{
		if (philo->id % 2 == 0)
			usleep_updated(30000, philo->data);
	}
	else
	{
		if (philo->id % 2)
			thinking(philo, true);
	}
}
