#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
    struct stat st;
    if (lstat(fname, &st) == -1)
    {
        perror("error lstat");
        return -1;
    }

    if (S_ISDIR(st.st_mode))
    {
        return get_size_dir(fname, blocks);
    }
    else
    {
        *blocks += st.st_blocks;
        return 0;
    }
}

/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
    DIR *dir = opendir(dname);
    if (dir == NULL)
    {
        perror("No se puede abrir");
        return -1;
    }
    struct dirent *leer;
    while ((leer = readdir(dir)) != NULL)
    {
        if (strcmp(leer->d_name, ".") == 0 || strcmp(leer->d_name, "..") == 0)
        {
            continue;
        }

        char ruta[1024];
        snprintf(ruta, sizeof(ruta), "%s/%s", dname, leer->d_name);

        if (S_ISDIR(leer->d_type))
        {
            if (get_size_dir(ruta, blocks) == -1)
            {
                closedir(dir);
                return -1;
            }
        }
        else
        {
            if (get_size(ruta, blocks) == -1)
            {
                closedir(dir);
                return -1;
            }
        }
    }
    closedir(dir);
    return 0;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Faltan parámetros");
        return 1;
    }
    size_t total = 0;
    for (int i = 1; i < argc; i++)
    {

        if (get_size(argv[i], &total) == -1)
        {
            perror("error al obtener el tamaño");
            exit(1);
        }
        else
        {
            printf("%luK %s\n", (unsigned long)(total * 512 / 1024), argv[i]);
        }
        total = 0; // Reset total for the next file/directory
    }

    return 0;
}


