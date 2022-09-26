/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philo_routine.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 23:12:52 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/26 13:28:36 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_philo_routine(void *arg_each_philo_struct_arry)
{
	t_each_philo	*each;
	struct timeval	now;
	long			i;

	each = arg_each_philo_struct_arry;
	i = -1;
	while (true)
	{
		if (++i == each->philo_env->num_of_philo)
			i = 0;
		gettimeofday(&now, NULL);
		if ((now.tv_sec * 1000000 + now.tv_usec
				- each[i].last_eat_time_us) >= each[i].philo_env->time_to_die
			* 1000)
		{
			pthread_mutex_lock(&(each->philo_env->finish_flag_mutex_t));
			each->philo_env->finish_flag = true;
			pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
			gettimeofday(&now, NULL);
			util_put_log(&each[i], RED, now.tv_sec * 1000000 + now.tv_usec,
					DIED);
			return (NULL);
		}
		pthread_mutex_lock(&(each->philo_env->finish_flag_mutex_t));
		if (each->philo_env->finish_flag == true)
		{
			pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
			return (NULL);
		}
		pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
	}
}
