/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 08:52:26 by mmughedd          #+#    #+#             */
/*   Updated: 2024/05/04 13:43:30 by mmughedd         ###   ########.fr       */
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
	data->is_running = 0;
	data->is_finished = 0;
	data->is_full_counter = data->philo_number;
	sem_unlink("/sem_print");
	sem_unlink("/sem_fork");
	sem_unlink("/sem_finished");
	sem_unlink("/sem_dead");
	data->sem_print = sem_open("/sem_print", O_CREAT, 0644, 1);
	data->sem_fork = sem_open("/sem_fork",  O_CREAT, 0644, data->philo_number);
	data->sem_finished = sem_open("/sem_finished", O_CREAT, 0644, 1);
	data->sem_dead = sem_open("/sem_dead", O_CREAT, 0644, 1);
	data->sem_full= sem_open("/sem_dead", O_CREAT, 0644, data->philo_number); //TODO:
	if (!data->sem_print || !data->sem_fork || !data->sem_finished || !data->sem_dead)
		print_error("Semaphore open error");
}

void	print_status(int status, t_philo *philo)
{
	long	time;

	time = gettime(MILLISEC) - philo->data->init_time;
	sem_wait(philo->data->sem_print);
	if ((status == FIRST_FORK || status == SECOND_FORK))
		printf("%6ld %d has taken a fork\n", time, philo->index);
	if (status == EATING)
		printf("%6ld %d is eating\n", time, philo->index);
	else if (status == SLEEPING)
		printf("%6ld %d is sleeping\n", time, philo->index);
	else if (status == THINKING)
		printf("%6ld %d is thinking\n", time, philo->index);
	else if (status == DEAD)
		printf("%6ld %d died\n", time, philo->index);
	if (status != DEAD || status != ALLFULL)
		sem_post(philo->data->sem_print);
}

void	eating(t_philo *philo)
{
	sem_wait(philo->data->sem_fork);
	print_status(FIRST_FORK, philo);
	sem_wait(philo->data->sem_fork);
	print_status(SECOND_FORK, philo);
	print_status(EATING, philo);
	usleep_updated(philo->data->time_to_eat);
	philo->last_meal = gettime(MILLISEC);
	sem_post(philo->data->sem_fork);
	sem_post(philo->data->sem_fork);
	philo->meals_counter++;
}

void	sleeping(t_philo *philo)
{
	print_status(SLEEPING, philo);
	usleep_updated(philo->data->time_to_sleep);
}

void	thinking(t_philo *philo, bool for_desync)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!for_desync)
		print_status(THINKING, philo);
	if (philo->data->philo_number % 2 == 0)
		return ;
	t_eat = philo->data->time_to_eat;
	t_sleep = philo->data->time_to_sleep;
	t_think = (t_eat * 2) - t_sleep;
	if (t_think < 0)
		t_think = 0;
	usleep_updated(t_think * 0.3);
}

bool	check_sem_full(t_data *data)
{
	bool	all_full;

	all_full = false;
	sem_wait(data->sem_full);
	if (data->is_full_counter == 0)
		all_full = true;
	sem_post(data->sem_full);
	return (all_full);
}
void	*monitor_sim(void *v_philo)
{
	t_philo	*philo;
	long	time;
	
	philo = (t_philo *)v_philo;
	while (1)
	{
		time = gettime(MILLISEC);
		sem_wait(philo->data->sem_dead);
		if (time - philo->last_meal >= philo->data->time_to_die / 1000)
		{
			print_status(DEAD, philo);
			sem_post(philo->data->sem_finished);
			break;
		}
		sem_post(philo->data->sem_dead);
		sem_wait(philo->data->sem_full);
		if (philo->meals_target >= 0 && philo->meals_target == philo->meals_counter)
		{
			philo->data->is_full_counter--;
			printf("%ld\n", philo->data->is_full_counter);
			if (!philo->data->is_full_counter)
			{
				print_status(ALLFULL, philo);
				sem_post(philo->data->sem_finished);
			}
			sem_post(philo->data->sem_full);
			break;
		}
		else
			sem_post(philo->data->sem_full);
	}
	return (NULL);
}

void	desynchronize_philos(t_philo *philo)
{
	if (philo->data->philo_number % 2 == 0)
	{
		if (philo->index % 2 == 0)
			usleep_updated(30000);
	}
	else
	{
		if (philo->index % 2)
			thinking(philo, true);
	}
}

void	run_philo_sim(t_philo *philo)
{
	pthread_t	monitor;

	philo->last_meal = gettime(MILLISEC);
	if (pthread_create(&monitor, NULL, monitor_sim, (void *)philo))
		print_error("Monitor thread create error");
	desynchronize_philos(philo);
	while (1)
	{
		eating(philo);
		sleeping(philo);
		thinking(philo, false);
	}
	pthread_join(monitor, NULL);
}

void	init_sim(t_data *data, t_philo *philo)
{
	int	i;

	i = -1;
	data->init_time = gettime(MILLISEC);
	while (++i < data->philo_number)
	{
		philo[i].pid = fork();
		if (philo[i].pid == -1)
			print_error("Fork error");
		if (philo[i].pid == 0)
			run_philo_sim(&philo[i]);
	}
}

void	free_data(t_data *data, t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < data->philo_number)
	{
		kill(philo[i].pid, SIGKILL);
	}
	sem_close(data->sem_finished);
	sem_close(data->sem_dead);
	sem_close(data->sem_print);
	sem_close(data->sem_fork);
	sem_unlink("/sem_print");
	sem_unlink("/sem_fork");
	sem_unlink("/sem_finished");
	sem_unlink("/sem_dead");
	free(philo);
	free(data);
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
		(*philo)[i].meals_target= data->meals_target;
		(*philo)[i].pid = -1;
	}
}

int	main(int argc, char *argv[])
{
	t_data	*sim;
	t_philo	*philo = NULL;

	if (argc == 5 || argc == 6)
	{
		sim = malloc(sizeof(t_data));
		if (!sim)
			print_error("sim malloc error");
		parser(argv, &sim);
		set_data(sim);
		philo = malloc(sizeof(t_philo) * sim->philo_number);
		if (!philo)
			print_error("Philo malloc error");
		set_philo(&philo, sim);
		sem_wait(sim->sem_finished);
		init_sim(sim, philo);
		sem_wait(sim->sem_finished);
		free_data(sim, philo);
	}
	else
		print_error("Wrong input");
	return (EXIT_SUCCESS);
}