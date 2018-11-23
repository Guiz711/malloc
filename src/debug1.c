/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:10:09 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/23 16:25:06 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void	print_allocs(t_mzone *zone, size_t zone_size, t_mode_mask mode)
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
			if (mode == DUMP)
				dump_hexa(alloc, alloc + block->size);
		}
		block = GET_BLOCK(block, block->size + BLKSZ);
	}
}

void	show_zones(t_mzone *zone, size_t zsize, char *type, t_mode_mask mode)
{
	size_t	i;

	i = 0;
	while (zone)
	{
		if (zsize == -1)
			zsize = get_big_zone_size(zone);
		print_zone(type, i++, zone);
		if (mode != FREE)
			print_allocs(zone, zsize, mode);
		else
			print_free_list(zone->free);
		zone = zone->next;
	}
}

void	show_alloc_mode(t_mode_mask mode, t_zone_mask zones_to_show)
{
	if(zones_to_show & TINY)
		show_zones(g_mctrl.tiny, TINY_PAGES_NB * PAGE_SIZE, "TINY", mode);
	if(zones_to_show & SMALL)
		show_zones(g_mctrl.small, SMALL_PAGES_NB * PAGE_SIZE, "SMALL", mode);
	if(zones_to_show & BIG)
		show_zones(g_mctrl.big, -1, "BIG", mode);
}

void	show_alloc_mem_ex(t_mode_mask mode, t_zone_mask zones_to_show)
{
	if(mode & ALLOCS)
	{
		ft_putstr("\n----------------------\n");
		ft_putstr("# Memory Allocations #\n\n");
		show_alloc_mode(mode & ALLOCS, zones_to_show);
	}
	if(mode & FREE)
	{
		ft_putstr("\n----------------------\n");
		ft_putstr("#     Free Lists     #\n\n");
		show_alloc_mode(mode & FREE, zones_to_show);
	}
	if(mode & DUMP)
	{
		ft_putstr("\n----------------------\n");
		ft_putstr("#      Hex Dump      #\n\n");
		show_alloc_mode(mode & DUMP, zones_to_show);
	}
}

void	show_alloc_mem()
{
	show_zones(g_mctrl.tiny, TINY_PAGES_NB * PAGE_SIZE, "TINY", ALLOCS);
	show_zones(g_mctrl.small, SMALL_PAGES_NB * PAGE_SIZE, "SMALL", ALLOCS);
	show_zones(g_mctrl.big, -1, "BIG", ALLOCS);
}
