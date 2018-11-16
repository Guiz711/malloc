/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/02 18:27:11 by gmichaud          #+#    #+#             */
/*   Updated: 2016/11/15 16:41:34 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr(int n)
{
	if (n < 0)
		ft_putchar('-');
	if (n >= -9 && n <= 9)
	{
		if (n < 0)
			n = -n;
		ft_putchar(n + 48);
	}
	if (n < -9)
	{
		ft_putnbr(n / -10);
		ft_putnbr(-(n % 10));
	}
	if (n > 9)
	{
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	}
}
