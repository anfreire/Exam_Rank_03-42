// INCLUDES

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

char *return_line(char *buffer)
{
    int i;
    int j;
    char *line;

    i = 0;
    j = 0;
    while (buffer[i] != '\n' && buffer[i] != '\0')
        i++;
    if (buffer[i] == '\0')
        return (NULL);
    line = malloc(sizeof(char) * (i + 1));
    if (!line)
        return (NULL);
    while (j <= i)
    {
        line[j] = buffer[j];
        j++;
    }
    line[j] = '\0';
    return (line);
}

char *remove_line_from_buffer(char *buffer)
{
    int i;
    int j;
    char *new_buffer;

    i = 0;
    j = 0;
    while (buffer[i] != '\n' && buffer[i] != '\0')
        i++;
    if (buffer[i] == '\0')
        return (NULL);
    new_buffer = malloc(sizeof(char) * (ft_strlen(buffer) - i + 1));
    if (!new_buffer)
        return (NULL);
    i++;
    while (buffer[i] != '\0')
    {
        new_buffer[j] = buffer[i];
        i++;
        j++;
    }
    new_buffer[j] = '\0';
    free(buffer);
    buffer = NULL;
    return (new_buffer);
}

char *append_buffer(char *buffer)
{
    int i;
    char *new_buffer;

    new_buffer = malloc(sizeof(char) * (ft_strlen(buffer) + BUFFER_SIZE + 1));
    if (!new_buffer)
        return (NULL);
    i = 0;
    while (buffer[i] != '\0')
    {
        new_buffer[i] = buffer[i];
        i++;
    }
    new_buffer[i] = '\0';
    free(buffer);
    buffer = NULL;
    return (new_buffer);
}

char *get_next_line(int fd)
{
    static char *buffer;
    char *line;
    int i;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    if (!buffer)
    {
        buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
        if (!buffer)
            return (NULL);
    }
    i = 0;
    while (buffer[i] != '\0' && buffer[i] != '\n')
        i++;
    if (buffer[i] == '\0')
    {
        buffer = append_buffer(buffer);
        if (!buffer)
            return (NULL);
        int len = ft_strlen(buffer);
        i = read(fd, buffer + ft_strlen(buffer), BUFFER_SIZE);
        buffer[len + i] = '\0';
        if (i == -1)
            return (NULL);
        buffer[ft_strlen(buffer)] = '\0';
        if (i == 0)
            return (NULL);
        return (get_next_line(fd));
    }
    line = return_line(buffer);
    buffer = remove_line_from_buffer(buffer);
    return (line);
}
