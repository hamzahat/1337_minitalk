/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenmoha <hbenmoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 03:15:20 by hbenmoha          #+#    #+#             */
/*   Updated: 2025/03/20 08:22:48 by hbenmoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//! server:
#include "minitalk.h"

//? print server PID
static void	ft_putnbr(int nb)
{
	long	num;

	num = nb;
	if (num > 9)
		ft_putnbr(num / 10);
	write(1, &(char){(num % 10) + 48}, 1);
}

//? handle signals
static void	signal_handler(int signal, siginfo_t *info, void *vd)
{
	static unsigned char	byte;
	static unsigned char	mask = 128;
	static int				pid;

	(void)vd;
	if (info->si_pid != pid)
	{
		pid = info->si_pid;
		mask = 128;
		byte = 0;
	}
	if (signal == SIGUSR2)
		byte |= mask;
	mask >>= 1;
	if (!mask)
	{
		write(1, &byte, 1);
		byte = 0;
		mask = 128;
	}
}

int	main(void)
{
	struct sigaction	s_info;

	ft_putnbr(getpid());
	write(1, "\n", 1);
	s_info.sa_sigaction = signal_handler;
	s_info.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &s_info, 0);
	sigaction(SIGUSR2, &s_info, 0);
	while (1337)
		pause();
	return (0);
}
