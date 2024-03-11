/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:38:16 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/11 12:40:05 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_atoi(char *n)
{
	int	res;
	int	sign;

	sign = 1;
	res = 0;
	if (*n == '-')
	{
		sign = -1;
		n++;
	}
	while (*n)
	{
		res += *n++;
		res *= 10;
	}
	return (res * sign);
}