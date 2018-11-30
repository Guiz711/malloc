/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:18:32 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/30 14:06:49 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"
#include <limits.h>

// int		main()
// {
// 	char	**arr;
// 	int		i;

// 	i = 0;
// 	arr = (char**)malloc(sizeof(char*) * 200);
// 	while (i < 200)
// 	{
// 		arr[i] = (char*)malloc(1088);
// 		++i;
// 	}
// 	show_alloc_mem_ex(FREE, ZONE_ALL);
// 	i = 199;
// 	while (i >= 10)
// 	{
// 		free(arr[i]);
// 		--i;
// 	}
// 	show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
// 	// free(str2);
// 	// show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
// 	return (0);
// }

int		main()
{
	char *str;
	char *str2;

	str = malloc(100);
	str2 = malloc(200);
	// show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
	// str = ft_rerealloc(str, 150);
	// show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
	free(str2 + 100);
	// show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
	// free(str2);
	// show_alloc_mem_ex(ALLOCS | FREE, ZONE_ALL);
	return (0);
}