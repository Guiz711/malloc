/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 18:50:01 by gmichaud          #+#    #+#             */
/*   Updated: 2018/12/08 17:01:35 by gmichaud         ###   ########.fr       */
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

bool		is_same_type(t_zone_mask type, size_t size)
{
	t_zone_mask	n_type;

	n_type = find_type(size);
	if (n_type == type)
		return (true);
	return (false);
}
