#include <stdio.h>
#include "../includes/fs_c.h"
#include "../includes/string_builder.h"

void log_str(char *str)
{

    fprintf(stderr, "%s\n", str);
}

void free_string_ele(void *string)
{
    if (is_null(string))
    {
        free_string((String *)string);
    }
}

int main(int argc, char **argv)
{
    List *list = allocate_list();

    FS_Dir fs_dir;

    Node *node;
    void *_state;

    if(is_null(list)) return -1;

    if (argc != 3)
    {
        fprintf(stderr, "%s <path> <name>\n", argv[0]);
        return -1;
    }

    fs_dir.name = argv[2];
    fs_dir.path = argv[1];

    if (fs_read_dir(&fs_dir, list) == 0)
    {
        node = mpm_list_next(list, &_state);
        if (is_null(node))
        {
            log_str("ending");
            goto end;
        }
        else
            goto loop;

        while ((node = mpm_list_next(NULL, &_state)) != NULL)
        {
        loop:
            fprintf(stderr, "=>%s\n", ((String *)node->element)->str);
        }
    }
end:
    free_list(list, free_string_ele);
    return 0;
}
