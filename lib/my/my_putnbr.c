/*
** EPITECH PROJECT, 2024
** MY_PUTNBR
** File description:
** Write a function that displays a number passed in parameter.
** The function must be able to display all possible values in a variable
** of type int.
*/

#include <unistd.h>

void my_putnbr(int n)
{
    int divisor = 1;
    unsigned int num;
    char buf;

    if (n < 0) {
        num = -n;
        write(1, "-", 1);
    } else
        num = n;
    while (num / divisor >= 10)
        divisor *= 10;
    while (divisor > 0) {
        buf = (num / divisor) + '0';
        write(1, &buf, 1);
        num %= divisor;
        divisor /= 10;
    }
}
