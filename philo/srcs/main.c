/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 22:41:04 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/20 14:13:13 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

struct data
{
	int count;
	pthread_mutex_t mutex;
};


void	*pthread(void *arg)
{
	int	i;
	struct data *data;

	data = arg;
	i = 0;
	while (i++ < 10000)
	{
	pthread_mutex_lock(&data->mutex);
		data->count++;
		// printf("in thread count %d\n", data->count);
	pthread_mutex_unlock(&data->mutex);
	}
	return 0;
}

int	main(int ac, char **av)
{
	pthread_t	pth[2];
	struct data	data;

	pthread_mutex_init(&data.mutex, NULL);
	data.count = 0;
	pthread_create(&pth[0], NULL, pthread, &data);
	pthread_create(&pth[1], NULL, pthread, &data);
	pthread_join(pth[0], NULL);
	pthread_join(pth[1], NULL);
	printf("count %d\n", data.count);
	return 0;
}
