/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 16:19:11 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/28 16:40:53 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

t_mode_mask	find_type(size_t size)
{
	t_zone_mask type;

	if (block->size <= TINY_MAX_SIZE)
		type = TINY;
	else if (block->size <= SMALL_MAX_SIZE)
		type = SMALL;
	else
		type = BIG;
}

void	*ft_realloc1(void *ptr, size_t size)
{
	t_mblock	*block;
	t_zone_mask	o_type;
	t_zone_mask	n_type;

	if(!ptr)
		return (ft_malloc(size));
	if(size <= 0)
	{
		ft_free(ptr);
		return (NULL);
	}
	block = GET_BLOCK(ptr, -BLKSZ);
	o_type = find_type(block->size);
	n_type = find_type(size);
	if(o_type != n_type)
		alloc_and_cpy(ptr, size, o_type, n_type);

	zone = NULL;
	if (block->size <= TINY_MAX_SIZE)
		type = TINY;
	else if (block->size <= SMALL_MAX_SIZE)
		type = SMALL;
	else if (block->size > SMALL_MAX_SIZE)
		type = BIG;
	zone = zone_search(type, ptr);
	if(zone != NULL)
	{

	}
	else
		return (NULL);
}