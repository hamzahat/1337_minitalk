/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenmoha <hbenmoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 03:14:53 by hbenmoha          #+#    #+#             */
/*   Updated: 2025/03/20 03:04:00 by hbenmoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//! client:
#include "minitalk.h"

//? send the signal + check failing
static void	ft_kill(pid_t pid, int signal)
{
	if (kill(pid, signal) < 0)
	{
		write(2, "Failed to send signal!\n", 23);
		exit(1);
	}
}

//? check for non numeric characters
static int	is_integer(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		if (str[i] < '0' || str[i++] > '9')
			return (1);
	return (0);
}

//? ft_atoi: Convert a string (PID)
static int	ft_atoi(const char *str, int *check)
{
	long	num;
	int		signe;

	*check = 0;
	signe = 1;
	num = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			signe = -signe;
		str++;
	}
	while (*str <= '9' && *str >= '0')
	{
		num = (num * 10) + (*str++ - 48);
		if ((num * signe) > 2147483647 || (num * signe) < -2147483648)
			return ((*check = 1));
	}
	return (num * signe);
}

//? convert char to bits and send it bit by bit
static void	send_char(char c, pid_t server_pid)
{
	short	mask;

	mask = 128;
	while (mask)
	{
		if (c & mask)
			ft_kill(server_pid, SIGUSR2);
		else
			ft_kill(server_pid, SIGUSR1);
		mask >>= 1;
		usleep(200);
		usleep(200);
	}
}

int	main(int ac, char *av[])
{
	pid_t	server_pid;
	size_t	i;
	int		check;

	i = 0;
	if (ac != 3)
	{
		write(2, "Usage: ./client <PID> <Message>\n", 32);
		return (1);
	}
	server_pid = ft_atoi(av[1], &check);
	if (server_pid <= 0 || check || is_integer(av[1]))
	{
		write(2, "Error: Invalid PID!\n", 20);
		return (1);
	}
	while (av[2][i])
		send_char(av[2][i++], server_pid);
	return (0);
}
