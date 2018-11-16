/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 10:42:41 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/16 18:47:31 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

t_mctrl g_mctrl = { NULL, NULL, NULL };

t_mfree	*init_free_list(t_mzone *zone, size_t zone_size)
{
	t_mblock	*block;
	t_mfree		*free_space;

	block = GET_BLOCK(zone, ZONESZ);
	free_space = GET_FREE(block, BLKSZ);
	block->size = zone_size - ZONESZ - BLKSZ;
	block->is_free = 1;
	free_space->next = NULL;
	free_space->prev = NULL;
	printf("%ld\n", (char*)free_space - (char*)zone);
	return (free_space);
}

short	zonelist_append(t_mzone **alst, t_mzone *new)
{
	t_mzone	*tmp;

	if (!alst)
		return (ERROR);
	tmp = *alst;
	if (tmp)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*alst = new;
	return (SUCCESS);
}

t_mzone	*create_zone(size_t size)
{
	t_mzone	*zone;
	void	*zone_ptr;

	zone_ptr = NULL;
	if ((zone_ptr = MMAP(NULL, size)) == MAP_FAILED)
		return (NULL);
	zone = (t_mzone*)zone_ptr;
	zone->free = init_free_list(zone, size);
	zone->next = NULL;
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

void	print_free_list(t_mfree *list)
{
	if(list == NULL)
	{
		printf("%p\n", list);
	}
	while(list != NULL)
	{
		printf("%p : next = %p; prev = %p\n", list, list->next, list->prev);
		list = list->next;
	}
}

t_mfree	*find_free_space(t_mfree *freelist, size_t alloc_size)
{
	t_mblock	*block;

	block = NULL;
	print_free_list(freelist);
	while (freelist != NULL)
	{
		block = GET_BLOCK(freelist, -BLKSZ);
		if (block->size >= alloc_size)
			return (freelist);
		freelist = freelist->next;
	}
	return (NULL);
}

short	freelist_replace(t_mzone *zone, t_mfree *old, t_mfree *new)
{
	if (old)
	{
		if(old->prev != NULL)
			old->prev->next = new;
		else
		{
			zone->free = new;
		}
		if(old->next != NULL)
			old->next->prev = new;
	}
	else
		zone->free = new;
	return (SUCCESS);
}

void	regi_space(t_mzone *zone, t_mfree *free_space, size_t size, size_t stp)
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
	{
		if(free_space->prev != NULL)
			free_space->prev->next = free_space->next;
		else
			zone->free = free_space->next;
		if(free_space->next != NULL)
			free_space->next->prev = free_space->prev;
	}
}

void	*alloc_space(t_mzone **zonelst, size_t stp, size_t p_nb, size_t size)
{
	t_mzone *zone;
	t_mfree	*free_space;

	if(size % stp != 0)
		size += stp - size % stp;
	zone = *zonelst;
	free_space = NULL;
	while (zone && !(free_space = find_free_space(zone->free, size)))
		zone = zone->next;
	if(free_space == NULL)
	{
		zone = init_zone(zonelst, p_nb);
		free_space = zone->free;
	}
	regi_space(zone, free_space, size, stp);
	return ((void*)free_space);
}

void	*ft_malloc(size_t size)
{
	size_t page_nb;

	if (size <= TINY_MAX_SIZE)
		return (alloc_space(&(g_mctrl.tiny), TINY_STEP, TINY_PAGES_NB, size));
	else if (size <= SMALL_MAX_SIZE)
	{
		return (alloc_space(&(g_mctrl.small), SMALL_STEP,
			SMALL_PAGES_NB, size));
	}
	else if (size > SMALL_MAX_SIZE)
	{
		page_nb = (size + BLKSZ) / PAGE_SIZE + (size % PAGE_SIZE != 0);
		return (alloc_space(&(g_mctrl.big), size, page_nb, size));
	}
	return (NULL);
}

int		main()
{
	char *str1 = (char*)ft_malloc(10000);
	char *str2 = (char*)ft_malloc(10000);
	char *str3 = (char*)ft_malloc(10000);
	char *str4 = (char*)ft_malloc(10000);

	printf("%p\n", str1);
	printf("%p\n", str2);
	printf("%p\n", str3);
	printf("%p\n", str4);
	// printf("%lu %lu\n", sizeof(t_mblock), sizeof(t_mfree));
	printf("%ld\n", (str2 - str1));
	printf("%ld\n", (str3 - str2));
	printf("%ld\n", (str4 - str3));
	return (0);
}