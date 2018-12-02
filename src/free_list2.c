/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_list2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 18:35:25 by gmichaud          #+#    #+#             */
/*   Updated: 2018/12/02 18:36:02 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

t_mfree	*find_free_space(t_mfree *freelist, size_t alloc_size)
{
	t_mblock	*block;

	block = NULL;
	while (freelist != NULL)
	{
		block = GET_BLOCK(freelist, -BLKSZ);
		if (block->size >= alloc_size)
			return (freelist);
		freelist = freelist->next;
	}
	return (NULL);
}
