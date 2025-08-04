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
##  update_pos.c

File Description:
##  Update the row/col of the cursor & screen
\**************************************************************/

#include "editor.h"     // editor_t type, get_file function
#include "error.h"      // error handling
#include <stdlib.h>     // free function
#include <stddef.h>     // size_t type, NULL define

/* Update pos function
----------------------------------------------------------------
 * Format/Update the row & col of the cursor & screen
 * to limits them in the limits
----------------------------------------------------------------
##  data -> main data structure
##  max_cols -> maximum number of colomuns
##  max_rows -> maximum number of rows
----------------------------------------------------------------
*/
int update_pos(editor_t *data, int max_cols, int max_rows)
{
    size_t lines_nb, line_len = 0;
    char *line = NULL;
    
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // formatage of the cursor's rows
    lines_nb = data->file_lines->len;
    if (lines_nb == 0)
        data->cursor_row = 0;
    else if (data->cursor_row > lines_nb - 1)
        data->cursor_row = lines_nb - 1;

    // formatage of the cursor's col
    line = data->file_lines->data[data->cursor_row];
    for (line_len = 0; line[line_len]; line_len++);
    if (line_len == 0)
        data->cursor_col = 0;
    else if (data->cursor_col > line_len && line_len > 0)
        data->cursor_col = line_len;
    
    // formatage of the screen's row
    if (data->cursor_row > data->screen_row + (max_rows - 1 - CURSOR_ROW_FROM_BORDER)) {
        if (data->cursor_row + CURSOR_ROW_FROM_BORDER >= lines_nb)
            data->screen_row = data->cursor_row - (max_rows + data->cursor_row - lines_nb);
         else if ((int) data->cursor_row >= max_rows - 1 - CURSOR_ROW_FROM_BORDER)
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
