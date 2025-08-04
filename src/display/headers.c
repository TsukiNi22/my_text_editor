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
##  headers.c

File Description:
##  Function to Display the top & botton header
\**************************************************************/

#include "editor.h"     // editor_t type, get_file function
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function
#include <stddef.h>     // size_t type, NULL define

/* Display top header Function
----------------------------------------------------------------
 * Display the top header informations
----------------------------------------------------------------
##  data -> main data structure
----------------------------------------------------------------
*/
int display_top_header(editor_t *data)
{
    const char *file_name = "[help]";
    int len = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // init the color
    init_pair(1, COLOR_BLUE, COLOR_WHITE);

    // write the header with the color
    bkgdset(COLOR_PAIR(1) | A_BOLD);
    move(0, 0);
    clrtoeol();
    
    // setup the file name
    if (!data->display_help)
        file_name = data->file;

    // write the header with the color
    for (len = 0; data->file[len]; len++);
    if (COLS - 22 - 12 < 6)
        mvprintw(0, 0, "'%.*s...'", (COLS - 22 - 6), file_name);
    else if (len + 12 > COLS - 22)
        mvprintw(0, 0, "file: '%.*s...'", (COLS - 22 - 12), file_name);
    else
        mvprintw(0, 0, "file: '%s'", file_name);
    mvprintw(0, COLS - 22, "(press F5 for help...)");

    // write the header with the color
    bkgdset(A_NORMAL);
    return OK;
}

/* Display bottom header Function
----------------------------------------------------------------
 * Display the bottom header informations
----------------------------------------------------------------
##  data -> main data structure
----------------------------------------------------------------
*/
int display_bottom_header(editor_t *data)
{
    int pos_cursor_info = 25;
    int pos = 0;

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
        mvprintw(LINES - 1, 0, "mode: '%s'", modes[data->mode]);
    else if (pos > 10)
        mvprintw(LINES - 1, 0, "mode: '%c'", modes[data->mode][0]);
    else
        mvprintw(LINES - 1, 0, "'%c'", modes[data->mode][0]);
    if (pos <= 4)
        pos = 5;
    mvprintw(LINES - 1, pos, "%lu,%lu", data->cursor_row + 1, data->cursor_col + 1);
    if (pos > 10)
        mvprintw(LINES - 1, COLS - 15 - 7, "%.1f%%", (((double) data->cursor_row * (double) 100) / ((double) data->file_lines->len - 1)));
    mvprintw(LINES - 1, COLS - 15, "(F1 to quit...)");
    
    // reset the color
    bkgdset(A_NORMAL);
    return OK;
}
