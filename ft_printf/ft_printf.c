#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print_d(long long number, int *lenght)
{
	if (number < 0)
	{
		number *= -1;
		*lenght += write(1, "-", 1);
	}
	if (number >= 10)
		print_d(number / 10, lenght);
	char number_in_char = (number % 10) + 48;
	*lenght += write(1, &number_in_char, 1);
}

void print_x(long long number, int *lenght)
{
	char hexa[16] = "0123456789abcdef";
	if (number >= 16)
		print_x(number / 16, lenght);
	
	*lenght += write(1, &hexa[number % 16], 1);
}

void print_s(char *str, int *lenght)
{
	if (!str)
		str = "(null)";
	while (*str)
	{
		*lenght += write(1, *(&str), 1);
		str++;
	}

}

int ft_printf(const char *fmt, ... )
{
	int lenght = 0;
	va_list pointer;
	va_start(pointer, fmt);
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			if (*fmt == 's')
				print_s(va_arg(pointer, char *), &lenght);
			else if (*fmt == 'd')
				print_d(va_arg(pointer, int), &lenght);
			else if (*fmt == 'x')
				print_x(va_arg(pointer, unsigned int), &lenght);
		}
		else
			lenght += write(1, &(*fmt), 1);
		fmt++;
	}
	return (va_end(pointer), lenght);
}