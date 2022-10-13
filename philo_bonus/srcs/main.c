/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:04 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/13 13:32:25 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char		*validate_arg(int ac, char **av);
static void		set_struct_p_env(char **av, t_p_env *p_env);
static t_each_p	*set_struct_each_p(t_p_env *p_env);
static bool		exe_each_p_process(t_each_p *each);

int	main(int ac, char **av)
{
	char		*err_msg;
	t_p_env		p_env;
	t_each_p	*each_p_struct;
	pthread_t	moni_thread;

	err_msg = validate_arg(ac, av);
	if (err_msg)
		util_put_error_msg_exit(err_msg);
	set_struct_p_env(av, &p_env);
	each_p_struct = set_struct_each_p(&p_env);
	if (!each_p_struct)
		util_put_error_msg_exit("Error: Memory allocation.");
	if (exe_each_p_process(each_p_struct))
		util_put_error_msg_exit("Error: exe_each_p");
	if (pthread_create(&moni_thread, NULL, moni_m_eat, each_p_struct))
	{
		util_kill_and_wait(p_env.num_of_p, each_p_struct);
		util_put_error_msg_exit("Error: create monitar pthread from main.");
	}
	pthread_join(moni_thread, NULL);
	exit(EXIT_SUCCESS);
}

static char	*validate_arg(int ac, char **av)
{
	int		tmp_num;
	size_t	i;

	if (ac < 5 || 6 < ac)
		return ("Error: The number of invalid arguments.\n");
	i = 0;
	while (av[++i])
	{
		tmp_num = util_atoi(av[i]);
		if ((tmp_num < 1) || ((2 <= i && i <= 4) && tmp_num < 60))
			return ("Error: Invalid argument.\n");
	}
	return (NULL);
}

static void	set_struct_p_env(char **av, t_p_env *p_env)
{
	p_env->num_of_p = util_atoi(av[1]);
	p_env->t_t_die = util_atoi(av[2]);
	p_env->t_t_eat = util_atoi(av[3]);
	p_env->t_t_sleep = util_atoi(av[4]);
	if (av[5])
		p_env->m_eat = util_atoi(av[5]);
	else
		p_env->m_eat = 0;
	sem_unlink(FORK_SEM_NAME);
	p_env->fork_sem = sem_open(FORK_SEM_NAME, O_CREAT, S_IRWXG, p_env->num_of_p);
	sem_unlink(PRINTF_SEM_NAME);
	p_env->print_sem = sem_open(PRINTF_SEM_NAME, O_CREAT, S_IRWXG, 1);
	sem_unlink(LAST_EAT_SEM_NAME);
	p_env->last_eat_sem = sem_open(LAST_EAT_SEM_NAME, O_CREAT, S_IRWXG, 1);
	sem_unlink(M_EAT_ACH_SEM_NAME);
	p_env->m_eat_ach_sem = sem_open(M_EAT_ACH_SEM_NAME, O_CREAT, S_IRWXG, 0);
	p_env->pid_arry = malloc(sizeof(pid_t) * p_env->num_of_p);
	if (!p_env->pid_arry)
		util_put_error_msg_exit("Error: Memory allocation.");
}

static t_each_p	*set_struct_each_p(t_p_env *p_env)
{
	int				i;
	t_each_p		*each;
	pthread_mutex_t	*fork_mutexs;

	each = malloc(sizeof(t_each_p) * p_env->num_of_p);
	fork_mutexs = malloc(sizeof(pthread_mutex_t) * p_env->num_of_p);
	if ((!each) || (!fork_mutexs))
		return (NULL);
	i = -1;
	while (++i < p_env->num_of_p)
		pthread_mutex_init(&fork_mutexs[i], NULL);
	i = -1;
	while (++i < p_env->num_of_p)
	{
		each[i].p_env = p_env;
		each[i].p_id_num = i + 1;
		each[i].eat_count = 0;
	}
	return (each);
}

static bool	exe_each_p_process(t_each_p *each)
{
	int				i;
	pthread_t		each_p_thread;
	struct timeval	initial;

	gettimeofday(&initial, NULL);
	each->p_env->initial_us = initial.tv_sec * 1000000 + initial.tv_usec
		+ 100000;
	i = -1;
	while (++i < each->p_env->num_of_p)
	{
		each->p_env->pid_arry[i] = fork();
		if (each->p_env->pid_arry[i] < 0)
			break ;
		if (each->p_env->pid_arry[i] == 0)
		{
			// sem_wait(each->p_env->last_eat_sem);
			each[i].last_eat_time_us = each->p_env->initial_us;
			// sem_post(each->p_env->last_eat_sem);
			util_wait_usleep(each->p_env->initial_us - 100000, 100);
			if (pthread_create(&each_p_thread, NULL, each_p_routine, &each[i]))
				util_put_error_msg_exit("Error: create pthread.");
			if (moni_ps_routine(&each[i]))
				exit(EXIT_SUCCESS);
		}
	}
	if (i != each->p_env->num_of_p)
	{
		util_kill_and_wait(i, each);
		return (true);
	}
	return (false);
}
