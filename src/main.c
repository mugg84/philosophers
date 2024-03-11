/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:26:10 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/11 12:39:55 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*ex(void *a)
{
	(void *)a;
}

void	philosophers(int n_phil, int t_eat, int t_die, int t_sleep, int n_rep)
{
	pthread_t	philosophers[n_phil];
	pthread_mutex_t	fork[n_phil];
	int			i;

	i = 0;
	while (i < n_phil)
	{
		if (pthread_create(philosophers + i, NULL, ex, NULL))
			exit(EXIT_FAILURE);
		i++;
	}
	i = 0;
	while (i < n_phil)
	{
		if (pthread_join(philosophers + i, NULL))
			exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < n_phil)
	{
		if(pthread_mutex_init(fork + i, NULL))
			exit(EXIT_FAILURE);
	}
}

int	main(int argc, char *argv[])
{
	int	rep;
	
	rep = 0;
	if (argc == 6)
		rep = ft_atoi(argv[5]);
	if (argc == 5 || argc == 6)
		philosophers(ft_atoi(argv[1]), ft_atoi(argv[2]), ft_atoi(argv[3]), ft_atoi(argv[4]), rep);
	return (0);
}