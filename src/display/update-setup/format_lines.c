/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  04/08/2025 by Tsukini

File Name:
##  format_lines.c

File Description:
## Format the line for the ncurses display
\**************************************************************/

#include "memory.h"     // my_malloc_c function
#include "editor.h"     // editor_t type, display defines
#include "error.h"      // error handling
#include <stdlib.h>     // free, malloc function
#include <stddef.h>     // size_t type, NULL define

/* Format lines function
----------------------------------------------------------------
 * Format the lines to be the right size for the ncurses display
----------------------------------------------------------------
##  data -> main data structure
##  max_cols -> maximum number of colomuns
##  max_rows -> maximum number of rows
----------------------------------------------------------------
*/
char **format_lines(editor_t *data, int max_cols, int max_rows)
{
    char **formated_lines = NULL;
    char *line = NULL;

    // Check for potential null pointer
    if (!data)
        return err_prog_n(PTR_ERR, ERR_INFO);

    // setup the array for the formated lines
    formated_lines = malloc(sizeof(char *) * (max_rows + 1));
    if (!formated_lines)
        return err_prog_n(MALLOC_ERR, ERR_INFO);
    for (int i = 0; i < max_rows; i++) {
        if (my_malloc_c(&(formated_lines[i]), max_cols + 1) == KO)
            return err_prog_n(UNDEF_ERR, ERR_INFO);
        for (int j = 0; j < max_cols; j++)
            formated_lines[i][j] = ' ';
        formated_lines[i][max_cols] = '\0';
    }
    formated_lines[max_rows] = NULL;

    // setup of the formated line
    for (int i = 0; i + data->screen_row < data->file_lines->len && i < max_rows; i++) {
        line = data->file_lines->data[i + data->screen_row];
        for (int j = 0; line[j + data->screen_col] && j < max_cols; j++)
            formated_lines[i][j] = line[j + data->screen_col];
    }
    return formated_lines;
}
