/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   each_philo_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 16:03:01 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/06 10:08:12 by nfukuma          ###   ########.fr       */
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
	if (each->philo_id_num % 2 == 0)
		usleep(10000);
	while (true)
	{
		if (take_fork_philo(each))
			return (NULL);
		if (eat_philo(each))
			return (NULL);
		if (sleep_philo(each))
			return (NULL);
		if (think_philo(each))
			return (NULL);
	}

}

static int	take_fork_philo(t_each_philo *each)
{
	struct timeval	now;

	pthread_mutex_lock(each->right_side_fork);
	if (util_check_fin(each))
	{
		pthread_mutex_unlock(each->right_side_fork);
		return (OTHER_PHILO_DEAD);
	}
	pthread_mutex_lock(&each->philo_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	util_put_log(each, now.tv_sec * 1000000 + now.tv_usec, MAGENTA, PIC_FORK);
	pthread_mutex_unlock(&each->philo_env->printf_mutex_t);
	if (each->philo_env->num_of_philo == 1)
	{
		pthread_mutex_unlock(each->right_side_fork);
		return (OTHER_PHILO_DEAD);
	}
	pthread_mutex_lock(each->left_side_fork);
	if (util_check_fin(each))
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

	pthread_mutex_lock(&each->philo_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	// pthread_mutex_lock(&(each->last_eat_mutex_t));
	each->last_eat_time_us = now.tv_sec * 1000000 + now.tv_usec;
	// pthread_mutex_unlock(&(each->last_eat_mutex_t));
	util_put_log(each, each->last_eat_time_us, CYAN, PIC_FORK);
	util_put_log(each, each->last_eat_time_us, YELLOW, EATING);
	pthread_mutex_unlock(&each->philo_env->printf_mutex_t);
	util_wait_usleep(each->last_eat_time_us, each->philo_env->time_to_eat);
	if (++each->eat_count == each->philo_env->must_eat)
	{
		pthread_mutex_lock(&(each->philo_env->must_eat_mutex_t));
		if (++each->philo_env->must_eat_achive_count
			>= each->philo_env->num_of_philo)
			each->philo_env->finish_flag = true;
		pthread_mutex_unlock(&(each->philo_env->must_eat_mutex_t));
	}
	pthread_mutex_unlock(each->left_side_fork);
	pthread_mutex_unlock(each->right_side_fork);
	return (OTHER_PHILO_ALIVE);
}

static int	sleep_philo(t_each_philo *each)
{
	struct timeval	now;
	long			now_us;

	if (util_check_fin(each))
		return (OTHER_PHILO_DEAD);
	pthread_mutex_lock(&each->philo_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	now_us = now.tv_sec * 1000000 + now.tv_usec;
	util_put_log(each, now_us, BLUE, SLEEPING);
	pthread_mutex_unlock(&each->philo_env->printf_mutex_t);
	util_wait_usleep(now_us, each->philo_env->time_to_sleep);
	return (OTHER_PHILO_ALIVE);
}

static int	think_philo(t_each_philo *each)
{
	struct timeval	now;
	long			now_us;

	if (util_check_fin(each))
		return (OTHER_PHILO_DEAD);
	gettimeofday(&now, NULL);
	now_us = now.tv_sec * 1000000 + now.tv_usec;
	util_put_log(each, now_us, WHITE, THINKING);
	return (OTHER_PHILO_ALIVE);
}
