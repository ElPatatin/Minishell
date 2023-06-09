/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:31:21 by cpeset-c          #+#    #+#             */
/*   Updated: 2023/04/25 16:07:30 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "mnshll_utils.h"
#include "mnshll_builtins.h"
#include "mnshll_data.h"
#include "mnshll_error.h"

extern int	g_exit_status;

static void	aux_ft_export(t_prompt *prompt, char **av, int cnt, int idx);
static int	print_declare_env(ssize_t size, t_prompt *prompt);
static void	aux_print_declare_env(char **cpy_env, ssize_t size);
static int	error_export(char *str);

int	ft_export(int ac, char **av, t_prompt *prompt)
{
	ssize_t	cnt;
	ssize_t	idx;

	cnt = 0;
	if (ac == 1)
		return (print_declare_env(ft_env_size(prompt->export), prompt));
	else
	{
		while (av[++cnt])
		{
			idx = 0;
			if (av[cnt][0] == '='
				|| !(av[cnt][0] == '_' || ft_isalpha(av[cnt][0])))
				return (error_export(av[cnt]));
			while (av[cnt][idx] && ft_strncmp(&av[cnt][idx], "=", 1))
				++idx;
			aux_ft_export(prompt, av, cnt, idx);
		}
	}
	return (0);
}

static int	print_declare_env(ssize_t size, t_prompt *prompt)
{
	char	**cpy_env;
	ssize_t	idx;
	t_env	*cpy;

	cpy_env = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!cpy_env)
		ft_prompt_clear(prompt, ERR_MEM, 1);
	idx = 0;
	cpy = prompt->export;
	while (cpy)
	{
		if (cpy->env_var && cpy->env_data)
		{
			cpy_env[idx] = aux_print_declare_env_join(cpy);
			if (!cpy_env[idx])
				ft_prompt_clear(prompt, ERR_MEM, 1);
			++idx;
		}
		cpy = cpy->next;
	}
	aux_print_declare_env(cpy_env, size);
	return (EXIT_SUCCESS);
}

static void	aux_print_declare_env(char **cpy_env, ssize_t size)
{
	ssize_t	idx;
	char	*str;

	bubble_sort(cpy_env, size);
	idx = -1;
	while (++idx < size)
	{
		str = ft_strtrim_charset_end(cpy_env[idx], MS_EXPCMP);
		printf("declare -x %s\n", str);
	}
	ft_memfree(cpy_env);
}

static int	error_export(char *str)
{
	ft_printf_fd(STDERR_FILENO, "mns: export: \'%s\': %s", str, ERR_EXP);
	g_exit_status = 1;
	return (EXIT_FAILURE);
}

static void	aux_ft_export(t_prompt *prompt, char **av, int cnt, int idx)
{
	if (av[cnt][idx])
	{
		before_export(&prompt->export, av[cnt], TRUE);
		before_export(&prompt->env, av[cnt], TRUE);
	}
	else
		before_export(&prompt->export, av[cnt], FALSE);
}
