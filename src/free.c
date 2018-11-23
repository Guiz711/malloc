/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 11:28:37 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/23 18:31:58 by gmichaud         ###   ########.fr       */
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

void	delete_if_needed(size_t zone_size)
{
	t_mzone	*tmp;
}

t_mzone	*zone_search(t_mzone *zone, size_t zone_size, void *ptr)
{
	while (zone != NULL)
	{
		if(zone_size == -1)
			zone_size = get_big_zone_size(zone);
		if(ptr > (void*)zone && ptr <= (void*)zone + zone_size)
			return (zone);
		zone = zone->next;
	}
	return NULL;
}

void	ft_free(void *ptr)
{
	t_mblock	*block;
	t_mzone		*zone;

	if(!ptr)
		return;
	block = GET_BLOCK(ptr, -BLKSZ);
	zone = NULL;
	if (block->size <= TINY_MAX_SIZE)
		zone = zone_search(g_mctrl.tiny, PAGE_SIZE * TINY_PAGES_NB, ptr);
	else if (block->size <= SMALL_MAX_SIZE)
		zone = zone_search(g_mctrl.small, PAGE_SIZE * SMALL_PAGES_NB, ptr);
	else if (block->size > SMALL_MAX_SIZE)
		zone = zone_search(g_mctrl.big, -1, ptr);
	if(zone != NULL)
	{
		freelist_insert(zone, ptr);
		freelist_defrag((t_mfree*)ptr);
		delete_if_needed(zone);
	}
}