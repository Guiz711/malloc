/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:08:38 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/24 14:55:19 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

short	freelist_replace(t_mzone *zone, t_mfree *old, t_mfree *new)
{
	if (old)
	{
		if (old->prev != NULL)
			old->prev->next = new;
		else
			zone->free = new;
		if(old->next != NULL)
			old->next->prev = new;
	}
	else
		zone->free = new;
	return (SUCCESS);
}

void	freelist_remove(t_mzone *zone, t_mfree *del)
{
	if (del->prev != NULL)
		del->prev->next = del->next;
	else
		zone->free = del->next;
	if (del->next != NULL)
		del->next->prev = del->prev;
}

void	freelist_defrag(t_mfree *link)
{
	t_mblock	*block;
	t_mblock	*next_block;
	t_mblock 	*prev_block;

	block = GET_BLOCK(link, -BLKSZ);
	if (link->next && NEXT_ALLOC(block, block->size) == (void*)link->next)
	{
		next_block = GET_BLOCK(block, block->size + BLKSZ);
		block->size += BLKSZ + next_block->size;
		link->next = link->next->next;
	}
	if (link->prev)
	{
		prev_block = GET_BLOCK(link->prev, -BLKSZ);
	 	if (NEXT_ALLOC(prev_block, prev_block->size) == (void*)link)
		{
			prev_block->size += BLKSZ + block->size;
			link->prev->next = link->next;
		}
	}
}

void	freelist_insert(t_mzone *zone, void *ptr)
{
	t_mblock	*block;
	t_mfree		*new;
	t_mfree		*prev_free;
	t_mfree		*next_free;

	block = GET_BLOCK(ptr, -BLKSZ);
	new = (t_mfree*)ptr;
	block->is_free = 1;
	prev_free = NULL;
	next_free = zone->free;
	while (next_free && next_free < new)
	{
		prev_free = next_free;
		next_free = next_free->next;
	}
	if (prev_free)
		prev_free->next = new;
	if (next_free)
		next_free->prev = new;
	new->prev = prev_free;
	new->next = next_free;
	if (!new->prev)
		zone->free = new;
}

t_mfree	*freelist_init(t_mzone *zone, size_t zone_size)
{
	t_mblock	*block;
	t_mfree		*free_space;

	block = GET_BLOCK(zone, ZONESZ);
	free_space = GET_FREE(block, BLKSZ);
	block->size = zone_size - ZONESZ - BLKSZ;
	block->is_free = 1;
	free_space->next = NULL;
	free_space->prev = NULL;
	return (free_space);
}

t_mfree	*find_free_space(t_mfree *freelist, size_t alloc_size)
{
	t_mblock	*block;

	block = NULL;
	while (freelist != NULL)
	{
		block = GET_BLOCK(freelist, -BLKSZ);
		if (block->size >= alloc_size)
			return (freelist);
		freelist = freelist->next;
	}
	return (NULL);
}
