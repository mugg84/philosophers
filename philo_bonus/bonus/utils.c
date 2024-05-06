/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 08:23:52 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/06 12:22:40 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_b.h"

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

/* Waits for processes and kills all if one process is_dead flag is true

 * Arguments:
 * - Data struct
 * - Philo struct
 * 
 * Returns:
 * - Nothing
 */
void	kill_processes(t_data *data, t_philo *philo)
{
	int	i;
	int	status;

	i = -1;
	while (++i < data->philo_number)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			i = -1;
			while (++i < data->philo_number)
				kill(philo[i].pid, SIGKILL);
			break ;
		}
	}
}

/* Closes processes, semaphores and frees structures

 * Arguments:
 * - Data struct
 * - Philo struct
 * 
 * Returns:
 * - Nothing
 */
void	free_data(t_data *data, t_philo *philo)
{
	kill_processes(data, philo);
	sem_close(data->sem_finished);
	sem_close(data->sem_print);
	sem_close(data->sem_fork);
	sem_close(data->sem_last_meal);
	sem_close(data->sem_meal_counter);
	sem_unlink("/sem_print");
	sem_unlink("/sem_fork");
	sem_unlink("/sem_finished");
	sem_unlink("/sem_last_meal");
	sem_unlink("/sem_meal_counter");
	free(philo);
	free(data);
}
