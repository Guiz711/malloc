/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 18:44:33 by gmichaud          #+#    #+#             */
/*   Updated: 2018/12/02 19:05:26 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

bool	block_search(t_mzone *zone, t_mblock *block, t_zone_mask type)
{
	size_t		zone_size;
	t_mblock	*next_block;
	size_t		size_tot;

	next_block = GET_BLOCK(zone, ZONESZ);
	if (type & TINY)
		zone_size = PAGE_SIZE * TINY_PAGES_NB;
	else if (type & SMALL)
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
