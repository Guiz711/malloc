/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 11:28:37 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/30 17:26:49 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

bool	is_empty_zone(t_mzone *zone)
{
	t_mblock	*block;
	t_mfree		*tmp_free;

	block = GET_BLOCK(zone, ZONESZ);
	tmp_free = zone->free;
	while(tmp_free->next)
		tmp_free = tmp_free->next;
	if(block == GET_BLOCK(tmp_free, -BLKSZ))
		return true;
	return false;
}

void	zonelist_remove(t_mzone *zone, t_zone_mask type)
{
	if (zone->prev != NULL)
		zone->prev->next = zone->next;
	else
	{
		if(type & TINY)
			g_mctrl.tiny = zone->next;
		else if(type & SMALL)
			g_mctrl.small = zone->next;
		else
			g_mctrl.big = zone->next;
	}
	if (zone->next != NULL)
		zone->next->prev = zone->prev;
}

void	delete_zone(t_mzone *zone, t_zone_mask type)
{
	size_t	size;
	char	*empty_nb;

	empty_nb = type & TINY ? &g_mctrl.empty_tiny : &g_mctrl.empty_small;
	zonelist_remove(zone, type);
	if(type & BIG)
		size =  get_big_zone_size(zone);
	else if(type & TINY)
		size = PAGE_SIZE * TINY_PAGES_NB;
	else
		size = PAGE_SIZE * SMALL_PAGES_NB;
	munmap(zone, size);
	if(!(type & BIG))
		(*empty_nb)--;
}

void	delete_if_needed(t_mzone *zone, t_zone_mask type)
{
	char	*empty_nb;

	empty_nb = type & TINY ? &g_mctrl.empty_tiny : &g_mctrl.empty_small;
	// printf("%d = %d\n", type, *empty_nb);
	if (type & BIG)
		delete_zone(zone, type);	
	else if (is_empty_zone(zone))
	{
		zone->is_empty = 1;
		(*empty_nb)++;
		// if (*empty_nb >= 1 || (zone->next == NULL && zone->prev == NULL))
		delete_zone(zone, type);
	}
}

t_mzone	*zone_search(t_zone_mask type, void *ptr)
{
	t_mzone	*zone;
	size_t	zone_size;

	if(type & TINY)
	{
		zone = g_mctrl.tiny;
		zone_size = PAGE_SIZE * TINY_PAGES_NB;
	}
	else if(type & SMALL)
	{
		zone = g_mctrl.small;
		zone_size = PAGE_SIZE * SMALL_PAGES_NB;
	}
	else
	{
		zone = g_mctrl.big;
		zone_size = get_big_zone_size(zone);
	}
	while (zone != NULL)
	{
		if(ptr > (void*)zone && ptr <= (void*)zone + zone_size)
			return (zone);
		zone = zone->next;
	}
	return NULL;
}

t_mzone *search_zone(t_mzone *zone, size_t zone_size, void *ptr)
{
	while (zone != NULL)
	{
		if(!zone_size)
			zone_size = get_big_zone_size(zone);
		if(ptr > (void*)zone && ptr <= (void*)zone + zone_size)
			return (zone);
		zone = zone->next;
	}
	return (NULL);
}

t_mzone	*zone_search2(void *ptr, t_zone_mask *type)
{
	t_mzone	*zone;
	size_t	zone_size;

	zone = g_mctrl.tiny;
	zone_size = PAGE_SIZE * TINY_PAGES_NB;
	if ((zone = search_zone(zone, zone_size, ptr)))
		*type = TINY;
	zone = g_mctrl.small;
	zone_size = PAGE_SIZE * SMALL_PAGES_NB;
	if ((zone = search_zone(zone, zone_size, ptr)))
		*type = SMALL;
	zone = g_mctrl.big;
	zone_size = 0;
	if ((zone = search_zone(zone, zone_size, ptr)))
		*type = BIG;
	return (zone);
}

bool	block_search(t_mzone *zone, t_mblock *block, t_zone_mask type)
{
	size_t		zone_size;
	t_mblock	*next_block;
	size_t		size_tot;

	next_block = GET_BLOCK(zone, ZONESZ);
	if(type & TINY)
		zone_size = PAGE_SIZE * TINY_PAGES_NB;
	else if(type & SMALL)
		zone_size = PAGE_SIZE * SMALL_PAGES_NB;
	else
		zone_size = get_big_zone_size(zone);
	size_tot = ZONESZ + BLKSZ;
	while (size_tot < zone_size && (void *)block < (void *)next_block)
	{
		if ((void *)next_block == (void *)block)
			return (true);
		size_tot = next_block->size + BLKSZ;
		next_block = GET_BLOCK(next_block, next_block->size + BLKSZ);
	}
	return (false);
}

void	free(void *ptr)
{
	t_mblock	*block;
	t_mzone		*zone;
	t_zone_mask	type;

	if(!ptr)
		return;
	block = GET_BLOCK(ptr, -BLKSZ);
	if (!(zone = zone_search2(ptr, &type)) || !block_search(zone, block, type))
		return;
	if(zone != NULL)
	{
		freelist_insert(zone, ptr);
		freelist_defrag((t_mfree*)ptr);
		delete_if_needed(zone, type);
	}
}