/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** my.h
*/

#ifndef MY_H
    #define MY_H
    #include <stddef.h>

int endswith(char *str, char *find);
int my_getnbr(char const *str);
void *my_memset(void *pointer, int value, size_t size);
void my_putnbr(int n);
int my_strcmp(char const *s1, char const *s2);
char *my_strdup(char *str);
int my_strlen(char const *str);
char *my_strncpy(char *dest, char *src, unsigned int n);

#endif //MY_H
