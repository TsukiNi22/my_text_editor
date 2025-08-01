/*
** EPITECH PROJECT, 2025
** char.h
** File description:
** Header for all thing who have a link with str
*/

#ifndef STRING_H
    #define STRING_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include <stdbool.h> // boolean

//----------------------------------------------------------------//
/* PROTOTYPE */

/* edit */
char *my_strcpy(char *dest, char const *src); // Error: NULL
char *my_strncpy(char *dest, char const *src, int n); // Error: NULL
char *my_strcat(char *dest, char const *src); // Error: NULL
char *my_strncat(char *dest, char const *src, int nb); // Error: NULL
char *my_revstr(char *str); // Error: NULL
char *my_strupcase(char *str); // Error: NULL
char *my_strlowcase(char *str); // Error: NULL
char *my_strcapitalize(char *str); // Error: NULL

/* retrive */
int my_strlen(char const *str); // Error: KO
int my_strlen_stop_at(char const *str, char c); // Error: KO
char *my_strstr(char *str, char const *to_find); // Error: NULL
int count_occurence(char const *str, char const c); // Error: KO
int my_strfind(const char *char_array, const char find_char); // Error: -2
int my_get_index(char const *str, char const c); // Error: -2

/* cmp */
int my_strcmp(char const *s1, char const *s2); // Error: 501
int my_strncmp(char const *s1, char const *s2, int n); // Error: 501
bool my_str_isalpha(char const *str); // Error: false
bool my_str_isnum(char const *str); // Error: false
bool my_str_isnumber(char const *str); // Error: false
bool my_str_islower(char const *str); // Error: false
bool my_str_isupper(char const *str); // Error: false
bool my_str_isprintable(char const *str); // Error: false
bool my_str_is(char const *str, char const *char_in); // Error: false

/* transformation */
char *cut_str(const char *char_array, const bool before, int cut_place); // Error: NULL
char *concat_params(char **array); // Error: NULL
char *get_full_path(char const *cr_path, char const *file); // Error: NULL
char **str_to_str_array(char const *str, char const *identifier, bool take); // Error: NULL
char *convertnbr_base(unsigned long long nbr, char const *base); // Error: NULL
char *my_itoa(long long n); // Error: NULL
char *my_ftoa(long double n); // Error: NULL
long long my_atoi(char *char_array); // Error: 0
long double my_atof(char *char_array); // Error: 0

#endif /* STRING_H */
