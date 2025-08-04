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
##  display_file.c

File Description:
## Return the content of the given file
\**************************************************************/

#include "editor.h"     // editor_t type, get_file function
#include "error.h"      // error handling
#include <stdlib.h>     // free function
#include <ncurses.h>    // ncurses function
#include <stddef.h>     // size_t type, NULL define

// display the line in the terminal
static int display_content(editor_t *data, int max_cols, int max_rows)
{
    char **formated_lines = NULL;
    
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // format the lines for the row & col number
    resize_term(0, 0);
    formated_lines = format_lines(data, max_cols, max_rows);
    if (!formated_lines)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // clear the screen
    for (int i = 0; i < LINES; i++) {
        move(i, 0);
        clrtoeol();
    }
    
    // display the content
    if (display_top_header(data) == KO || display_bottom_header(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int i = 1, row = 0; formated_lines[row] && i < LINES  - 1; i++, row++)
        mvprintw(i, 0, "%s", formated_lines[row]);
    refresh();

    // free memory alloced
    for (size_t i = 0; formated_lines[i]; i++)
        free(formated_lines[i]);
    free(formated_lines);
    return OK;
}

/* Handle open/close file function
----------------------------------------------------------------
 * Handle the opening & closing of the file in the display
----------------------------------------------------------------
##  data -> main data structure
----------------------------------------------------------------
*/
int display_file(editor_t *data, const char *file)
{
    int max_cols, max_rows = 0;
    
    // Check for potential null pointer
    if (!data || !file)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // init the file value
    data->mode = READ;
    data->file = file;
    data->content = get_file(file);
    data->file_lines = get_file_lines(data->content);
    data->screen_row = 0;
    data->screen_col = 0;
    data->cursor_actual_col = 0;
    data->cursor_row = 0;
    data->cursor_col = 0;
    if (!data->content || !data->file_lines)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // wait for F1 to quit
    for (int ch = KO; ch != KEY_F(1); ch = getch()) {
        max_cols = COLS, max_rows = LINES - 2;
        
        // handle the key pressed
        if (ch != KO && handle_keys(data, ch) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);

        // update the col & row of screen & cursor
        if (update_pos(data, max_cols, max_rows) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        
        // update the display
        if (display_content(data, max_cols, max_rows) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        move(data->cursor_row - data->screen_row + 1, data->cursor_col - data->screen_col);
    }
    
    // free memory alloced
    free(data->content);
    return OK;
}
