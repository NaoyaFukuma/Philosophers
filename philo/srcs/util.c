/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:28:32 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/29 11:19:00 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	util_atoi(char *str)
{
	long	ret;

	if (*str == '-')
		return (-1);
	if (*str == '+')
		str++;
	ret = 0;
	while (*str)
	{
		if (!('0' <= *str && *str <= '9'))
			return (-1);
		if (INT_MAX / 10 - (*str - '0') < ret)
			return (-1);
		ret = ret * 10 + (*str - '0');
		str++;
	}
	return ((int)ret);
}

void	util_put_log(t_each_philo *each, char *color, long now_us, char *msg)
{
	long	time_stamp;

	time_stamp = (now_us - each->philo_env->initial_us) / 1000;
	printf("%s%ld\t%d%s\e[m\n", color, time_stamp, each->philo_id_num, msg);
	return ;
}

void	util_wait_usleep(long start_time_us, long wait_time_ms)
{
	struct timeval	now;
	long			now_us;
	long			dest_time_us;

	dest_time_us = start_time_us + wait_time_ms * 1000;
	usleep(wait_time_ms * 1000 - 100);
	while (1)
	{
		gettimeofday(&now, NULL);
		now_us = now.tv_sec * 1000000 + now.tv_usec;
		if (dest_time_us - 10 <= now_us)
			break ;
		usleep(5);
	}
	return ;
}

bool	util_check_fin(t_each_philo *each)
{
	pthread_mutex_lock(&(each->philo_env->fin_flag_mutex_t));
	if (each->philo_env->finish_flag == OTHER_PHILO_DEAD)
	{
		pthread_mutex_unlock(&(each->philo_env->fin_flag_mutex_t));
		return (true);
	}
	pthread_mutex_lock(&(each->philo_env->must_eat_mutex_t));
	if (each->philo_env->must_eat_achive_count >= each->philo_env->num_of_philo)
	{
			each->philo_env->finish_flag = true;
		pthread_mutex_unlock(&(each->philo_env->must_eat_mutex_t));
		pthread_mutex_unlock(&(each->philo_env->fin_flag_mutex_t));
		return (true);
	}
	pthread_mutex_unlock(&(each->philo_env->must_eat_mutex_t));
	pthread_mutex_unlock(&(each->philo_env->fin_flag_mutex_t));
	return (false);
}

void	util_all_free(t_each_philo	*each)
{
	free(each->right_side_fork);
	free(each->philo_env->each_philo_thread);
	free(each);
}
