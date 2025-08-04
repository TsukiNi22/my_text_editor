/*
** EPITECH PROJECT, 2025
** insert_array.c
** File description:
** Add a new element to the array at the given index
*/

#include "define.h"
#include "array.h"
#include "error.h"
#include <stdlib.h>
#include <stddef.h>

int insert_array(array_t *array, void *data, unsigned int index)
{
    void **tmp = NULL;

    if (!array || !data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (array->len == array->size) {
        array->size *= 2;
        tmp = malloc(sizeof(void *) * array->size);
        if (!tmp)
            return err_prog(MALLOC_ERR, KO, ERR_INFO);
        for (unsigned int i = 0; i < array->len; i++)
            tmp[i] = array->data[i];
        free(array->data);
        array->data = tmp;
    }
    for (size_t i = array->len; i > index; i--)
        array->data[i] = array->data[i - 1];
    array->data[index] = data;
    array->len++;
    return OK;
}
