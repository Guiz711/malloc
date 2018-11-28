/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:18:32 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/28 16:10:19 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"
#include <limits.h>

int		main()
{
	char	**arr;
	int		i;

	i = 0;
	arr = (char**)ft_malloc(sizeof(char*) * 200);
	while (i < 200)
	{
		arr[i] = (char*)ft_malloc(1088);
		++i;
	}
	show_alloc_mem_ex(FREE, ZONE_ALL);
	i = 199;
	while (i >= 10)
	{
		ft_free(arr[i]);
		--i;
	}
	show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
	// ft_free(str2);
	// show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
	return (0);
}
