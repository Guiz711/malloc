/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone_list1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 18:37:46 by gmichaud          #+#    #+#             */
/*   Updated: 2018/12/02 18:43:58 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void	zonelist_remove(t_mzone *zone, t_zone_mask type)
{
	if (zone->prev != NULL)
		zone->prev->next = zone->next;
	else
	{
		if(type & TINY)
			g_mctrl.tiny = zone->next;
		else if(type & SMALL)
			g_mctrl.small = zone->next;
		else
			g_mctrl.big = zone->next;
	}
	if (zone->next != NULL)
		zone->next->prev = zone->prev;
}

void	delete_zone(t_mzone *zone, t_zone_mask type)
{
	size_t	size;
	char	*empty_nb;

	empty_nb = type & TINY ? &g_mctrl.empty_tiny : &g_mctrl.empty_small;
	zonelist_remove(zone, type);
	if(type & BIG)
		size =  get_big_zone_size(zone);
	else if(type & TINY)
		size = PAGE_SIZE * TINY_PAGES_NB;
	else
		size = PAGE_SIZE * SMALL_PAGES_NB;
	munmap(zone, size);
	if(!(type & BIG))
		(*empty_nb)--;
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
		new->prev = tmp;
	}
	else
		*alst = new;
	return (SUCCESS);
}
