/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:00 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/27 09:39:27 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>   // for INT_MAX
# include <pthread.h>  // for pthread_create, detach, join, init, destroy,
	lock,unlock
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
	bool			finish_flag;
	pthread_mutex_t	finish_flag_mutex_t;
	pthread_mutex_t	print_mutex_t;
	long			initial_us;
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

/* in validate.c */
char				*validate_arg(int ac, char **av);

/* in set_struct.c */
void				set_struct_philo_env(char **av, t_philo_env *philo_env);
t_each_philo		*set_struct_each_philo(t_philo_env *philo_env);

/* each_philo_routine.c */
void				*each_philo_routine(void *arg_each_philo_struct);

/* monitor_philos_routine.c */
void				*monitor_philos_routine(void *arg_each_philo_struct_arry);

/* in util.c */
void				util_puterrormsg_exit(char *err_msg);
int					util_atoi(char *str);
void				util_put_log(t_each_philo *each, char *color,
						long time_stamp, char *msg);
void				util_wait_usleep(long start_time_us, long wait_time_ms);
bool				util_check_finish(t_each_philo *each);

#endif
