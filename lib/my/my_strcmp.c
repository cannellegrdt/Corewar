/*
** EPITECH PROJECT, 2024
** MY_STRCMP
** File description:
** Write the my_strcmp function.
*/

int my_strcmp(char const *s1, char const *s2)
{
    while (*s1 && *s2) {
        if (*s1 > *s2)
            return 1;
        if (*s1 < *s2)
            return -1;
        s1++;
        s2++;
    }
    return 0;
}
