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
	sem_wait(each->p_env->last_eat_sem);
	if ((now_us - each->last_eat_time_us) >= each->p_env->t_t_die * 1000)
	{
		sem_post(each->p_env->last_eat_sem);
		return (true);
	}
	sem_post(each->p_env->last_eat_sem);
	return (false);
}
