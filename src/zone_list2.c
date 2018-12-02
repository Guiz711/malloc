/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone_list2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:05:56 by gmichaud          #+#    #+#             */
/*   Updated: 2018/12/02 18:43:57 by gmichaud         ###   ########.fr       */
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

t_mzone *is_contained_in_zone(t_mzone *zone, size_t zone_size, void *ptr)
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

t_mzone	*zone_search(void *ptr, t_zone_mask *type)
{
	t_mzone	*zone;
	size_t	zone_size;

	zone = g_mctrl.tiny;
	zone_size = PAGE_SIZE * TINY_PAGES_NB;
	if ((zone = is_contained_in_zone(zone, zone_size, ptr)))
		*type = TINY;
	zone = g_mctrl.small;
	zone_size = PAGE_SIZE * SMALL_PAGES_NB;
	if ((zone = is_contained_in_zone(zone, zone_size, ptr)))
		*type = SMALL;
	zone = g_mctrl.big;
	zone_size = 0;
	if ((zone = is_contained_in_zone(zone, zone_size, ptr)))
		*type = BIG;
	return (zone);
}
