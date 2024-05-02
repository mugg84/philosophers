/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 08:52:26 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/02 14:34:39 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_b/philo_b.h"

void	print_error(char *err)
{
	printf("%s\n", err);
	exit(EXIT_FAILURE);
}

long	gettime(int time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		print_error("Gettimeofday error");
	if (time == MILLISEC)
		return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
	else if (time == MICROSEC)
		return (tv.tv_sec * 1000000 + (tv.tv_usec));
	else
		print_error("Wrong gettimeofday input");
	return (EXIT_FAILURE);
}

void	usleep_updated(long usec)
{
	long	start;
	long	elapsed;
	long	rem;

	start = gettime(MICROSEC);
	while (gettime(MICROSEC) - start < usec)
	{
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

void	set_data(t_data *data)
{
	data->init_time = 0;
	data->index = 0;
	data->last_meal = 0;
	data->meals_number = 0;
	data->is_running = false;
	data->is_finished = false;
	data->is_dead = false;
	data->pid = malloc(sizeof(pid_t) * data->philo_number);
	if (!data->pid)
		print_error("Process malloc error");
	sem_unlink("/sem_print");
	sem_unlink("/sem_fork");
	data->sem_print = sem_open("/sem_print", O_CREAT, 0644, 1);
	data->sem_fork = sem_open("/sem_fork",  O_CREAT, 0644, data->philo_number);
	if (!data->sem_print || !data->sem_fork)
		print_error("Semaphore open error");
}

void	print_status(int status, t_data *data)
{
	long	time;

	time = gettime(MILLISEC) - data->init_time;
	sem_wait(data->sem_print);
	if ((status == FIRST_FORK || status == SECOND_FORK) && !data->is_finished)
		printf("%6ld %d has taken a fork\n", time, data->index);
	if (status == EATING && !data->is_finished)
		printf("%6ld %d is eating\n", time, data->index);
	else if (status == SLEEPING && !data->is_finished)
		printf("%6ld %d is sleeping\n", time, data->index);
	else if (status == THINKING && !data->is_finished)
		printf("%6ld %d is thinking\n", time, data->index);
	else if (status == DEAD && !data->is_finished)
		printf("%6ld %d died\n", time, data->index);
	sem_post(data->sem_print);
}

void	eating(t_data *data)
{
	sem_wait(data->sem_fork);
	print_status(FIRST_FORK, data);
	sem_wait(data->sem_fork);
	print_status(SECOND_FORK, data);
	print_status(EATING, data);
	usleep_updated(data->time_to_eat);
	data->last_meal = gettime(MILLISEC);
	sem_post(data->sem_fork);
	sem_post(data->sem_fork);
	data->meals_number++;
}

void	sleeping(t_data *data)
{
	print_status(SLEEPING, data);
	usleep_updated(data->time_to_sleep);
}

void	thinking(t_data *data)
{
	print_status(THINKING, data);
}

void	*monitor_sim(void *v_data)
{
	t_data	*data;
	long	time;
	
	data = (t_data *)v_data;
	while (!data->is_finished)
	{
		usleep_updated(60);
		time = gettime(MILLISEC);
		if (time - data->last_meal >= data->time_to_die / 1000)
		{
			print_status(DEAD, data);
			data->is_dead = true;
			data->is_finished = true;
			break;
		}
		else if (data->meals_number == data -> meals_target)
		{
			data->is_finished = true;
			break;
		}
	}
	if (data->is_dead)
		exit(1);
	else
		exit(0);
}

void	run_philo_sim(t_data *data)
{
	if (pthread_create(&data->monitor, NULL, &monitor_sim, (void *)data))
		print_error("Monitor thread create error");
	//pthread_detach(data->monitor);
	if (data->index % 2)
		usleep_updated(1000);
	while (1)
	{
		eating(data);
		sleeping(data);
		thinking(data);
	}
	if (pthread_join(data->monitor, NULL))
		print_error("Monitor thread join error");
}

void	init_sim(t_data *data)
{
	int	i;

	i = -1;
	data->init_time = gettime(MILLISEC);
	while (++i < data->philo_number)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			print_error("Fork error");
		if (data->pid[i] == 0)
		{
			data->index = i + 1;
			data->last_meal = gettime(MILLISEC);
			run_philo_sim(data);
		}
	}
}

void	free_data(t_data *data)
{
	int	i;
	int	status;

	i = -1;
	while (++i < data->philo_number)
	{
		waitpid(-1, &status, 0);
		if (status)
		{
			i = -1;
			while (++i < data->philo_number)
				kill(data->pid[i], SIGKILL);
			break;
		}
	}
	sem_close(data->sem_print);
	sem_close(data->sem_fork);
	sem_unlink("/sem_print");
	sem_unlink("/sem_fork");
	free(data->pid);
	free(data);
}

int	main(int argc, char *argv[])
{
	t_data	*data;

	if (argc == 5 || argc == 6)
	{
		data = malloc(sizeof(t_data));
		if (!data)
			print_error("Malloc error");
		parser(argv, &data);
		set_data(data);
		init_sim(data);
		free_data(data);
	}
	else
		print_error("Wrong input");
	return (EXIT_SUCCESS);
}