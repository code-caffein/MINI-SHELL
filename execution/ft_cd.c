/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:41:24 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/17 18:08:46 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "../includes/mini_lib1.c"

void ft_cd(char *path)
{
	struct stat st;
	if(stat(path, &st) != 0)
	{
		write(2,"sh: cd: ", 8);
		write(2, path, ft_strlen(path));
		write(2,": No such file or directory\n", 28);
	}
	else
		chdir(path);
}


int main(int ac, char  **av)
{
	char s[100];
	// printing current working directory 
	// printf("%s\n", getcwd(s, 100)); 
	// printf("av[1] = %s\n", av[1]);
	ft_cd(av[1]);
	// printing current working directory 
	// printf("%s\n", getcwd(s, 100)); 
}