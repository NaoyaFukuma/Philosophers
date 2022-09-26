/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   each_philo_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 16:03:01 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/26 13:17:20 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_fork_philo(t_each_philo *each);
static int	eat_philo(t_each_philo *each);
static int	sleep_philo(t_each_philo *each);
static int	think_philo(t_each_philo *each);

void	*each_philo_routine(void *arg_each_philo_struct)
{
	t_each_philo	*each;

	each = arg_each_philo_struct;
	if (each->philo_id_num % 2 == 1)
		usleep(20);
	while (true)
	{
		if (take_fork_philo(each) == OTHER_PHILO_DEAD)
			return (NULL);
		if (eat_philo(each) == OTHER_PHILO_DEAD)
			return (NULL);
		if (sleep_philo(each) == OTHER_PHILO_DEAD)
			return (NULL);
		if (think_philo(each) == OTHER_PHILO_DEAD)
			return (NULL);
	}
}

static int	take_fork_philo(t_each_philo *each)
{
	struct timeval	now;

	pthread_mutex_lock(each->right_side_fork);
	if (util_check_finish(each))
	{
		pthread_mutex_unlock(each->right_side_fork);
		return (OTHER_PHILO_DEAD);
	}
	gettimeofday(&now, NULL);
	util_put_log(each, MAGENTA, now.tv_sec * 1000000 + now.tv_usec, PIC_FORK);
	if (each->philo_env->num_of_philo == 1)
	{
		pthread_mutex_unlock(each->right_side_fork);
		return (OTHER_PHILO_DEAD);
	}
	pthread_mutex_lock(each->left_side_fork);
	if (util_check_finish(each))
	{
		pthread_mutex_unlock(each->left_side_fork);
		pthread_mutex_unlock(each->right_side_fork);
		return (OTHER_PHILO_DEAD);
	}
	return (OTHER_PHILO_ALIVE);
}

static int	eat_philo(t_each_philo *each)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	each->last_eat_time_us = now.tv_sec * 1000000 + now.tv_usec;
	util_put_log(each, CYAN, now.tv_sec * 1000000 + now.tv_usec, PIC_FORK);
	util_put_log(each, YELLOW, each->last_eat_time_us, EATING);
	util_wait_usleep(each->last_eat_time_us, each->philo_env->time_to_eat);
	each->eat_count++;
	if (each->eat_count == each->philo_env->must_eat)
	{
		pthread_mutex_lock(&(each->philo_env->finish_flag_mutex_t));
		each->philo_env->finish_flag = 1;
		pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
	}
	pthread_mutex_unlock(each->left_side_fork);
	pthread_mutex_unlock(each->right_side_fork);
	return (OTHER_PHILO_ALIVE);
}

static int	sleep_philo(t_each_philo *each)
{
	struct timeval	now;

	pthread_mutex_lock(&(each->philo_env->finish_flag_mutex_t));
	if (each->philo_env->finish_flag == OTHER_PHILO_DEAD)
	{
		pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
		return (OTHER_PHILO_DEAD);
	}
	pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
	gettimeofday(&now, NULL);
	util_put_log(each, BLUE, now.tv_sec * 1000000 + now.tv_usec, SLEEPING);
	util_wait_usleep((now.tv_sec * 1000000 + now.tv_usec),
			each->philo_env->time_to_sleep);
	return (OTHER_PHILO_ALIVE);
}

static int	think_philo(t_each_philo *each)
{
	struct timeval	now;

	pthread_mutex_lock(&(each->philo_env->finish_flag_mutex_t));
	if (each->philo_env->finish_flag == OTHER_PHILO_DEAD)
	{
		pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
		return (OTHER_PHILO_DEAD);
	}
	pthread_mutex_unlock(&(each->philo_env->finish_flag_mutex_t));
	gettimeofday(&now, NULL);
	util_put_log(each, WHITE, now.tv_sec * 1000000 + now.tv_usec, THINKING);
	return (OTHER_PHILO_ALIVE);
}
