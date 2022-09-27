/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:28:32 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/26 15:52:38 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	util_puterrormsg_exit(char *err_msg)
{
	printf("\e[31m%s\e[m", err_msg);
	exit(EXIT_FAILURE);
}

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
		if (INT_MAX / 10 - (*str - '0')  < ret)
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
	pthread_mutex_lock(&(each->philo_env->print_mutex_t));
	printf("%s%ld %d%s\e[m\n", color, time_stamp, each->philo_id_num, msg);
	pthread_mutex_unlock(&(each->philo_env->print_mutex_t));
	return ;
}

void	util_wait_usleep(long start_time_us, long wait_time_ms)
{
	struct timeval	now;
	long			now_us;
	long			dest_time_us;

	dest_time_us = start_time_us + wait_time_ms * 1000;
	usleep(wait_time_ms * 1000 - 10000);
	while (1)
	{
		usleep(5);
		gettimeofday(&now, NULL);
		now_us = now.tv_sec * 1000000 + now.tv_usec;
		if (dest_time_us - 20 <= now_us)
			break ;
	}
	return ;
}

bool	util_check_finish(t_each_philo *each)
{
	pthread_mutex_lock(&(each->philo_env->finish_flag_mutex_t));
	if (each->philo_env->finish_flag == OTHER_PHILO_DEAD)
	{
		pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
		return (true);
	}
	pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
	return (false);
}