/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   each_philo_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 16:03:01 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/16 00:58:33 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_right_fork_p(t_each_p *each);
static int	eat_p(t_each_p *each);
static int	sleep_p(t_each_p *each);
static int	think_p(t_each_p *each);

void	*each_p_routine(void *arg_each_p_struct)
{
	t_each_p	*each;

	each = arg_each_p_struct;
	if (each->p_id_num % 2 == 1)
		usleep(1000);
	while (true)
	{
		if (take_left_fork_p(each))
			return (NULL);
		if (take_right_fork_p(each))
			return (NULL);
		if (eat_p(each))
			return (NULL);
		if (sleep_p(each))
			return (NULL);
		if (think_p(each))
			return (NULL);
	}
}

static int	take_right_fork_p(t_each_p *each)
{
	struct timeval	now;

	pthread_mutex_lock(each->right_side_fork);
	pthread_mutex_lock(&each->p_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	if (util_check_fin(each))
	{
		pthread_mutex_unlock(each->right_side_fork);
		pthread_mutex_unlock(&each->p_env->printf_mutex_t);
		return (OTHER_PHILO_DEAD);
	}
	if (check_last_eat(each, now.tv_sec * 1000000 + now.tv_usec))
	{
		pthread_mutex_unlock(each->right_side_fork);
		pthread_mutex_unlock(&each->p_env->printf_mutex_t);
		set_finish_flag_and_put_log(each, now);
		return (OTHER_PHILO_DEAD);
	}
	util_put_log(each, now.tv_sec * 1000000 + now.tv_usec, MAGENTA, PIC_FORK);
	pthread_mutex_unlock(&each->p_env->printf_mutex_t);
	return (OTHER_PHILO_ALIVE);
}



static int	eat_p(t_each_p *each)
{
	struct timeval	now;

	util_put_log(each, now.tv_sec * 1000000 + now.tv_usec, CYAN, PIC_FORK);
	util_put_log(each, now.tv_sec * 1000000 + now.tv_usec, YELLOW, EATING);
	pthread_mutex_lock(&(each->last_eat_mutex_t));
	each->last_eat_time_us = now.tv_sec * 1000000 + now.tv_usec;
	pthread_mutex_unlock(&(each->last_eat_mutex_t));
	pthread_mutex_unlock(&each->p_env->printf_mutex_t);
	util_wait_usleep(now.tv_sec * 1000000 + now.tv_usec, each->p_env->t_t_eat);
	if (++each->eat_count == each->p_env->m_eat)
	{
		pthread_mutex_lock(&(each->p_env->m_eat_mutex_t));
		if (++each->p_env->m_eat_achive_count >= \
			each->p_env->num_of_p)
			each->p_env->finish_flag = true;
		pthread_mutex_unlock(&(each->p_env->m_eat_mutex_t));
	}
	pthread_mutex_unlock(each->left_side_fork);
	pthread_mutex_unlock(each->right_side_fork);
	return (OTHER_PHILO_ALIVE);
}

static int	sleep_p(t_each_p *each)
{
	struct timeval	now;

	if (util_check_fin(each))
		return (OTHER_PHILO_DEAD);
	pthread_mutex_lock(&each->p_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	util_put_log(each, now.tv_sec * 1000000 + now.tv_usec, BLUE, SLEEPING);
	pthread_mutex_unlock(&each->p_env->printf_mutex_t);
	util_wait_usleep(now.tv_sec * 1000000 + now.tv_usec, each->p_env->t_t_sleep);
	return (OTHER_PHILO_ALIVE);
}

static int	think_p(t_each_p *each)
{
	struct timeval	now;

	if (util_check_fin(each))
		return (OTHER_PHILO_DEAD);
	pthread_mutex_lock(&each->p_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	util_put_log(each, now.tv_sec * 1000000 + now.tv_usec, WHITE, THINKING);
	pthread_mutex_unlock(&each->p_env->printf_mutex_t);
	return (OTHER_PHILO_ALIVE);
}
