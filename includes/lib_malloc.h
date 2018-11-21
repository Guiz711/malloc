/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_malloc.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 11:25:22 by gmichaud          #+#    #+#             */
/*   Updated: 2018/11/21 16:59:27 by gmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_MALLOC_H
# define LIB_MALLOC_H

# include <sys/mman.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include "libft.h"
# include <limits.h>

/*
** TINY_STEP and SALL_STEP should be a multiple of getpagesize()
*/

# define SUCCESS 1
# define ERROR 0

# define TINY_PAGES_NB 31
# define TINY_STEP 16
# define TINY_MAX_SIZE 1088

# define SMALL_PAGES_NB 560
# define SMALL_STEP 368
# define SMALL_MAX_SIZE 8448
# define PAGE_SIZE getpagesize()
# define MMAP_FLAGS PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS
# define MMAP(p, sz) mmap((p), (sz), MMAP_FLAGS, -1, 0)
# define DECAL(p, step) (void*)(p) + (step)
# define GET_BLOCK(p, step) (t_mblock*)(DECAL((p), (step)))
# define GET_FREE(p, step) (t_mfree*)(DECAL((p), (step)))
# define NEXT_ALLOC(p, size) DECAL((p), (size) + BLKSZ * 2)

# define BLKSZ sizeof(t_mblock)
# define ZONESZ sizeof(t_mzone)

typedef struct		s_mblock
{
	size_t			size;
	char			is_free;
}					t_mblock;

typedef struct		s_mfree
{
	struct s_mfree	*prev;
	struct s_mfree	*next;
}					t_mfree;

typedef struct		s_mzone
{
	t_mfree			*free;
	struct s_mzone	*next;
}					t_mzone;

typedef struct		s_mctrl
{
	t_mzone			*tiny;
	t_mzone			*small;
	t_mzone			*big;
}					t_mctrl;

t_mctrl g_mctrl;

void	*ft_malloc(size_t size);

t_mfree	*find_free_space(t_mfree *freelist, size_t alloc_size);
t_mfree	*freelist_init(t_mzone *zone, size_t zone_size);
void	freelist_remove(t_mzone *zone, t_mfree *del);
short	freelist_replace(t_mzone *zone, t_mfree *old, t_mfree *new);
void	freelist_defrag(t_mfree *link);
void	freelist_insert(t_mzone *zone, void *ptr);

size_t	get_big_zone_size(t_mzone *zone);
size_t	big_page_nb(size_t size);
t_mzone	*create_zone(size_t size);
t_mzone	*init_zone(t_mzone **zonelst, size_t pages_nb);

short	zonelist_append(t_mzone **alst, t_mzone *new);

void	print_free_list(t_mfree *list);

void	ft_puthexa(long n);
void	ft_puthexa_formated(long n, int size);
void	show_alloc_mem();

#endif
