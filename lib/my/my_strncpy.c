/*
** EPITECH PROJECT, 2024
** MY_STRNCPY
** File description:
** Write a function that copies n characters from a string into another.
*/

char *my_strncpy(char *dest, char *src, unsigned int n)
{
    char *start = dest;

    while (n > 0 && *src) {
        *dest = *src;
        dest++;
        src++;
        n--;
    }
    while (n > 0) {
        *dest = '\0';
        dest++;
        n--;
    }
    *dest = '\0';
    return start;
}
