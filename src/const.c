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
##  const.c

File Description:
## Constant used in flag initialisation and other thing
\**************************************************************/

#include "editor.h" // editor_t type
#include <stddef.h> // NULL definition

// Flag that are with '-'
char const flags[] = {
    'h',
    'd',
    'D',
    '\0'
};

// Flag that are with '--'
char const *full_flags[] = {
    "--help",
    "--directory",
    "--Directory",
    NULL
};

// Used for the number of arg of a flag (-1 for those who can't be conbined)
int const flags_argc[] = {
    -1, // help
    1,  // directory
    1,  // Directory
};

// Function call at the detection of  a flag
int (* const flag_functions[])(editor_t *, int const, char const *[]) = {
    &flag_null, // help (special case)
    &flag_directory, // directory
    &flag_Directory, // Directory
};

// Full mode name
char const *modes[] = {
    "none",
    "read",
    "write",
    "select",
    "exe",
    NULL
};
