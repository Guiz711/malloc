/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:18:32 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/21 17:38:18 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

int		main()
{
	char *str0 = (char*)ft_malloc(900);
	char *str1 = (char*)ft_malloc(100);
	char *str2 = (char*)ft_malloc(100);
	char *str3 = (char*)ft_malloc(20);
	char *str4 = (char*)ft_malloc(500);
	char *str5 = (char*)ft_malloc(10000);
	char *str6 = (char*)ft_malloc(20000);
	char *str7 = (char*)ft_malloc(57821265600);
	// char *str8 = (char*)malloc(90772524001);
	// printf("%p\n", str8);
	// str7[90772524000] = '\0';
	// for(size_t i = 0; i < 90772524001; ++i)
	// {
	// 	str7[i] = 'a';
	// }
	// ft_putendl(str7);
	// printf("%p\n", str0);
	// printf("%p\n", str1);
	// printf("%p\n", str2);
	// printf("%p\n", str3);
	// printf("%p\n", str4);
	show_alloc_mem();
	// ft_putchar('\n');
	// free(str0);
	// free(str1);
	// show_alloc_mem();
	// ft_putchar('\n');
	// str1 = (char*)ft_malloc(1089);
	// show_alloc_mem();
	
	// printf("%p\n", str1);
	return (0);
}
