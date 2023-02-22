#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int ft_strlen(char *str)
{
    int i;

    if (!str)
        return (0);
    i = 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

char *recover_save(char **save, char *line)
{
    if ((*save))
    {
        int save_len = ft_strlen((*save));
        int str_len = ft_strlen(line);
        char *tmp = malloc(sizeof(char) * (save_len + str_len + 1));
        if (!tmp)
        {
            free(line);
            free((*save));
            return (NULL);
        }
        tmp[save_len + str_len] = 0;
        if ((*save))
        {
            while (save_len >= 0)
            {
                tmp[save_len] = (*save)[save_len];
                save_len--;
            }
            free((*save));
        }
        if (line)
        {
            while (str_len >= 0)
            {
                tmp[save_len + str_len] = line[str_len];
                str_len--;
            }
            free(line);
        }
        *save = NULL;
        return (tmp);
    }
    return (line);
}

char *ft_realloc(char *str, char **save)
{
    str = recover_save(save, str);
    if (!str)
    {
        str = malloc(sizeof(char) * (BUFFER_SIZE + 1));
        if (!str)
            return (NULL);
        str[BUFFER_SIZE] = 0;
    }
    else
    {
        int tmp_len = ft_strlen(str);
        char *tmp = malloc(sizeof(char) * (tmp_len + BUFFER_SIZE + 1));
        if (!tmp)
        {
            free(str);
            return (NULL);
        }
        tmp[tmp_len + BUFFER_SIZE] = 0;
        while (tmp_len >= 0 && str)
        {
            tmp[tmp_len] = str[tmp_len];
            tmp_len--;
        }
        free(str);
        str = &(*tmp);
    }
    return (str);
}

int ft_strchr(char *str, char c)
{
    int i;

    if (!str)
        return (0);
    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (i);
        i++;
    }
    return (0);
}

char *get_next_line(int fd)
{
    char *line = NULL;
    static char *save = NULL;
    int read_bytes = 0;

    while ((line = ft_realloc(line, &save)) != NULL && (read_bytes = read(fd, &line[ft_strlen(line)], BUFFER_SIZE)) > 0)
    {
        if (!line)
            return (NULL);
        else if (read_bytes < BUFFER_SIZE)
        {
            line[read_bytes] = 0;
            return (line);
        }
        else if (ft_strchr(line, '\n'))
        {
            save = malloc(sizeof(char) * (ft_strlen(line) - ft_strchr(line, '\n') + 1));
            if (!save)
            {
                free(line);
                return (NULL);
            }
            int i = 1;
            while (line[ft_strchr(line, '\n') + i])
            {
                save[i - 1] = line[ft_strchr(line, '\n') + i];
                line[ft_strchr(line, '\n') + i] = 0;
                i++;
            }
            save[i - 1] = 0;
            return (line);
        }
    }
    return (NULL);
}

#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

    char *line = NULL;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s\n", line);
        free(line);
    }

    if (line == NULL)
    {
        printf("Reached end of file.\n");
    }
    else
    {
        printf("Error: get_next_line returned NULL.\n");
    }

    if (close(fd) == -1)
    {
        perror("close");
        exit(1);
    }

    return 0;
}
