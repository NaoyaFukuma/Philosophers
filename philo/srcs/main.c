/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:04 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/27 09:38:51 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static char	*start_each_philo_threads(t_each_philo *each);

int	main(int ac, char **av)
{
	char			*err_msg;
	t_philo_env		philo_env;
	t_each_philo	*each_philo_struct_ptr;
	pthread_t		monitor_philos_thread;

	err_msg = validate_arg(ac, av);
	if (err_msg)
		util_puterrormsg_exit(err_msg);
	set_struct_philo_env(av, &philo_env);
	each_philo_struct_ptr = set_struct_each_philo(&philo_env);
	if (!each_philo_struct_ptr)
		util_puterrormsg_exit("Error: Memory allocation in set each philo struct.");
	if (pthread_create(&monitor_philos_thread, NULL, monitor_philos_routine,
			each_philo_struct_ptr))
		util_puterrormsg_exit("Error: create pthread.");
	err_msg = start_each_philo_threads(each_philo_struct_ptr);
	if (err_msg)
		util_puterrormsg_exit(err_msg);
	if (pthread_join(monitor_philos_thread, NULL))
		util_puterrormsg_exit("Error: join pthread.");
	return (0);
}

static char	*start_each_philo_threads(t_each_philo *each)
{
	int			i;
	pthread_t	*each_philo_thread;

	each_philo_thread = malloc(sizeof(pthread_t)
			* each->philo_env->num_of_philo);
	if (!each_philo_thread)
		return ("Error: Memory allocation in pthread_t.");
	i = -1;
	while (++i < each->philo_env->num_of_philo)
		if (pthread_create(&each_philo_thread[i], NULL, each_philo_routine,
				&each[i]))
			return ("Error: create pthread.");
	i = -1;
	while (++i < each->philo_env->num_of_philo)
		if (pthread_join(each_philo_thread[i], NULL))
			return ("Error: join pthread.");
	return (NULL);
}
