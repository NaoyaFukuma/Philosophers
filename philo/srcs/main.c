/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:04 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/27 13:14:36 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static char	*validate_arg(int ac, char **av);
static char	*start_each_philo_threads(t_each_philo *each);
static void	set_struct_philo_env(char **av, t_philo_env *philo_env);
static t_each_philo	*set_struct_each_philo(t_philo_env *philo_env);

int	main(int ac, char **av)
{
	char			*err_msg;
	t_philo_env		philo_env;
	t_each_philo	*each_philo_struct_ptr;
	pthread_t		monitor_philos_thread;

	err_msg = validate_arg(ac, av);
	if (err_msg)
		util_puterrormsg_exit(err_msg);
	set_struct_philo_env(av, &philo_env);
	each_philo_struct_ptr = set_struct_each_philo(&philo_env);
	if (!each_philo_struct_ptr)
		util_puterrormsg_exit("Error: Memory allocation in set each philo struct.");
	if (pthread_create(&monitor_philos_thread, NULL, monitor_philos_routine,
			each_philo_struct_ptr))
		util_puterrormsg_exit("Error: create pthread.");
	err_msg = start_each_philo_threads(each_philo_struct_ptr);
	if (err_msg)
		util_puterrormsg_exit(err_msg);
	if (pthread_join(monitor_philos_thread, NULL))
		util_puterrormsg_exit("Error: join pthread.");
	return (0);
}

static char	*validate_arg(int ac, char **av)
{
	int	tmp_num;

	if (ac < 5 || 6 < ac)
		return ("Error: The number of invalid arguments.\n");
	while (*(++av))
	{
		tmp_num = util_atoi(*av);
		if (tmp_num < 1)
			return ("Error: Invalid argument.\n");
	}
	return (NULL);
}

static char	*start_each_philo_threads(t_each_philo *each)
{
	int			i;
	pthread_t	*each_philo_thread;

	each_philo_thread = malloc(sizeof(pthread_t)
			* each->philo_env->num_of_philo);
	if (!each_philo_thread)
		return ("Error: Memory allocation in pthread_t.");
	i = -1;
	while (++i < each->philo_env->num_of_philo)
		if (pthread_create(&each_philo_thread[i], NULL, each_philo_routine,
				&each[i]))
			return ("Error: create pthread.");
	i = -1;
	while (++i < each->philo_env->num_of_philo)
		if (pthread_join(each_philo_thread[i], NULL))
			return ("Error: join pthread.");
	return (NULL);
}

static void	set_struct_philo_env(char **av, t_philo_env *philo_env)
{
	struct timeval	initial;

	gettimeofday(&initial, NULL);
	philo_env->initial_us = initial.tv_sec * 1000000 + initial.tv_usec;
	philo_env->num_of_philo = util_atoi(av[1]);
	philo_env->time_to_die = util_atoi(av[2]);
	philo_env->time_to_eat = util_atoi(av[3]);
	philo_env->time_to_sleep = util_atoi(av[4]);
	if (av[5])
		philo_env->must_eat = util_atoi(av[5]);
	else
		philo_env->must_eat = 0;
	philo_env->finish_flag = false;
	pthread_mutex_init(&philo_env->printf_mutex_t, NULL);
	pthread_mutex_init(&philo_env->fin_flag_mutex_t, NULL);
}

static t_each_philo	*set_struct_each_philo(t_philo_env *philo_env)
{
	int				i;
	t_each_philo	*each_philo_ptr;
	pthread_mutex_t	*fork_mutexs;

	each_philo_ptr = malloc(sizeof(t_each_philo) * philo_env->num_of_philo);
	fork_mutexs = malloc(sizeof(pthread_mutex_t) * philo_env->num_of_philo);
	if ((!each_philo_ptr) || (!fork_mutexs))
		return (NULL);
	i = -1;
	while (++i < philo_env->num_of_philo)
		pthread_mutex_init(&fork_mutexs[i], NULL);
	i = -1;
	while (++i < philo_env->num_of_philo)
	{
		each_philo_ptr[i].philo_env = philo_env;
		each_philo_ptr[i].philo_id_num = i + 1;
		each_philo_ptr[i].right_side_fork = &fork_mutexs[i];
		each_philo_ptr[i].left_side_fork = &fork_mutexs[(i
				+ philo_env->num_of_philo - 1) % philo_env->num_of_philo];
		each_philo_ptr[i].eat_count = 0;
		each_philo_ptr[i].last_eat_time_us = philo_env->initial_us;
		pthread_mutex_init(&each_philo_ptr[i].last_eat_mutex_t, NULL);
	}
	return (each_philo_ptr);
}
