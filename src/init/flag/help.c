/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  01/08/2025 by Tsukini

File Name:
##  help.c

File Description:
## Display help
\**************************************************************/

#include "write.h"      // my_printf function
#include "editor.h"     // external variable

/* Help function
----------------------------------------------------------------
 * A function used to display the help
 * Call when the flag '-h' or '--help' is used
----------------------------------------------------------------
*/
int flag_help(void)
{
    // Display help
    return my_printf(
    "[36mPROJECT\n"
    "\tThis project is a text editor\n"
    "\tThe name of the text editor is [35m\'Banana Writer\'[36m or [35m\'bw\'\n\n"
    "[32mINFORMATION\n"
    "\t-Multiple flag will overwrite the first!!!\n"
    "\t-At least one file must be given\n\n"
    "[35mUSAGE\n"
    "\tbw files_path... [-%c directory_path] [-%c directory_path]\n"
    "\tbw [-%c directory_path] [-%c directory_path]\n"
    "\tbw -%c\n\n"
    "[0mOPTIONS\n"
    "\t[32m-%c, %s [0m\n"
    "\t\tWrite the informations of the executable\n"
    "\t[32m-%c, %s [31mdirectory_path[0m\n"
    "\t\tAdd the files found without recursive in the given [31mdirectory_path[0m in the list of file to compile\n"
    "\t[32m-%c, %s [31mdirectory_path[0m\n"
    "\t\tAdd the files found recursively in the given [31mdirectory_path[0m in the list of file to compile\n"
    // USAGE
    , flags[1], flags[2] // d D
    , flags[1], flags[2] // d D
    , flags[0] // h
    // OPTIONS
    , flags[0], full_flags[0] // h help
    , flags[1], full_flags[1] // d directory
    , flags[2], full_flags[2] // D DIRECTORY
    );
}
