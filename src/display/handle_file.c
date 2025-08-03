/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  03/08/2025 by Tsukini

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

// display the top header information
static int display_top_header(editor_t *data)
{
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // init the color
    init_pair(1, COLOR_BLUE, COLOR_WHITE);

    // write the header with the color
    bkgdset(COLOR_PAIR(1) | A_BOLD);
    move(0, 0);
    clrtoeol();
    mvprintw(0, 0, "File: '%s'", data->file);
    bkgdset(A_NORMAL);
    return OK;
}

// display the bottom header information
static int display_bottom_header(editor_t *data)
{
    size_t pos_cursor_info = 25;
    size_t pos = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // setup the position of the text
    if (COLS <= 15 + pos_cursor_info)
        pos_cursor_info += COLS - (15 + pos_cursor_info);

    // init the color
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    
    // set the color
    bkgdset(COLOR_PAIR(2));
    move(LINES - 1, 0);
    clrtoeol();

    // write the header with the color
    pos = COLS - 15 - pos_cursor_info;
    if (pos > 14)
        mvprintw(LINES - 1, 0, "mode: '%s'", "select"); // read, write, select, exe, none
    else if (pos > 9)
        mvprintw(LINES - 1, 0, "mode: '%c'", 's'); // r, w, s, e, n
    else
        mvprintw(LINES - 1, 0, "'%c'", 's'); // r, w, s, e, n
    if (pos <= 5)
        pos = 6;
    mvprintw(LINES - 1, pos, "%lu,%lu", data->cursor_row + 1, data->cursor_col + 1);
    mvprintw(LINES - 1, COLS - 15, "(F1 to quit...)");
    
    // reset the color
    bkgdset(A_NORMAL);
    return OK;
}

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
    data->screen_row = 0;
    data->screen_col = 0;
    data->cursor_row = 0;
    data->cursor_col = 0;
    if (!data->content)
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
