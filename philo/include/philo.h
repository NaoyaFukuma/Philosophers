/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:00 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/28 01:51:38 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>   // for INT_MAX
# include <pthread.h>  // for pthread_create, detach, join, init, destroy,
# include <stdbool.h>  // bool
						//lock,unlock
# include <stdbool.h>  // for type bool
# include <stdio.h>    //  for printf
# include <stdlib.h>   // for malloc, free
# include <string.h>   // for memset
# include <sys/time.h> // for gettimeofday
# include <unistd.h>   // for write, usleep

/* log msg */
# define PIC_FORK " has taken a fork"
# define EATING " is eating"
# define SLEEPING " is sleeping"
# define THINKING " is thinking"
# define DIED " died"

/* color */
# define RED "\e[31m"
# define YELLOW "\e[33m"
# define BLUE "\e[34m"
# define MAGENTA "\e[35m"
# define CYAN "\e[36m"
# define WHITE "\e[m"

/* flag about is philo died */
# define OTHER_PHILO_ALIVE 0
# define OTHER_PHILO_DEAD 1

typedef struct s_philo_env
{
	int				num_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				must_eat_achive_count;
	pthread_mutex_t	must_eat_mutex_t;
	bool			finish_flag;
	pthread_mutex_t	fin_flag_mutex_t;
	pthread_mutex_t	printf_mutex_t;
	long			initial_us;
	pthread_t		*each_philo_thread;
}					t_philo_env;

typedef struct s_each_philo
{
	t_philo_env		*philo_env;
	int				philo_id_num;
	pthread_mutex_t	*right_side_fork;
	pthread_mutex_t	*left_side_fork;
	int				eat_count;
	long			last_eat_time_us;
	pthread_mutex_t	last_eat_mutex_t;
}					t_each_philo;

/* each_philo_routine.c */
void				*each_philo_routine(void *arg_each_philo_struct);

/* moni_philos_routine.c */
void				*moni_philos_routine(void *arg_each_philo_structarry);
bool				start_monitar_thread(pthread_t *moni_thread,
		t_each_philo *each);

/* in util.c */
int					util_atoi(char *str);
void				util_put_log(t_each_philo *each, char *color,
		long time_stamp, char *msg);
void				util_wait_usleep(long start_time_us, long wait_time_ms);
bool				util_check_fin(t_each_philo *each);
void				util_all_free(t_each_philo	*each);


#endif
