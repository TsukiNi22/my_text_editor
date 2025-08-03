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

// update the col & row of cursor & screen
static int update_pos(editor_t *data, int max_cols, int max_rows)
{
    size_t lines_nb, line_lenght = 0;
    
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // formatage of the cursor's rows
    lines_nb = data->file_map->len;
    if (lines_nb == 0)
        data->cursor_row = 0;
    else if (data->cursor_row > lines_nb - 1)
        data->cursor_row = lines_nb - 1;

    // formatage of the cursor's col
    line_lenght = ((line_t *) data->file_map->data[data->cursor_row])->len;
    if (line_lenght == 0)
        data->cursor_col = 0;
    else if (data->cursor_col > line_lenght - 1 && line_lenght > 0)
        data->cursor_col = line_lenght - 1;
    
    // formatage of the screen's row
    if (data->cursor_row > data->screen_row + (max_rows - 1 - CURSOR_ROW_FROM_BORDER) && data->cursor_row + CURSOR_ROW_FROM_BORDER < lines_nb) {
        if ((int) data->cursor_row >= max_rows - 1 - CURSOR_ROW_FROM_BORDER)
            data->screen_row = data->cursor_row - (max_rows - 1 - CURSOR_ROW_FROM_BORDER);
        else
            data->screen_row = 0;
    }
    if (data->cursor_row < data->screen_row + CURSOR_ROW_FROM_BORDER) {
        if (data->cursor_row >= CURSOR_ROW_FROM_BORDER)
            data->screen_row = data->cursor_row - CURSOR_ROW_FROM_BORDER;
        else
            data->screen_row = 0;
    }

    // formatage of the screen's col
    if (data->cursor_col > data->screen_col + (max_cols - 1 - CURSOR_COL_FROM_BORDER)) {
        if ((int) data->cursor_col >= max_cols - 1 - CURSOR_COL_FROM_BORDER)
            data->screen_col = data->cursor_col - (max_cols - 1 - CURSOR_COL_FROM_BORDER);
        else
            data->screen_col = 0;
    }
    if (data->cursor_col < data->screen_col + CURSOR_COL_FROM_BORDER) {
        if (data->cursor_col >= CURSOR_COL_FROM_BORDER)
            data->screen_col = data->cursor_col - CURSOR_COL_FROM_BORDER;
        else
            data->screen_col = 0;
    }
    return OK;
}

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
    line_t *line = NULL;

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

    // update the col & row of screen & cursor
    if (update_pos(data, max_cols, max_rows) == KO)
        return err_prog_n(UNDEF_ERR, ERR_INFO);

    // setup of the formated line
    for (int i = 0; i + data->screen_row < data->file_map->len && i < max_rows; i++) {
        line = data->file_map->data[i + data->screen_row];
        for (int j = 0; j + data->screen_col < line->len && j < max_cols; j++)
            formated_lines[i][j] = line->content[j + data->screen_col];
    }
    return formated_lines;
}
