/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:21:12 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/07 07:02:31 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Sets is_finished flag
 * 
 * Arguments:
 * - Philo structure
 * 
 * Returns:
 * - Nothing
 */
void	set_is_finished(t_data *data)
{
	pthread_mutex_lock(&data->data_mutex);
	data->is_finished = true;
	pthread_mutex_unlock(&data->data_mutex);
}

/* Sets is_dead flag
 * 
 * Arguments:
 * - Philo structure
 * 
 * Returns:
 * - Nothing
 */
void	set_is_ready(t_data *data)
{
	pthread_mutex_lock(&data->data_mutex);
	data->is_ready = true;
	pthread_mutex_unlock(&data->data_mutex);
}
