/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   each_philo_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 16:03:01 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/13 01:13:04 by nfukuma          ###   ########.fr       */
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
		usleep(10000);
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
	sem_wait(each->philo_env->print_sem);
	gettimeofday(&now, NULL);
	util_put_log(each, MAGENTA, now.tv_sec * 1000000 + now.tv_usec, PIC_FORK);
	sem_post(each->philo_env->print_sem);
	sem_wait(each->philo_env->fork_sem);
	return (OTHER_PHILO_ALIVE);
}

static int	eat_philo(t_each_philo *each)
{
	struct timeval	now;

	sem_wait(each->philo_env->print_sem);
	gettimeofday(&now, NULL);
	pthread_mutex_lock(&(each->last_eat_mutex_t));
	each->last_eat_time_us = now.tv_sec * 1000000 + now.tv_usec;
	pthread_mutex_unlock(&(each->last_eat_mutex_t));
	util_put_log(each, CYAN, each->last_eat_time_us, PIC_FORK);
	util_put_log(each, YELLOW, each->last_eat_time_us, EATING);
	sem_post(each->philo_env->print_sem);
	util_wait_usleep(each->last_eat_time_us, each->philo_env->time_to_eat);
	if (++each->eat_count == each->philo_env->must_eat)
		sem_post(each->philo_env->must_eat_achieve_sem);
	sem_post(each->philo_env->fork_sem);
	sem_post(each->philo_env->fork_sem);
	return (OTHER_PHILO_ALIVE);
}

static int	sleep_philo(t_each_philo *each)
{
	struct timeval	now;

	sem_wait(each->philo_env->print_sem);
	gettimeofday(&now, NULL);
	util_put_log(each, BLUE, now.tv_sec * 1000000 + now.tv_usec, SLEEPING);
	sem_post(each->philo_env->print_sem);
	util_wait_usleep((now.tv_sec * 1000000 + now.tv_usec),
		each->philo_env->time_to_sleep);
	return (OTHER_PHILO_ALIVE);
}

static int	think_philo(t_each_philo *each)
{
	struct timeval	now;

	sem_wait(each->philo_env->print_sem);
	gettimeofday(&now, NULL);
	util_put_log(each, WHITE, now.tv_sec * 1000000 + now.tv_usec, THINKING);
	sem_post(each->philo_env->print_sem);
	return (OTHER_PHILO_ALIVE);
}
