/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:00 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/28 00:28:23 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <errno.h>     // for errno
# include <limits.h>    // for INT_MAX
# include <pthread.h>   // for pthread_create, detach, join,  init, destroy,
	//lock, unlock
# include <semaphore.h> //for sem_open, wait, post, close, unlink
# include <signal.h>    // for kill
# include <stdbool.h>   // for type bool
# include <stdio.h>     //  for printf
# include <stdlib.h>    // for malloc, free
# include <string.h>    // for memset
# include <sys/time.h>  // for gettimeofday
# include <sys/types.h> // for fork, waitpid
# include <unistd.h>    // for write, usleep

/* sem name */
# define FORK_SEM_NAME "FORK_sem"
# define PRINTF_SEM_NAME "PRINTF_sem"
# define MUST_EAT_ACHIEVE_SEM_NAME "MUST_EAT_ACHIEVE_sem"

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
	long			initial_us;
	sem_t			*fork_sem;
	sem_t			*print_sem;
	sem_t			*must_eat_achieve_sem;
}					t_philo_env;

typedef struct s_each_philo
{
	t_philo_env		*philo_env;
	int				philo_id_num;
	int				eat_count;
	pthread_mutex_t	eat_count_mutex_t;
	long			last_eat_time_us;
	pthread_mutex_t	last_eat_mutex_t;
}					t_each_philo;

/* each_philo_routine.c */
void				*each_philo_routine(void *arg_each_philo_struct);

/* monitor_philos_routine.c */
void				*monitor_philos_routine(void *arg);
void				*monitor_must_eat(void *arg);

/* in util.c */
void				util_put_error_msg_exit(char *err_msg);
int					util_atoi(char *str);
void				util_put_log(t_each_philo *each, char *color,
						long time_stamp, char *msg);
void				util_wait_usleep(long start_time_us, long wait_time_ms);

#endif
