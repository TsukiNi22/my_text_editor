/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  02/08/2025 by Tsukini

File Name:
##  init_ncurses.c

File Description:
## Initialisation of ncurses
\**************************************************************/

#include "error.h"      // error handling
#include <ncurses.h>    // ncurses init function

/* Ncurses initialisation function
----------------------------------------------------------------
 * Initialisation of the terminal for ncurses
----------------------------------------------------------------
##  void -> nothing
----------------------------------------------------------------
*/
int init_ncurses(void)
{
    int res = OK;

    // init ncurses
    if (!initscr())
        return err_prog(UNDEF_ERR, KO, ERR_INFO);

    // parametre of ncurses
    res += (cbreak() == KO);                // no wait for enter to read the keys pressed
    res += (noecho() == KO);                // dosen't display the keys pressed
    res += (keypad(stdscr, TRUE) == KO);    // allow spécial keys
    if (res != OK)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);

    return OK;
}
