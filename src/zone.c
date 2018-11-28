/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 10:07:32 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/28 11:42:16 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

size_t	get_big_zone_size(t_mzone *zone)
{
	t_mblock	*block;

	block = GET_BLOCK(zone, ZONESZ);
	return (big_page_nb(block->size) * PAGE_SIZE);
}

size_t	big_page_nb(size_t size)
{
	size_t page_nb;
	size_t total_size;

	total_size = size + BLKSZ + ZONESZ;
	page_nb = total_size / PAGE_SIZE + (total_size % PAGE_SIZE != 0);
	// fprintf(stderr, "%ld + %ld + %ld / %ld + %ld = %ld\n", size, BLKSZ, ZONESZ, PAGE_SIZE, size % PAGE_SIZE != 0, page_nb);
	return (page_nb);
}

t_mzone	*create_zone(size_t size)
{
	t_mzone	*zone;
	void	*zone_ptr;

	zone_ptr = NULL;
	if ((zone_ptr = MMAP(NULL, size)) == MAP_FAILED)
		return (NULL);
	zone = (t_mzone*)zone_ptr;
	zone->free = freelist_init(zone, size);
	zone->next = NULL;
	zone->prev = NULL;
	zone->is_empty = 1;
	t_mblock *block = GET_BLOCK(zone->free, -BLKSZ);
	return (zone);
}

t_mzone	*init_zone(t_mzone **zonelst, size_t pages_nb)
{
	t_mzone	*zone;

	if ((zone = create_zone(PAGE_SIZE * pages_nb)) == NULL)
		return (NULL);
	zonelist_append(zonelst, zone);
	return (zone);
}
