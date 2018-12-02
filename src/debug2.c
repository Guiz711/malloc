/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 16:23:20 by gmichaud          #+#    #+#             */
/*   Updated: 2018/12/02 19:07:11 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void	print_free_list(t_mfree *list)
{
	t_mblock	*block;

	block = NULL;
	if (list == NULL)
	{
		ft_puthexa_formated((unsigned long)0x0, 9, true);
		ft_putchar('\n');
	}
	while (list != NULL)
	{
		block = GET_BLOCK(list, -BLKSZ);
		ft_puthexa_formated((unsigned long)list, 9, true);
		ft_putstr(" : size = ");
		ft_putnbrlu_aligned(block->size, 7);
		ft_putstr(" | prev = ");
		ft_puthexa_formated((unsigned long)list->prev, 9, true);
		ft_putstr("; next = ");
		ft_puthexa_formated((unsigned long)list->next, 9, true);
		ft_putchar('\n');
		list = list->next;
	}
}

void	print_zone(char *type, size_t i, void *ptr)
{
	ft_putstr(type);
	ft_putstr(" ");
	ft_putnbr(i);
	ft_putstr(" : ");
	ft_puthexa_formated((unsigned long)ptr, 9, true);
	ft_putchar('\n');
}

void	print_alloc(void *begin, void *end, size_t size)
{
	ft_puthexa_formated((unsigned long)begin, 9, true);
	ft_putstr(" - ");
	ft_puthexa_formated((unsigned long)end, 9, true);
	ft_putstr(" : ");
	ft_putnbrlu(size);
	ft_putendl(" octets");
}

void	dump_hexa(void *begin, void *end)
{
	char	*hexa;
	size_t	i;

	hexa = NULL;
	while (begin < end)
	{
		hexa = (char*)begin;
		i = 0;
		while (i < BLKSZ)
		{
			ft_puthexa_formated((unsigned char)hexa[i], 2, false);
			ft_putchar(' ');
			++i;
		}
		i = 0;
		while (i < BLKSZ)
		{
			ft_isprint(hexa[i]) ? ft_putchar(hexa[i]) : ft_putchar('.');
			++i;
		}
		ft_putchar('\n');
		begin += 16;
	}
}
