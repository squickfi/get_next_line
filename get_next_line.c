#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 64
#endif

int	find_char(char *s, char c)
{
	int	count;

	count = 0;
	while (s[count] != c)
		count++;
	return (count);
}

char	*ft_strrchr(char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return (s);
		s++;
	}
	return (0);
}

int	ft_strdup_till_c(char **s1, char **s2, char c, char **buf)
{
	char	*s1_clone;
	char	*s2_clone;

	*s1 = malloc(sizeof(char) * (find_char(*s2, c) + 1));
	if (!*s1)
	{
		free(*buf);
		return (-1);
	}
	s1_clone = *s1;
	s2_clone = *s2;
	while (*s2_clone != c)
		*s1_clone++ = *s2_clone++;
	*s1_clone = '\0';
	return (0);
}

int	ft_strjoin(char **s1, char **s2)
{
	char	*str;
	char	*str_start;
	char	*s1_clone;
	char	*s2_clone;

	str = malloc(sizeof(char) * (find_char(*s1, 0) + find_char(*s2, 0) + 1));
	if (!str)
	{
		free(*s1);
		free(*s2);
		return (-1);
	}
	s1_clone = *s1;
	s2_clone = *s2;
	str_start = str;
	while (*s1_clone)
		*str++ = *s1_clone++;
	while (*s2_clone)
		*str++ = *s2_clone++;
	*str = '\0';
	free (*s1);
	*s1 = str_start;
	return (0);
}

int	first_check(int fd, char **line, char **buf, ssize_t *ret)
{
	if (fd < 0 || !line || BUFFER_SIZE < 1)
		return (-1);
	*buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!*buf)
		return (-1);
	*ret = 1;
	return (0);
}

int	first_call(char **remain, char **buf)
{
	char	*buf_clone;
	char	*remain_clone;

	if (!*remain)
	{
		*remain = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!(*remain))
		{
			free(*buf);
			return (-1);
		}
		buf_clone = *buf;
		remain_clone = *remain;
		while (*buf_clone)
			*remain_clone++ = *buf_clone++;
		*remain_clone = '\0';
	}
	return (0);
}

int	n_is_in_remain(char **remain, char **line, char **buf)
{
	char	*temp;
	char	*temp_for_n;

	temp_for_n = ft_strrchr(*remain, '\n') + 1;
	if (ft_strdup_till_c(line, remain, '\n', buf))
	{
		free (*remain);
		return (-1);
	}
	if (ft_strdup_till_c(&temp, &temp_for_n, '\0', buf))
	{
		free(*remain);
		return (-1);
	}
	free(*remain);
	*remain = temp;
	return (0);
}

int	free_all_and_return(ssize_t ret, char **buf, char **remain, int var)
{
	if (var)
	{
		if (ret == -1)
		{
			free(*buf);
			return (-1);
		}
		return (0);
	}
	free(*buf);
	free(*remain);
	*remain = NULL;
	return (0);
}

int	foo_for_return(char **remain, char **line, ssize_t ret, char **buf)
{
	if (ft_strrchr(*remain, '\n'))
	{
		if (n_is_in_remain(remain, line, buf))
			return (-1);
	}
	else if (ret == 0)
	{
		if (ft_strdup_till_c(line, remain, '\0', buf))
		{
			free(*remain);
			return (-1);
		}
		return (free_all_and_return(ret, buf, remain, 0));
	}
	free(*buf);
	return (1);
}

int	get_next_line(int fd, char **line)
{
	ssize_t		ret;
	char		*buf;
	static char	*remain;

	if (first_check(fd, line, &buf, &ret))
		return (-1);
	while (ret > 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (free_all_and_return(ret, &buf, &remain, 1))
			return (-1);
		buf[ret] = '\0';
		if (!remain)
		{
			if (first_call(&remain, &buf))
				return (-1);
		}
		else if (ft_strjoin(&remain, &buf))
			return (-1);
		if (ft_strrchr(remain, '\n'))
			break ;
	}
	return (foo_for_return(&remain, line, ret, &buf));
}
