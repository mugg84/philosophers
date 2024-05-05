/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 08:27:57 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/05 13:09:29 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_b.h"

void	parser(char *argv[], t_data **data)
{
	(*data)->philo_number = ft_atol(argv[1]);
	(*data)->time_to_die = ft_atol(argv[2]) * 1000;
	(*data)->time_to_eat = ft_atol(argv[3]) * 1000;
	(*data)->time_to_sleep = ft_atol(argv[4]) * 1000;
	if ((*data)->time_to_die < 6000
		|| (*data)->time_to_eat < 60000 || (*data)->time_to_sleep < 60000)
		print_error("Inputs bigger than 60 milliseconds");
	else if ((*data)->philo_number == 0)
		print_error("At least one philospher needed");
	if (argv[5])
		(*data)->meals_target = ft_atol(argv[5]);
	else
		(*data)->meals_target = -1;
}

void	print_status(int status, t_philo *philo)
{
	long	time;

	time = gettime(MILLISEC) - philo->data->init_time;
	sem_wait(philo->data->sem_print);
	if ((status == FIRST_FORK || status == SECOND_FORK) && !is_dead(philo))
		printf("%6ld %d has taken a fork\n", time, philo->index);
	if (status == EATING && !is_dead(philo))
		printf("%6ld %d is eating\n", time, philo->index);
	else if (status == SLEEPING && !is_dead(philo))
		printf("%6ld %d is sleeping\n", time, philo->index);
	else if (status == THINKING && !is_dead(philo))
		printf("%6ld %d is thinking\n", time, philo->index);
	else if (status == DEAD)
		printf("%6ld %d died\n", time, philo->index);
	sem_post(philo->data->sem_print);
}

void	set_philo(t_philo **philo, t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_number)
	{
		(*philo)[i].data = data;
		(*philo)[i].meals_counter = 0;
		(*philo)[i].last_meal = 0;
		(*philo)[i].index = i + 1;
		(*philo)[i].is_dead = false;
		(*philo)[i].is_finished = false;
		(*philo)[i].meals_target= data->meals_target;
		(*philo)[i].pid = -1;
	}
}
