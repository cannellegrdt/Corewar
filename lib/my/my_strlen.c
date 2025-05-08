/*
** EPITECH PROJECT, 2024
** MY_STRLEN
** File description:
** Write a function that counts the number of characters in a string and
** returns the number found.
*/

int my_strlen(char const *str)
{
    int count = 0;

    if (!str)
        return 0;
    while (*str) {
        str++;
        count++;
    }
    return count;
}
