/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 10:42:41 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/21 17:23:20 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void	reg_space(t_mzone *zone, t_mfree *free_space, size_t size, size_t stp)
{
	t_mblock	*block;
	t_mblock	*new_block;
	t_mfree		*new_free;

	block = GET_BLOCK(free_space, -BLKSZ);
	new_block = NULL;
	new_free = NULL;
	block->is_free = 0;
	if (block->size - size >= BLKSZ + stp)
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

void	*alloc_space(t_mzone **zonelst, size_t stp, size_t p_nb, size_t size)
{
	t_mzone *zone;
	t_mfree	*free_space;

	if (size % stp != 0)
		size += stp - size % stp;	
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
	reg_space(zone, free_space, size, stp);
	return ((void*)free_space);
}

void	*ft_malloc(size_t size)
{

	if (size <= TINY_MAX_SIZE)
		return (alloc_space(&(g_mctrl.tiny), TINY_STEP, TINY_PAGES_NB, size));
	else if (size <= SMALL_MAX_SIZE)
		return (alloc_space(&(g_mctrl.small), SMALL_STEP,
			SMALL_PAGES_NB, size));
	else if (size > SMALL_MAX_SIZE)
		return (alloc_space(&(g_mctrl.big), size, big_page_nb(size), size));
	return (NULL);
}
