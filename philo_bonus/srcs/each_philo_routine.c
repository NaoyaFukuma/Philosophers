/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   each_philo_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 16:03:01 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/28 00:28:05 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	take_fork_philo(t_each_philo *each);
static int	eat_philo(t_each_philo *each);
static int	sleep_philo(t_each_philo *each);
static int	think_philo(t_each_philo *each);

void	*each_philo_routine(void *arg_each_philo_struct)
{
	t_each_philo	*each;

	each = arg_each_philo_struct;
	if (each->philo_id_num % 2 == 0)
		usleep(50);
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

	sem_wait(each->philo_env->fork_sem);
	gettimeofday(&now, NULL);
	util_put_log(each, MAGENTA, now.tv_sec * 1000000 + now.tv_usec, PIC_FORK);
	sem_wait(each->philo_env->fork_sem);
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
		sem_post(each->philo_env->must_eat_achieve_sem);
	sem_post(each->philo_env->fork_sem);
	sem_post(each->philo_env->fork_sem);
	return (OTHER_PHILO_ALIVE);
}

static int	sleep_philo(t_each_philo *each)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	util_put_log(each, BLUE, now.tv_sec * 1000000 + now.tv_usec, SLEEPING);
	util_wait_usleep((now.tv_sec * 1000000 + now.tv_usec),
						each->philo_env->time_to_sleep);
	return (OTHER_PHILO_ALIVE);
}

static int	think_philo(t_each_philo *each)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	util_put_log(each, WHITE, now.tv_sec * 1000000 + now.tv_usec, THINKING);
	return (OTHER_PHILO_ALIVE);
}
