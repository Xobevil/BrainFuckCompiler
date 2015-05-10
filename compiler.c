/*
**	Autor : Sylvain "Xob" Garant
**
**	BrainFuck Interpreter
*/

#include <stdlib.h>
#include <stdio.h>

#define INPUT_SIZE 1000

typedef struct 		s_lnk
{
	char			*ptr;
	struct s_lnk	*next;
}					t_lnk;

int		my_strlen(char *str)
{
	return ((*str) ? (1 + my_strlen(str + 1)) : (0));
}

char		*my_strdup(char *str)
{
	char	*result;
	char	*buf;

	if (!(result = malloc(my_strlen(str) + 1)))
		return (NULL);
	buf = result;
	while (*str++)
		*buf++ = *(str - 1);
	*buf = '\0';
	return (result);
}

void		my_memset(void *s, char p, int n)
{
	char	*ptr;

	ptr = (char*) &s;
	while (n--)
		*ptr++ = p;
}

int match(char *s1, char *s2)
{
  	if (*s1 != '\0' && *s2 == '*')
    	return (match(s1 + 1, s2) || match(s1, s2 + 1));
  	if (*s1 == '\0' && *s2 == '*')
	    return (match(s1, s2 + 1));
  	if (*s1 == *s2 && *s1 != '\0' && *s2 != '\0')
	    return (match(s1 + 1, s2 + 1));
  	if (*s1 == *s2 && *s1 == '\0' && *s2 == '\0')
	    return (1);
  	return (0);
}

char		*reallocat(char *st, char *nd)
{
	char	*result;
	char	*buf;

	if (!st)
		return (nd);
	if (!(result = malloc(my_strlen(st) + my_strlen(nd) + 1)))
		return (NULL);
	buf = result;
	while (*st++)
		*buf++ = *(st - 1);
	while (*nd++)
		*buf++ = *(nd - 1);
	*buf = '\0';
	return (result);
}

char		*get_file(FILE *file)
{
	char	*full_file;
	char	buf[100];

	full_file = NULL;
	while (fgets(buf, sizeof(buf), file))
	{
		if (!full_file)
			full_file = my_strdup(buf);
		else if (!(full_file = reallocat(full_file, buf)))
			return (NULL);
	}
	return (full_file);
}

int		do_want(char c, char *want)
{
	while (*want++)
		if (c == *(want - 1))
			return (1);
	return (0);
}

char		*epurstr(char *str, char *want)
{
	char	*result;
	char	*buf;

	if (!(result = malloc(my_strlen(str) + 1)))
		return (NULL);
	buf = result;
	while (*str++)
		if (do_want(*(str - 1), want))
			*buf++ = *(str - 1);
	*buf = '\0';
	return (result);
}

int 				put_list(struct s_lnk **stack, char *ptr)
{
	struct s_lnk	*elem;

	if (!(elem = malloc(sizeof(*elem))))
		return (1);
	my_memset(elem, 0, sizeof(*elem));
	elem->ptr = ptr;
	elem->next = *stack;
	*stack = elem;
	return (0);
}

void			brainfuck(char *code, char *input)
{
	struct s_lnk	*stack;
	char			*ptr;
	char			mem[30000];

	ptr = mem;
	my_memset(mem, '\0', 30000);
	stack = NULL;
	while (*code++)
	{
		if (*(code - 1) == '+')
			(*ptr)++;
		else if (*(code - 1) == '-')
			(*ptr)--;
		else if (*(code - 1) == '>')
			*ptr++;
		else if (*(code - 1) == '<')
			*ptr--;
		else if (*(code - 1) == '.')
			printf("%c", *ptr);
		else if (*(code - 1) == ',')
			(*input) ? (*ptr = *input++) : (*ptr = *input);
		else if (*(code - 1) == '[')
			put_list(&stack, code);
		else if (*(code - 1) == ']' && *ptr && stack)
			code = stack->ptr;
		else if (*(code - 1) == ']' && !(*ptr) && stack)
			stack = stack->next;
	}
}

int			main(int argc, char **argv)
{
	FILE	*file;
	char	*full_file;
	char	input[INPUT_SIZE];

	if (argc != 2)
		return (1);
	if (!(file = fopen(argv[1], "r")))
		return (1);
	if (!(full_file = get_file(file)))
		return (1);
	full_file = epurstr(full_file, "<>.,[]+-");
	my_memset(input, 0, INPUT_SIZE);
	if (match(full_file, "*,*"))
	{
		printf("Input > ");
		scanf("%s", input);
	}
	brainfuck(full_file, input);
	fclose(file);
	return (0);
}
