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
##  handle_file.c

File Description:
## Return the content of the given file
\**************************************************************/

#include "editor.h"     // editor_t type, get_file function
#include "error.h"      // error handling
#include <stdlib.h>     // free function
#include <ncurses.h>    // ncurses function
#include <stddef.h>     // size_t type, NULL define

// display the line in the terminal
static int display(editor_t *data)
{
    char **formated_lines = NULL;
    
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // format the lines for the row & col number
    resize_term(0, 0);
    formated_lines = format_lines(data, COLS, LINES - 1);
    if (!formated_lines)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // Temporary test display
    clear();
    if (display_top_header(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    refresh();
    for (int i = 1, row = 0; formated_lines[row] && i < LINES; i++, row++)
        mvprintw(i, 0, "%s", formated_lines[row]);
    refresh();
    if (display_bottom_header(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
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
int handle_file(editor_t *data, const char *file)
{
    // Check for potential null pointer
    if (!data || !file)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // init the file value
    data->file = file;
    data->content = get_file(file);
    data->file_map = map_file(data->content);
    data->screen_row = 0;
    data->screen_col = 0;
    data->cursor_row = 0;
    data->cursor_col = 0;
    if (!data->content || !data->file_map)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // wait for F1 to quit
    for (int ch = KO; ch != KEY_F(1); ch = getch()) {
        // handle the key pressed
        if (ch != KO && handle_keys(data, ch) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);

        // update the display
        if (display(data) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        move(data->cursor_row - data->screen_row + 1, data->cursor_col - data->screen_col);
    }
    
    // free memory alloced
    free(data->content);
    return OK;
}
