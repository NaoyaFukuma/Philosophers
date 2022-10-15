/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   each_philo_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 16:03:01 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/15 16:20:30 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_fork_p(t_each_p *each);
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
		if (take_fork_p(each))
			return (NULL);
		if (eat_p(each))
			return (NULL);
		if (sleep_p(each))
			return (NULL);
		if (think_p(each))
			return (NULL);
	}
}

static int	take_fork_p(t_each_p *each)
{
	struct timeval	now;

	printf("\ndebug id %d\n", each->p_id_num);
	pthread_mutex_lock(each->right_side_fork);
	pthread_mutex_lock(&each->p_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	if (check_last_eat(each, now.tv_sec * 1000000 + now.tv_usec))
		set_finish_flag_and_put_log(each, now);
	util_put_log(each, now.tv_sec * 1000000 + now.tv_usec, MAGENTA, PIC_FORK);
	pthread_mutex_unlock(&each->p_env->printf_mutex_t);
	if (each->p_env->num_of_p == 1)
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

static int	eat_p(t_each_p *each)
{
	struct timeval	now;

	pthread_mutex_lock(&each->p_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	if (check_last_eat(each, now.tv_sec * 1000000 + now.tv_usec))
		set_finish_flag_and_put_log(each, now);
	pthread_mutex_lock(&(each->last_eat_mutex_t));
	each->last_eat_time_us = now.tv_sec * 1000000 + now.tv_usec;
	pthread_mutex_unlock(&(each->last_eat_mutex_t));
	util_put_log(each, each->last_eat_time_us, CYAN, PIC_FORK);
	util_put_log(each, each->last_eat_time_us, YELLOW, EATING);
	pthread_mutex_unlock(&each->p_env->printf_mutex_t);
	util_wait_usleep(each->last_eat_time_us, each->p_env->t_t_eat);
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
	long			now_us;

	if (check_last_eat(each, now.tv_sec * 1000000 + now.tv_usec))
		set_finish_flag_and_put_log(each, now);
	pthread_mutex_lock(&each->p_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	now_us = now.tv_sec * 1000000 + now.tv_usec;
	util_put_log(each, now_us, BLUE, SLEEPING);
	pthread_mutex_unlock(&each->p_env->printf_mutex_t);
	util_wait_usleep(now_us, each->p_env->t_t_sleep);
	return (OTHER_PHILO_ALIVE);
}

static int	think_p(t_each_p *each)
{
	struct timeval	now;
	long			now_us;

	if (check_last_eat(each, now.tv_sec * 1000000 + now.tv_usec))
		set_finish_flag_and_put_log(each, now);
	pthread_mutex_lock(&each->p_env->printf_mutex_t);
	gettimeofday(&now, NULL);
	now_us = now.tv_sec * 1000000 + now.tv_usec;
	util_put_log(each, now_us, WHITE, THINKING);
	pthread_mutex_unlock(&each->p_env->printf_mutex_t);
	return (OTHER_PHILO_ALIVE);
}
