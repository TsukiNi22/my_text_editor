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
##  free_data.c

File Description:
## Free any malloced memory during the compilation
\**************************************************************/

#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <stdlib.h>     // free function
#include <ncurses.h>    // endwin function

#include <stdio.h>

// only in a linux environement
#if defined(__linux__)
    #include <dlfcn.h>  // dl* function 
#endif

// free the memory from the ncurses initialisation
static int free_ncurses(void)
{
    void (*_nc_free_all)(void) = NULL;
    void *handle = NULL;
    
    // reset the terminal and free main memory alloced
    if (endwin() == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    
    // try to free all memory from the ncurses, only in a linux environement in some case
    #if defined(__linux__)
        // try to charge the ncurses lib (without the version '.*', low chance of success)
        handle = dlopen("libncurses.so", RTLD_LAZY);
        if (!handle) // if not found try the wide version
            handle = dlopen("libncursesw.so", RTLD_LAZY);

        if (handle) {
            // try to charge the function
            _nc_free_all = dlsym(handle, "_nc_free_all");
            
            // if the function is found use it
            if (_nc_free_all)
                _nc_free_all();
            
            // close the open ncurses lib
            dlclose(handle);
        }
    #endif
    return OK;
}

// Free simple pointer
static int free_ptr(void *ptr)
{
    // Check for potential null pointer
    if (!ptr)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    free(ptr);
    return OK;
}

/* Free data function
----------------------------------------------------------------
 * Free the var in the main structure who was malloced
 * during the compilation
----------------------------------------------------------------
##  data -> main data structure
----------------------------------------------------------------
*/
int free_data(editor_t *data)
{
    int res = OK;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    res += delete_array(&data->files, &free_ptr);
    res += free_ncurses();
    if (res != OK)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
