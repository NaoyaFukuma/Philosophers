/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_struct.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 01:03:39 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/26 16:52:44 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_struct_philo_env(char **av, t_philo_env *philo_env)
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
	pthread_mutex_init(&philo_env->print_mutex_t, NULL);
	pthread_mutex_init(&philo_env->finish_flag_mutex_t, NULL);
}

t_each_philo	*set_struct_each_philo(t_philo_env *philo_env)
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
