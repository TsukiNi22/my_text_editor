/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  05/08/2025 by Tsukini

File Name:
##  handle_keys.c

File Description:
## Handle the key pressed during the display
\**************************************************************/

#include "my_string.h"  // my_strcmp function
#include "memory.h"     // my_malloc_c function
#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function
#include <wchar.h>      // wchar_t type, wchar functions
#include <stdlib.h>     // free function
#include <stdio.h>      // file handle functions
#include <string.h>     // memset function
#include <stdbool.h>    // bool type
#include <stddef.h>     // size_t type, NULL define

char *get_new_content(editor_t *data)
{
    char *file_content = NULL;
    wchar_t *wline = NULL;
    char *line = NULL;
    size_t size, len, index = 0;
    mbstate_t state = {0};
    
    // Check for potential null pointer
    if (!data)
        return err_prog_n(PTR_ERR, ERR_INFO);

    // get the full size
    for (size_t i = 0; i < data->file_lines->len; i++) {
        wline = data->file_lines->data[i];
        memset(&state, 0, sizeof(state));
        size += wcsrtombs(NULL, (const wchar_t **) &wline, 0, &state);
        size++;
    }
    if (my_malloc_c(&file_content, size + 1) == KO)
        return err_prog_n(UNDEF_ERR, ERR_INFO);

    // setup the content
    for (size_t i = 0; i < data->file_lines->len; i++) {
        // setup of the line
        wline = data->file_lines->data[i];
        memset(&state, 0, sizeof(state));
        len = wcsrtombs(NULL, (const wchar_t **) &wline, 0, &state);
        if (len == (size_t) KO)
            return err_prog_n(UNDEF_ERR, ERR_INFO);
        if (my_malloc_c(&line, len + 1) == KO)
            return err_prog_n(UNDEF_ERR, ERR_INFO);

        // convert the wchar to char
        wline = data->file_lines->data[i];
        memset(&state, 0, sizeof(state));
        wcsrtombs(line, (const wchar_t **) &wline, len + 1, &state);

        // set the char in the content
        memcpy(&file_content[index], line, len);
        index += len;
        if (i + 1 < data->file_lines->len)
            file_content[index++] = '\n';
        free(line);
    }
    file_content[index] = '\0';
    return file_content;
}

/* Save content function
----------------------------------------------------------------
 * Save the actual content to the designed file
----------------------------------------------------------------
##  data -> main data structure
----------------------------------------------------------------
*/
int save_content(editor_t *data)
{
    FILE *file = NULL;
    char *file_content = NULL;
    char *anwser = NULL;
    

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // check if the user want to overwrite
    if (is_valid_file(data, data->file, false)) {
        anwser = dialog(data, "This file already exist do you want to overwrite it? (y/n)", KO);
        if (!anwser)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (my_strcmp(anwser, "N") == 0 || my_strcmp(anwser, "n") == 0
            || my_strcmp(anwser, "No") == 0 || my_strcmp(anwser, "no") == 0
            || my_strcmp(anwser, "Non") == 0 || my_strcmp(anwser, "non") == 0) {
            free(anwser);
            return OK;
        }
        free(anwser);
    }
    data->changed = false;

    // get the new content
    file_content = get_new_content(data);
    if (!file_content)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);

    // write the file
    file = fopen(data->file, "w");
    if (!file)
        return err_prog(OP_FILE_ERR, KO, ERR_INFO);
    fputs(file_content, file);
    fclose(file);
    free(file_content);
    return OK;
}
