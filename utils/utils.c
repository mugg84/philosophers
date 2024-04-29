/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:38:16 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/29 11:26:02 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static char	*check_input(char *n)
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

void	print_error(char *err)
{
	printf("%s\n", err);
	exit(EXIT_FAILURE);
}