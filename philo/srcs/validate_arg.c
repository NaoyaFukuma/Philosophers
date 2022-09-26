/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:27:20 by nfukuma           #+#    #+#             */
/*   Updated: 2022/09/23 13:53:50 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*validate_arg(int ac, char **av)
{
	int	tmp_num;

	if (ac < 5 || 6 < ac)
		return ("Error: The number of invalid arguments.\n");
	while (*(++av))
	{
		tmp_num = util_atoi(*av);
		if (tmp_num < 1)
			return ("Error: Invalid argument.\n");
	}
	return (NULL);
}
