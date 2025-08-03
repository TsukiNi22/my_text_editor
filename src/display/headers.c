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
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // init the color
    init_pair(1, COLOR_BLUE, COLOR_WHITE);

    // write the header with the color
    bkgdset(COLOR_PAIR(1) | A_BOLD);
    move(0, 0);
    clrtoeol();
    
    // write the header with the color
    mvprintw(0, 0, "File: '%s'", data->file);
    
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
        mvprintw(LINES - 1, 0, "mode: '%s'", "none"); // read, write, select, exe, none
    else if (pos > 9)
        mvprintw(LINES - 1, 0, "mode: '%c'", 'n'); // r, w, s, e, n
    else
        mvprintw(LINES - 1, 0, "'%c'", 'n'); // r, w, s, e, n
    if (pos <= 5)
        pos = 6;
    mvprintw(LINES - 1, pos, "%lu,%lu", data->cursor_row + 1, data->cursor_col + 1);
    mvprintw(LINES - 1, COLS - 15, "(F1 to quit...)");
    
    // reset the color
    bkgdset(A_NORMAL);
    return OK;
}
