/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:00 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/15 18:13:23 by nfukuma          ###   ########.fr       */
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
# define BLUE "\e[m"
# define MAGENTA "\e[m"
# define CYAN "\e[m"
# define WHITE "\e[m"

/* flag about is philo died */
# define OTHER_PHILO_ALIVE 0
# define OTHER_PHILO_DEAD 1

typedef struct s_p_env
{
	int				num_of_p;
	int				t_t_die;
	int				t_t_eat;
	int				t_t_sleep;
	int				m_eat;
	int				m_eat_achive_count;
	pthread_mutex_t	m_eat_mutex_t;
	bool			finish_flag;
	pthread_mutex_t	fin_flag_mutex_t;
	pthread_mutex_t	printf_mutex_t;
	long			initial_us;
	pthread_t		*each_p_thread;
}					t_p_env;

typedef struct s_each_p
{
	t_p_env			*p_env;
	int				p_id_num;
	pthread_mutex_t	*right_side_fork;
	pthread_mutex_t	*left_side_fork;
	int				eat_count;
	long			last_eat_time_us;
	long			initial_us;
	pthread_mutex_t	last_eat_mutex_t;
}					t_each_p;

/* each_philo_routine.c */
void				*each_p_routine(void *arg_each_p_struct);

/* moni_philos_routine.c */
bool				start_monitar_thread(pthread_t *moni_thread,
						t_each_p *each);
bool				check_last_eat(t_each_p *each, long now_us);
void				set_finish_flag_and_put_log(t_each_p *each,
						struct timeval now);

/* in util.c */
int					util_atoi(char *str);
void				util_put_log(t_each_p *each, struct timeval now, char *color,
						char *msg);
void				util_wait_usleep(long start_time_us, long wait_time_ms);
bool				util_check_fin(t_each_p *each, struct timeval now);
void				util_all_free(t_each_p *each);

#endif
