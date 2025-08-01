/*
** EPITECH PROJECT, 2025
** my_strcpy.c
** File description:
** Copy a string into another
*/

#include "define.h"
#include "error.h"

char *my_strcpy(char *dest, char const *src)
{
    int i = 0;

    if (!dest || !src)
        return err_prog_n(PTR_ERR, ERR_INFO);
    for (; src[i]; i++)
        dest[i] = src[i];
    dest[i] = '\0';
    return dest;
}
