/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:04 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/28 00:33:32 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char			*validate_arg(int ac, char **av);
static void			set_struct_philo_env(char **av, t_philo_env *philo_env);
static t_each_philo	*set_struct_each_philo(t_philo_env *philo_env);
static bool			exe_each_philo_process(t_each_philo *each);

int	main(int ac, char **av)
{
	char			*err_msg;
	t_philo_env		philo_env;
	t_each_philo	*each_philo_struct_ptr;
	pthread_t		monitor_philos_thread;

	err_msg = validate_arg(ac, av);
	if (err_msg)
		util_put_error_msg_exit(err_msg);
	set_struct_philo_env(av, &philo_env);
	each_philo_struct_ptr = set_struct_each_philo(&philo_env);
	if (!each_philo_struct_ptr)
		util_put_error_msg_exit("Error: Memory allocation.");
	if (exe_each_philo_process(each_philo_struct_ptr))
		util_put_error_msg_exit("Error: fork");
	if (pthread_create(&monitor_philos_thread, NULL, monitor_must_eat,
			&philo_env))
		util_put_error_msg_exit("Error: create pthread.");
	waitpid(-1, NULL, 0);
	kill(0, SIGINT);
	pthread_join(monitor_philos_thread, NULL);
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
	sem_unlink(FORK_SEM_NAME);
	philo_env->fork_sem = sem_open(FORK_SEM_NAME, O_CREAT, S_IRWXG,
			philo_env->num_of_philo);
	sem_unlink(PRINTF_SEM_NAME);
	philo_env->print_sem = sem_open(PRINTF_SEM_NAME, O_CREAT, S_IRWXG, 1);
	sem_unlink(MUST_EAT_ACHIEVE_SEM_NAME);
	philo_env->must_eat_achieve_sem = sem_open(MUST_EAT_ACHIEVE_SEM_NAME,
												O_CREAT,
												S_IRWXG,
												0);
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
		each_philo_ptr[i].eat_count = 0;
		each_philo_ptr[i].last_eat_time_us = philo_env->initial_us;
		pthread_mutex_init(&each_philo_ptr[i].last_eat_mutex_t, NULL);
		pthread_mutex_init(&each_philo_ptr[i].eat_count_mutex_t, NULL);
	}
	return (each_philo_ptr);
}

static bool	exe_each_philo_process(t_each_philo *each)
{
	int			i;
	pid_t		pid;
	pthread_t	each_philo_thread;

	i = -1;
	while (++i < each->philo_env->num_of_philo)
	{
		pid = fork();
		if (pid < 0)
			return (true);
		if (pid == 0)
		{
			if (pthread_create(&each_philo_thread, NULL, each_philo_routine,
					&each[i]))
				util_put_error_msg_exit("Error: create pthread.");
			if (monitor_philos_routine(&each[i]))
				exit(EXIT_SUCCESS);
		}
	}
	return (false);
}
