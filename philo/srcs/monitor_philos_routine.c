/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philos_routine.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 23:12:52 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/28 00:32:58 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_last_eat_time(t_each_philo *each, struct timeval now);

void	*monitor_philos_routine(void *arg_each_philo_struct_arry)
{
	struct timeval	now;
	t_each_philo	*each;
	long			i;

	each = arg_each_philo_struct_arry;
	i = -1;
	while (true)
	{
		if (++i == each->philo_env->num_of_philo)
			i = 0;
		if (util_check_fin(&each[i]))
			return (NULL);
		gettimeofday(&now, NULL);
		if (check_last_eat_time(&each[i], now))
		{
			pthread_mutex_lock(&(each->philo_env->fin_flag_mutex_t));
			each->philo_env->finish_flag = true;
			pthread_mutex_unlock(&(each->philo_env->fin_flag_mutex_t));
			util_put_log(&each[i], RED, now.tv_sec * 1000000 + now.tv_usec,
				DIED);
			return (NULL);
		}
		usleep(50);
	}
}

static bool	check_last_eat_time(t_each_philo *each, struct timeval now)
{
	pthread_mutex_lock(&(each->last_eat_mutex_t));
	if ((now.tv_sec * 1000000 + now.tv_usec
			- each->last_eat_time_us) >= each->philo_env->time_to_die * 1000)
	{
		pthread_mutex_unlock(&(each->last_eat_mutex_t));
		return (true);
	}
	pthread_mutex_unlock(&(each->last_eat_mutex_t));
	return (false);
}
