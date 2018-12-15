/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 11:28:37 by gmichaud          #+#    #+#             */
/*   Updated: 2018/12/14 18:23:31 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

static void	delete_if_needed(t_mzone *zone, t_zone_mask type)
{
	char	*empty_nb;

	empty_nb = type & TINY ? &g_mctrl.empty_tiny : &g_mctrl.empty_small;
	if (type & BIG)
		delete_zone(zone, type);
	else if (is_empty_zone(zone))
	{
		zone->is_empty = 1;
		(*empty_nb)++;
		delete_zone(zone, type);
	}
}

void		free(void *ptr)
{
	t_mblock	*block;
	t_mzone		*zone;
	t_zone_mask	type;

	if (!ptr)
		return ;
	block = GET_BLOCK(ptr, -BLKSZ);
	if (!(zone = zone_search(ptr, &type)) || !block_search(zone, block, type))
		return ;
	if (zone != NULL)
	{
		freelist_insert(zone, ptr);
		freelist_defrag((t_mfree*)ptr);
		delete_if_needed(zone, type);
	}
}
