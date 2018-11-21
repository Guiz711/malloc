/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:10:09 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/21 17:07:16 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void	print_free_list(t_mfree *list)
{
	if (list == NULL)
	{
		printf("%p\n", list);
	}
	while (list != NULL)
	{
		printf("%p : next = %p; prev = %p\n", list, list->next, list->prev);
		list = list->next;
	}
}

void	ft_puthexa(long n)
{
	const char	*hexa = "0123456789ABCDEF";

	if (n)
	{
		ft_puthexa(n / 16);
		ft_putchar(hexa[n % 16]);
	}
}

void ft_puthexa_formated(long n, int size)
{
	size_t	len;
	long	tmp;

	tmp = n;
	len = 0;
	while (tmp)
	{
		len++;
		tmp /= 16;
	}
	ft_putstr("0x");
	while (size > len)
	{
		ft_putchar('0');
		--size;
	}
	ft_puthexa(n);
}

void	print_zone(char *type, size_t i, void *ptr)
{
	ft_putstr(type);
	ft_putstr(" ");
	ft_putnbr(i);
	ft_putstr(" : ");
	ft_puthexa_formated((long)ptr, 9);
	ft_putchar('\n');
}

void	print_alloc(void *begin, void *end, size_t size)
{
	ft_puthexa_formated((long)begin, 9);
	ft_putstr(" - ");
	ft_puthexa_formated((long)end, 9);
	ft_putstr(" : ");
	ft_putnbrll(size);
	ft_putstr(" | ");
	ft_putnbrll(end - begin);
	ft_putendl(" octets");
}

void	print_allocs(t_mzone *zone, size_t zone_size)
{
	t_mblock	*block;
	void		*zone_end;
	void		*alloc;

	block = GET_BLOCK(zone, BLKSZ);
	zone_end = (void*)zone + zone_size;
	alloc = NULL;
	while ((void*)block < zone_end)
	{
		if (!block->is_free)
		{
			alloc = (void*)block + BLKSZ;
			print_alloc(alloc, alloc + block->size, block->size);
		}
		block = GET_BLOCK(block, block->size + BLKSZ);
	}
}

void	show_zones(t_mzone *zone, size_t zone_size, char *type)
{
	size_t	i;

	i = 0;
	while (zone)
	{
		if(zone_size == -1)
			zone_size = get_big_zone_size(zone);
		print_zone(type, i++, zone);
		print_allocs(zone, zone_size);
		zone = zone->next;
	}
}

void	show_alloc_mem()
{
	show_zones(g_mctrl.tiny, TINY_PAGES_NB * PAGE_SIZE, "TINY");
	show_zones(g_mctrl.small, SMALL_PAGES_NB * PAGE_SIZE, "SMALL");
	show_zones(g_mctrl.big, -1, "BIG");
}