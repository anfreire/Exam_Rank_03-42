#include "get_next_line.h"

int ft_strlen(char *str)
{
    int i;

    if (!str)
        return (0);
    i = 0;
    while (str[i])
        i++;
    return (i);
}

int new_line_index(char *str)
{
    int i;

    if (!str)
        return (-1);
    i = 0;
    while (str[i])
    {
        if (str[i] == '\n')
            return (i);
        i++;
    }
    return (-1);
}

int recover_rest_line(char **save, char **line)
{
    if ((*save))
    {
        int save_len = ft_strlen(*save);
        int line_len = ft_strlen(*line);
        char *tmp = malloc(sizeof(char) * (save_len + line_len + 1));
        if (!tmp)
        {
            free(*line);
            free(*save);
            *line = NULL;
            *save = NULL;
            return (0);
        }
        tmp[save_len + line_len] = 0;
        if (*save)
        {
            while (save_len >= 0)
            {
                tmp[save_len] = (*save)[save_len];
                save_len--;
            }
            free(*save);
            *save = NULL;
        }
        if (*line)
        {
            while (line_len >= 0)
            {
                tmp[save_len + line_len] = (*line)[line_len];
                line_len--;
            }
            free(*line);
            *line = NULL;
        }
        *line = tmp;
    }
    return (1);
}

int save_rest_line(char **line, char **save)
{
    int line_len = ft_strlen(*line);
    int nl_index_position = new_line_index(*line);
    int save_len = line_len - nl_index_position - 1;
    *save = malloc(sizeof(char) * (save_len + 1));
    if (!(*save))
    {
        free(*line);
        *line = NULL;
        return (0);
    }
    (*save)[save_len] = 0;
    while (save_len >= 0)
    {
        (*save)[save_len] = (*line)[nl_index_position + 1 + save_len];
        (*line)[nl_index_position + 1 + save_len] = 0;
        save_len--;
    }
    return (1);
}

int gnl_realloc(char **line)
{
    if (!(*line))
    {
        *line = malloc(sizeof(char) * (BUFFER_SIZE + 1));
        if (!(*line))
            return (0);
        (*line)[BUFFER_SIZE] = 0;
    }
    else
    {
        int line_len = ft_strlen(*line);
        char *tmp = malloc(sizeof(char) * (line_len + BUFFER_SIZE + 1));
        if (!tmp)
        {
            free(*line);
            *line = NULL;
            return (0);
        }
        tmp[line_len + BUFFER_SIZE] = 0;
        while (line_len >= 0)
        {
            tmp[line_len] = (*line)[line_len];
            line_len--;
        }
        free(*line);
        *line = tmp;
    }
    return (1);
}

char *get_next_line(int fd)
{
    int read_bytes = 0;
    char *line = NULL;
    static char *save = NULL;

    while (1)
    {
        if (save)
        {
            if (!recover_rest_line(&save, &line))
                return (NULL);
            if (new_line_index(line) != -1)
            {
                if (!save_rest_line(&line, &save))
                    return (NULL);
                return (line);
            }
        }
        if (!gnl_realloc(&line))
            return (NULL);
        read_bytes = read(fd, &line[ft_strlen(line)], BUFFER_SIZE);
        if (read_bytes == -1)
        {
            free(line);
            line = NULL;
            return (NULL);
        }
        else if (new_line_index(line) != -1)
        {
            if (!save_rest_line(&line, &save))
                return (NULL);
            return (line);
        }
        else if (read_bytes < BUFFER_SIZE)
        {
            if (read_bytes == 0)
                return (line);
            line[ft_strlen(line)] = 0;
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

    printf("%s", get_next_line(fd));
    printf("%s", get_next_line(fd));

    if (close(fd) == -1)
    {
        perror("close");
        exit(1);
    }

    return 0;
}
