/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:28:32 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/13 10:39:01 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	util_put_error_msg_exit(char *err_msg)
{
	printf("\e[31m%s\e[m\n", err_msg);
	exit(EXIT_FAILURE);
}

void	util_kill_and_wait(int cp_num, t_each_p *each)
{
	int	i;

	i = -1;
	while (++i < cp_num)
	{
		kill(each->p_env->pid_arry[i], SIGINT);
		waitpid(each->p_env->pid_arry[i], NULL, 0);
	}
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
		if (INT_MAX / 10 - (*str - '0') < ret)
			return (-1);
		ret = ret * 10 + (*str - '0');
		str++;
	}
	return ((int)ret);
}

void	util_put_log(t_each_p *each, char *color, long now_us, char *msg)
{
	long	time_stamp;

	if (util_check_last_eat_time(each, now_us) && msg != DIED)
		return ;
	time_stamp = (now_us - each->p_env->initial_us) / 1000;
	printf("%s%ld %d%s\e[m\n", color, time_stamp, each->p_id_num, msg);
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
		if (dest_time_us - 10 <= now_us)
			return ;
	}
}
