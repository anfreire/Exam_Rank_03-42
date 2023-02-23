# Printf

---


## Functions


### print_d

This function prints a decimal number.

```
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
```

- In the function above, we use the ASCII table to convert the number to a character.
- If the number is negative, we multiply it by -1 and print a '-' before the number.
- Its important to note that the number is converted to a character in the reverse order, so we need to use recursion to print the number in the correct order.
    - For example, if we want to print the number 123, we need to print the number 3 first, then the number 2 and finally the number 1.
    - To achieve this, we identify if the number is greater than 10, if it is, we call the function again with the number divided by 10. (recursion)
    - Then we print the last digit of the number, which is the number % 10.
    - By doing this, we print the number in the correct order.
- Allways remember to increment the lenght variable, because we need to know how many characters were printed.

----

### print_x

This function prints a hexadecimal number.

```
void print_x(long long number, int *lenght)
{
	char hexa[16] = "0123456789abcdef";
	if (number >= 16)
		print_x(number / 16, lenght);
	
	*lenght += write(1, &hexa[number % 16], 1);
}
```

- In the function above, we use the ASCII table to convert the number to a character.
- This function will not work if the number is negative, see more on ft_printf function.
- We use a string to store the hexadecimal numbers.
- Then we use the same logic as the print_d function, but instead of using the ASCII table, we use the string we created to print the hexadecimal number.
- Allways remember to increment the lenght variable, because we need to know how many characters were printed.

---

### print_s

This function prints a string.

```
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
```
- If the string is null we assign the string "(null)" to it, because the printf function prints "(null)" when the string is null.
- Then we print the string with a basic while loop.
- Allways remember to increment the lenght variable, because we need to know how many characters were printed.

----

### ft_printf

Implementation of the printf function.

```
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
```

- We use the va_list to get the arguments passed to the function.
- We start the va_list with va_start(pointer, fmt).
- Then we use a while loop to iterate through the string.
- In the while loop, we check if the current character is a '%', if it is, we increment the fmt pointer and check the next character.
  - If the next character is 's', we call the print_s function with the next argument in the va_list.
  - If the next character is 'd', we call the print_d function with the next argument in the va_list.
  - If the next character is 'x', we call the print_x function with the next argument in the va_list.
- If the current character is not a '%', we print the character with the write function.
- Notice the important use of specifying the type of the argument in the va_arg function. If we don't specify the type, the function will not work properly.
- Its important to sent the lenght variable as a pointer, because we need to know how many characters were printed in the functions, that's why we are able to modify the lenght variable in the functions and the changes will be reflected in the ft_printf function.
- At the end of each loop, we increment the fmt pointer, so we can iterate through the string.
- At the end of the function, we end the va_list with va_end(pointer) and return the lenght variable.
