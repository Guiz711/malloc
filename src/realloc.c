/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 16:19:11 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/30 17:28:58 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

t_zone_mask	find_type(size_t size)
{
	t_zone_mask type;

	if (size <= TINY_MAX_SIZE)
		type = TINY;
	else if (size <= SMALL_MAX_SIZE)
		type = SMALL;
	else
		type = BIG;
	return (type);
}

void        *reduce_block(t_mblock *block, t_mzone *zone, size_t size, t_zone_mask type)
{
	t_mblock    *new_block;
	t_mfree      *new_free;
	size_t  step;
	
	step = type != BIG ? select_step(type) : size;
	if (block->size - size >= BLKSZ * 2)
	{
		new_block = GET_BLOCK(block, BLKSZ + size);
		new_block->size = block->size - size - BLKSZ;
		new_block->is_free = 1;
		block->size = size;
		new_free = GET_FREE(new_block, BLKSZ);
		new_free->next = NULL;
		new_free->prev = NULL;
		freelist_insert(zone, (void *)(new_block + BLKSZ));
	}
	return (DECAL(block, BLKSZ));
}

void        *alloc_and_copy(t_mblock *old_block, size_t size)
{
	char    *new_ptr;
	char    *old_ptr;
	size_t  i;

	i = 0;
	if (!(new_ptr = (char*)malloc(size)))
		return (NULL);	
	old_ptr = (char*)DECAL(old_block, BLKSZ);
	while(i < old_block->size)
	{
		new_ptr[i] = old_ptr[i];
		i++;
	}
	free(old_ptr);
	return((void *)new_ptr);
}

bool        verify_next_block(t_mblock *block, t_mzone *zone, size_t size)
{
	t_mfree     *next_free;
	t_mblock    *next_block;

	next_free = NULL;
	next_block = GET_BLOCK(block, BLKSZ + block->size);
	if (!next_block->is_free)
		return (false);
	if (block->size + next_block->size + BLKSZ >= size)
	{
		next_free = GET_FREE(next_block, BLKSZ);
		freelist_remove(zone, next_free);
		block->size += next_block->size + BLKSZ;
		return (true);
	}
	return (false);
}

bool        is_same_type(t_zone_mask type, size_t size)
{
	t_zone_mask	n_type;
	
	n_type = find_type(size);
	if (n_type == type)
		return (true);
	return (false);
}

static inline void	*resize_block(t_mblock *block, t_mzone *zone, size_t size, t_zone_mask type)
{
	if (size < block->size || verify_next_block(block, zone, size))
		return (reduce_block(block, zone, size, type));
	return (alloc_and_copy(block, size));
}

void        *realloc(void *ptr, size_t size)
{
	t_mblock	*block;
	t_mzone		*zone;
	t_zone_mask	type;

	if(!ptr)
		return (malloc(size));
	if (size <= 0)
	{
		free(ptr);
		return (NULL);
	}
	block = GET_BLOCK(ptr, -BLKSZ);
	if(!(zone = zone_search2(ptr, &type)) || !block_search(zone, block, type))
		return (NULL);
	if (zone->free && (size < block->size || is_same_type(type, size)))
		ptr = resize_block(block, zone, size, type);
	else if (size > block->size)
		ptr = alloc_and_copy(block, size);
	return(ptr);
}