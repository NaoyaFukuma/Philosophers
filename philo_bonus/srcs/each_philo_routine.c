/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   each_philo_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 16:03:01 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/13 15:16:52 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	take_fork_p(t_each_p *each);
static int	eat_p(t_each_p *each);
static int	sleep_p(t_each_p *each);
static int	think_p(t_each_p *each);

void	*each_p_routine(void *arg_each_p_struct)
{
	t_each_p	*each;

	each = arg_each_p_struct;
	if (each->p_id_num % 2 == 0)
		usleep(10000);
	while (true)
	{
		if (take_fork_p(each) == OTHER_PHILO_DEAD)
			return (NULL);
		if (eat_p(each) == OTHER_PHILO_DEAD)
			return (NULL);
		if (sleep_p(each) == OTHER_PHILO_DEAD)
			return (NULL);
		if (think_p(each) == OTHER_PHILO_DEAD)
			return (NULL);
	}
}

static int	take_fork_p(t_each_p *each)
{
	struct timeval	now;

	sem_wait(each->p_env->fork_sem);
	gettimeofday(&now, NULL);
	if (util_check_last_eat_time(each, now.tv_sec * 1000000 + now.tv_usec))
		usleep(100000);
	sem_wait(each->p_env->print_sem);
	util_put_log(each, MAGENTA, now.tv_sec * 1000000 + now.tv_usec, PIC_FORK);
	sem_post(each->p_env->print_sem);
	sem_wait(each->p_env->fork_sem);
	gettimeofday(&now, NULL);
	if (util_check_last_eat_time(each, now.tv_sec * 1000000 + now.tv_usec))
		usleep(100000);
	return (OTHER_PHILO_ALIVE);
}

static int	eat_p(t_each_p *each)
{
	struct timeval	now;

	sem_wait(each->p_env->print_sem);
	util_put_log(each, CYAN, now.tv_sec * 1000000 + now.tv_usec, PIC_FORK);
	util_put_log(each, YELLOW, now.tv_sec * 1000000 + now.tv_usec, EATING);
	sem_wait(each->last_eat_sem);
	printf("id %d from each count %ld\n",each->p_id_num, ++(each->count));
	each->last_eat_time_us = now.tv_sec * 1000000 + now.tv_usec;
	sem_post(each->last_eat_sem);
	sem_post(each->p_env->print_sem);
	util_wait_usleep(now.tv_sec * 1000000 + now.tv_usec, each->p_env->t_t_eat);
	if (++each->eat_count == each->p_env->m_eat)
		sem_post(each->p_env->m_eat_ach_sem);
	sem_post(each->p_env->fork_sem);
	sem_post(each->p_env->fork_sem);
	return (OTHER_PHILO_ALIVE);
}

static int	sleep_p(t_each_p *each)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	if (util_check_last_eat_time(each, now.tv_sec * 1000000 + now.tv_usec))
		usleep(100000);
	sem_wait(each->p_env->print_sem);
	util_put_log(each, BLUE, now.tv_sec * 1000000 + now.tv_usec, SLEEPING);
	sem_post(each->p_env->print_sem);
	util_wait_usleep((now.tv_sec * 1000000 + now.tv_usec),
						each->p_env->t_t_sleep);
	return (OTHER_PHILO_ALIVE);
}

static int	think_p(t_each_p *each)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	if (util_check_last_eat_time(each, now.tv_sec * 1000000 + now.tv_usec))
		usleep(100000);
	sem_wait(each->p_env->print_sem);
	util_put_log(each, WHITE, now.tv_sec * 1000000 + now.tv_usec, THINKING);
	sem_post(each->p_env->print_sem);
	return (OTHER_PHILO_ALIVE);
}
