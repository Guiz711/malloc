/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 10:42:41 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/24 16:12:47 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void	reg_space(t_mzone *zone, t_mfree *free_space, size_t size, size_t step)
{
	t_mblock	*block;
	t_mblock	*new_block;
	t_mfree		*new_free;

	block = GET_BLOCK(free_space, -BLKSZ);
	new_block = NULL;
	new_free = NULL;
	block->is_free = 0;
	if (block->size - size >= BLKSZ + step)
	{
		new_block = GET_BLOCK(block, BLKSZ + size);
		new_block->size = block->size - size - BLKSZ;
		new_block->is_free = 1;
		block->size = size;
		new_free = GET_FREE(new_block, BLKSZ);
		new_free->next = NULL;
		new_free->prev = NULL;
		freelist_replace(zone, free_space, new_free);
	}
	else
		freelist_remove(zone, free_space);
}

size_t	select_step(t_zone_mask mask)
{
	if (mask == TINY)
		return (TINY_STEP);
	else if (mask == SMALL)
		return (SMALL_STEP);
	return (0);
}

void	*alloc_space(t_mzone **zonelst, t_zone_mask mask,
	size_t p_nb, size_t size)
{
	t_mzone *zone;
	t_mfree	*free_space;
	size_t	step;

	step = mask != BIG ? select_step(mask) : size;
	if (size % step != 0)
		size += step - size % step;
	zone = *zonelst;
	free_space = NULL;
	while (zone && !(free_space = find_free_space(zone->free, size)))
		zone = zone->next;
	if (free_space == NULL)
	{
		if ((zone = init_zone(zonelst, p_nb)) == NULL)
			return (NULL);
		free_space = zone->free;
	}
	reg_space(zone, free_space, size, step);
	if (zone->is_empty)
	{
		zone->is_empty = 0;
		if (mask == TINY)
			g_mctrl.empty_tiny++;
		else if (mask == SMALL)
			g_mctrl.empty_small++;
	}
	return ((void*)free_space);
}

void	*ft_malloc(size_t size)
{
	if (size <= TINY_MAX_SIZE)
		return (alloc_space(&(g_mctrl.tiny), TINY, TINY_PAGES_NB, size));
	else if (size <= SMALL_MAX_SIZE)
		return (alloc_space(&(g_mctrl.small), SMALL, SMALL_PAGES_NB, size));
	else if (size > SMALL_MAX_SIZE)
		return (alloc_space(&(g_mctrl.big), BIG, big_page_nb(size), size));
	return (NULL);
}
