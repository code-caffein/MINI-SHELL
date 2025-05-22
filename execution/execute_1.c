/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:48:07 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/22 12:51:38 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_executable_path(char *str, t_malloc **alloc, t_env *env)
{
	int		i;
	char	*s;
	char	**paths;
	char	*check;

	if (!str || !str[0])
		return (NULL);
	s = get_key_value("PATH", env);
	if (!s)
		return (NULL);
	paths = ft_split(s, ':', alloc);
	i = -1;
	while (paths[++i])
	{
		check = ft_strjoin(paths[i], ft_strjoin("/", str, alloc), alloc);
		if (!access(check, X_OK))
			return (check);
	}
	return (NULL);
}