/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:04 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/13 16:44:25 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool		validate_arg(int ac, char **av);
static char		*start_each_p_threads(t_each_p *each);
static void		set_struct_p_env(char **av, t_p_env *p_env);
static t_each_p	*set_struct_each_p(t_p_env *p_env);

int	main(int ac, char **av)
{
	char		*err_msg;
	t_p_env		p_env;
	t_each_p	*each_p_struct;
	pthread_t	moni_thread;

	if (validate_arg(ac, av))
		return (1);
	set_struct_p_env(av, &p_env);
	each_p_struct = set_struct_each_p(&p_env);
	if (!each_p_struct)
		printf("\e[31mError: Memory allocation.\e[m");
	else if (start_monitar_thread(&moni_thread, each_p_struct))
		printf("\e[31mError: create pthread.\e[m");
	else
	{
		err_msg = start_each_p_threads(each_p_struct);
		if (err_msg)
			printf("%s", err_msg);
		else if (pthread_join(moni_thread, NULL))
			printf("\e[31mError: join pthread.\e[m");
	}
	util_all_free(each_p_struct);
	return (0);
}

static bool	validate_arg(int ac, char **av)
{
	int		tmp_num;
	size_t	i;

	if (ac < 5 || 6 < ac)
	{
		printf("\e[31mError: The number of invalid arguments.\n\e[m");
		return (true);
	}
	i = 0;
	while (av[++i])
	{
		tmp_num = util_atoi(av[i]);
		if ((tmp_num < 1) || ((2 <= i && i <= 4) && tmp_num < 60))
		{
			printf("\e[31mError: Invalid argument.\n\e[m");
			return (true);
		}
	}
	return (false);
}

static void	set_struct_p_env(char **av, t_p_env *p_env)
{
	struct timeval	initial;

	gettimeofday(&initial, NULL);
	p_env->initial_us = initial.tv_sec * 1000000 + initial.tv_usec;
	p_env->num_of_p = util_atoi(av[1]);
	p_env->t_t_die = util_atoi(av[2]);
	p_env->t_t_eat = util_atoi(av[3]);
	p_env->t_t_sleep = util_atoi(av[4]);
	if (av[5])
		p_env->m_eat = util_atoi(av[5]);
	else
		p_env->m_eat = -1;
	p_env->m_eat_achive_count = 0;
	p_env->finish_flag = false;
	pthread_mutex_init(&p_env->fin_flag_mutex_t, NULL);
	pthread_mutex_init(&p_env->m_eat_mutex_t, NULL);
	pthread_mutex_init(&p_env->printf_mutex_t, NULL);
}

static t_each_p	*set_struct_each_p(t_p_env *p_env)
{
	int				i;
	t_each_p		*each_p_ptr;
	pthread_mutex_t	*fork_mutexs;

	each_p_ptr = malloc(sizeof(t_each_p) * p_env->num_of_p);
	fork_mutexs = malloc(sizeof(pthread_mutex_t) * p_env->num_of_p);
	if ((!each_p_ptr) || (!fork_mutexs))
		return (NULL);
	i = -1;
	while (++i < p_env->num_of_p)
		pthread_mutex_init(&fork_mutexs[i], NULL);
	i = -1;
	while (++i < p_env->num_of_p)
	{
		each_p_ptr[i].p_env = p_env;
		each_p_ptr[i].p_id_num = i + 1;
		each_p_ptr[i].initial_us = p_env->initial_us;
		each_p_ptr[i].right_side_fork = &fork_mutexs[i];
		each_p_ptr[i].left_side_fork = &fork_mutexs[(i + p_env->num_of_p - 1)
			% p_env->num_of_p];
		each_p_ptr[i].eat_count = 0;
		each_p_ptr[i].last_eat_time_us = p_env->initial_us;
		pthread_mutex_init(&each_p_ptr[i].last_eat_mutex_t, NULL);
	}
	return (each_p_ptr);
}

static char	*start_each_p_threads(t_each_p *each)
{
	int	i;

	each->p_env->each_p_thread = malloc(sizeof(pthread_t)
			* each->p_env->num_of_p);
	if (!each->p_env->each_p_thread)
		return ("Error: Memory allocation in pthread_t.");
	i = -1;
	while (++i < each->p_env->num_of_p)
		if (pthread_create(&each->p_env->each_p_thread[i], NULL, each_p_routine,
				&each[i]))
			return ("Error: create pthread.");
	i = -1;
	while (++i < each->p_env->num_of_p)
		if (pthread_join(each->p_env->each_p_thread[i], NULL))
			return ("Error: join pthread.");
	return (NULL);
}
