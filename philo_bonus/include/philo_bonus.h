/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:00 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/13 12:30:02 by nfukuma          ###   ########.fr       */
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
# define FORK_SEM_NAME "/FORK_sem"
# define PRINTF_SEM_NAME "/PRINTF_sem"
# define M_EAT_COUNT_SEM_NAME "/M_EAT_COUNT_sem"
# define LAST_EAT_SEM_NAME "/LAST_EAT_sem"
# define M_EAT_ACH_SEM_NAME "/M_EAT_ACH_sem"

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

typedef struct s_p_env
{
	int		num_of_p;
	int		t_t_die;
	int		t_t_eat;
	int		t_t_sleep;
	int		m_eat;
	long	initial_us;
	sem_t	*fork_sem;
	sem_t	*print_sem;
	sem_t	*m_eat_ach_sem;
	sem_t	*last_eat_sem;
	pid_t	*pid_arry;
}			t_p_env;

typedef struct s_each_p
{
	t_p_env	*p_env;
	int		p_id_num;
	int		eat_count;
	long	last_eat_time_us;
}			t_each_p;

/* each_philo_routine.c */
void		*each_p_routine(void *arg_each_philo_struct);

/* moni_philos_routine.c */
void		*moni_ps_routine(void *arg);
void		*moni_m_eat(void *arg);
bool		util_check_last_eat_time(t_each_p *each, long now_us);

/* in util.c */
void		util_put_error_msg_exit(char *err_msg);
int			util_atoi(char *str);
void		util_put_log(t_each_p *each, char *color, long time_stamp,
				char *msg);
void		util_wait_usleep(long start_time_us, long wait_time_ms);
void		util_kill_and_wait(int cp_num, t_each_p *each);

#endif
