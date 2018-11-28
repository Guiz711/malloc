/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:18:32 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/28 12:27:48 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"
#include <limits.h>

int		main()
{
	char	**arr;
	int		i;

	i = 0;
	arr = (char**)ft_malloc(sizeof(char*) * 500);
	while (i < 500)
	{
		arr[i] = (char*)ft_malloc(1088);
		++i;
	}
	show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
	while (i > 200)
	{
		ft_free(arr[i]);
		--i;
	}
	// ft_free(str2);
	// show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
	return (0);
}
