/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:48:21 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/30 13:28:15 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
