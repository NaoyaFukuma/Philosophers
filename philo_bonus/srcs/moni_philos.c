/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moni_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 23:12:52 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/28 00:27:56 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*moni_ps_routine(void *arg)
{
	struct timeval	now;
	t_each_p		*each;
	long			i;

	each = arg;
	i = -1;
	usleep(each->p_env->t_t_die * 1000 - 5000);
	while (true)
	{
		gettimeofday(&now, NULL);
		if (util_check_last_eat_time(each, now.tv_sec * 1000000 + now.tv_usec))
		{
			sem_wait(each->p_env->print_sem);
			while (++i < each->p_env->num_of_p)
				sem_post(each->p_env->m_eat_ach_sem);
			util_put_log(each, RED, now.tv_sec * 1000000 + now.tv_usec, DIED);
			i = -1;
			usleep(2000);
		}
		usleep(700);
	}
}

void	*moni_m_eat(void *arg)
{
	int			i;
	t_each_p	*each;

	each = arg;
	i = -1;
	while (++i < each->p_env->num_of_p)
		sem_wait(each->p_env->m_eat_ach_sem);
	util_kill_and_wait(each->p_env->num_of_p, each);
	return (NULL);
}

bool	util_check_last_eat_time(t_each_p *each, long now_us)
{
	sem_wait(each->last_eat_sem);
	printf("id %d from moni count %ld\n", each->p_id_num, ++(each->count));
	if ((now_us - each->last_eat_time_us) >= each->p_env->t_t_die * 1000)
	{
		sem_post(each->last_eat_sem);
		return (true);
	}
	sem_post(each->last_eat_sem);
	return (false);
}

void	create_thread(t_each_p *each)
{
	pthread_t	each_p_thread;

	sem_wait(each->last_eat_sem);
	each->last_eat_time_us = each->p_env->initial_us;
	sem_post(each->last_eat_sem);
	util_wait_usleep(each->p_env->initial_us - 100000, 100);
	if (pthread_create(&each_p_thread, NULL, each_p_routine, each))
		util_put_error_msg_exit("Error: create pthread.");
	if (moni_ps_routine(each))
		exit(EXIT_SUCCESS);
}
