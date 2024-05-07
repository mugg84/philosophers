/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:38:16 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/07 07:07:29 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Checks if string to be converted is a positive int and skips initial spaces

 * Arguments:
 * - N - string to be converted to long int
 * 
 * Returns:
 * - String of number characters
 */
char	*check_input(char *n)
{
	int		len;
	char	*res;

	len = 0;
	while ((*n >= 9 && *n <= 13) || *n == 32)
		n++;
	if (*n == '+')
		n++;
	else if (*n == '-')
		print_error("Only positive numbers");
	if (!(*n >= '0' && *n <= '9'))
		print_error("Incorrect input");
	res = n;
	while (*n && (*n >= '0' && *n <= '9'))
	{
		n++;
		len++;
	}
	if (len > 10)
		print_error("Number too big");
	return (res);
}

/* Same as atoi(), but for long int

 * Arguments:
 * - N - string to be converted to long int
 * 
 * Returns:
 * - Long int
 */
long	ft_atol(char *n)
{
	long	res;
	char	*num;

	res = 0;
	num = check_input(n);
	while (*num)
	{
		res = res * 10 + (*num - '0');
		num++;
	}
	if (res > INT_MAX)
		print_error("Number too big");
	return (res);
}

/* Closes processes, semaphores and frees structures

 * Arguments:
 * - Data struct
 * - Philo struct
 * 
 * Returns:
 * - Nothing
 */
void	free_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_number)
	{
		pthread_mutex_destroy(&(data->philos + i)->philo_mutex);
		pthread_mutex_destroy(&(data->forks + i)->fork);
	}
	pthread_mutex_destroy(&(data->data_mutex));
	pthread_mutex_destroy(&(data->write_mutex));
	free(data->forks);
	free(data->philos);
}

/* Waits until all the threads are ready

 * Arguments:
 * - Data struct
 * 
 * Returns:
 * - Nothing
 */
void	wait_threads(t_data *data)
{
	while (!wait_is_ready(data))
		;
}

/* Creates all the threads and runs the philo simulation

 * Arguments:
 * - Data struct
 * 
 * Returns:
 * - Nothing
 */
void	create_threads(t_data *d)
{
	int	i;

	i = -1;
	while (++i < d->philo_number)
		pthread_create(&d->philos[i].t_id, NULL,
			run_sim, (void *)&d->philos[i]);
}
