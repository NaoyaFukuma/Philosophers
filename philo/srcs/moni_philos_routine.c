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

static bool	check_last_eat_time(t_each_philo *each, long now_us);

bool	start_monitar_thread(pthread_t *moni_thread, t_each_philo	*each)
{
	if (pthread_create(moni_thread, NULL,
			moni_philos_routine, each))
		return (true);
	return (false);
}

void	*moni_philos_routine(void *arg_each_philo_structarry)
{
	struct timeval	now;
	long	now_us;
	t_each_philo	*each;
	long			i;

	each = arg_each_philo_structarry;
	i = -1;
	usleep(each->philo_env->time_to_die * 1000 - 2000);
	while (true)
	{
		gettimeofday(&now, NULL);
		now_us = now.tv_sec * 1000000 + now.tv_usec;
		while (++i < each->philo_env->num_of_philo)
		{
			if (util_check_fin(&each[i]))
				return (NULL);
			if (check_last_eat_time(&each[i], now_us))
			{
				pthread_mutex_lock(&(each->philo_env->fin_flag_mutex_t));
				each->philo_env->finish_flag = true;
				pthread_mutex_unlock(&(each->philo_env->fin_flag_mutex_t));
				util_put_log(&each[i], RED, now_us, DIED);
					return (NULL);
			}
		}
		i = -1;
		usleep(7000);
	}
}

static bool	check_last_eat_time(t_each_philo *each, long now_us)
{
	pthread_mutex_lock(&(each->last_eat_mutex_t));
	if ((now_us - each->last_eat_time_us) >= each->philo_env->time_to_die * 1000)
	{
		pthread_mutex_unlock(&(each->last_eat_mutex_t));
		return (true);
	}
	pthread_mutex_unlock(&(each->last_eat_mutex_t));
	return (false);
}