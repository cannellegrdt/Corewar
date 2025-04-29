/*
** EPITECH PROJECT, 2024
** MY_GETNBR
** File description:
** Write a function that returns a number, sent to the function as a string.
*/

#include <limits.h>

int my_getnbr(char const *str)
{
    long long result = 0;
    int sign = 1;
    int i = 0;

    while (str[i] == ' ')
        i++;
    if (str[i] == '+' || str[i] == '-') {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        if (result > INT_MAX && sign == 1)
            return INT_MAX;
        if (result > INT_MAX && sign != 1)
            return INT_MIN;
        i++;
    }
    return (int)(result * sign);
}
