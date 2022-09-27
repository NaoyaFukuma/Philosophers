/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moni_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 23:12:52 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/28 00:27:56 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool	check_last_eat_time(t_each_philo *each, struct timeval now);

void	*moni_philos_routine(void *arg)
{
	struct timeval	now;
	t_each_philo	*each;
	long			i;

	each = arg;
	i = -1;
	while (true)
	{
		gettimeofday(&now, NULL);
		if (check_last_eat_time(each, now))
		{
			util_put_log(each, RED, now.tv_sec * 1000000 + now.tv_usec, DIED);
			sem_wait(each->philo_env->print_sem);
			i = -1;
			while (++i < each->philo_env->num_of_philo)
				sem_post(each->philo_env->must_eat_achieve_sem);
			return ("died");
		}
		usleep(50);
	}
}

void	*moni_must_eat(void *arg)
{
	int			i;
	t_philo_env	*philo_env;

	philo_env = arg;
	i = -1;
	while (++i < philo_env->num_of_philo)
		sem_wait(philo_env->must_eat_achieve_sem);
	sem_wait(philo_env->print_sem);
	kill(0, SIGINT);
	return (NULL);
}

static bool	check_last_eat_time(t_each_philo *each, struct timeval now)
{
	pthread_mutex_lock(&(each->last_eat_mutex_t));
	if ((now.tv_sec * 1000000 + now.tv_usec
			- each->last_eat_time_us) >= each->philo_env->time_to_die * 1000)
	{
		pthread_mutex_unlock(&(each->last_eat_mutex_t));
		return (true);
	}
	pthread_mutex_unlock(&(each->last_eat_mutex_t));
	return (false);
}
