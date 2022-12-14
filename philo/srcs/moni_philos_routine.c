/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moni_philos_routine.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 23:12:52 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/28 01:22:29 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*moni_ps_routine(void *arg);

bool	start_monitar_thread(pthread_t *moni_thread, t_each_p *each)
{
	if (pthread_create(moni_thread, NULL, moni_ps_routine, each))
		return (true);
	return (false);
}

static void	*moni_ps_routine(void *arg)
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
		while (++i < each->p_env->num_of_p)
		{
			if (util_check_fin(&each[i]))
				return (NULL);
			if (check_last_eat(&each[i], now.tv_sec * 1000000 + now.tv_usec))
			{
				pthread_mutex_lock(&each->p_env->printf_mutex_t);
				set_finish_flag_and_put_log(&each[i], now);
				pthread_mutex_unlock(&each->p_env->printf_mutex_t);
				return (NULL);
			}
		}
		i = -1;
		usleep(7000);
	}
}

bool	check_last_eat(t_each_p *each, long now_us)
{
	pthread_mutex_lock(&(each->last_eat_mutex_t));
	if ((now_us - each->last_eat_time_us) >= each->p_env->t_t_die * 1000)
	{
		pthread_mutex_unlock(&(each->last_eat_mutex_t));
		return (true);
	}
	pthread_mutex_unlock(&(each->last_eat_mutex_t));
	return (false);
}

void	set_finish_flag_and_put_log(t_each_p *each, struct timeval now)
{
	pthread_mutex_lock(&(each->p_env->fin_flag_mutex_t));
	each->p_env->finish_flag = true;
	pthread_mutex_unlock(&(each->p_env->fin_flag_mutex_t));
	usleep(1000);
	util_put_log(each, now.tv_sec * 1000000 + now.tv_usec, RED, DIED);
}

int	take_left_fork_p(t_each_p *each)
{
	if (each->p_env->num_of_p == 1)
	{
		pthread_mutex_unlock(each->right_side_fork);
		return (OTHER_PHILO_DEAD);
	}
	pthread_mutex_lock(each->left_side_fork);
	pthread_mutex_lock(&each->p_env->printf_mutex_t);
	if (util_check_fin(each))
	{
		pthread_mutex_unlock(each->left_side_fork);
		pthread_mutex_unlock(each->right_side_fork);
		pthread_mutex_unlock(&each->p_env->printf_mutex_t);
		return (OTHER_PHILO_DEAD);
	}
	return (OTHER_PHILO_ALIVE);
}
