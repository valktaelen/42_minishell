/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges & jmilhas <x@student.42lyon.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 00:33:11 by aartiges &        #+#    #+#             */
/*   Updated: 2022/10/06 16:26:54 by aartiges &       ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "includes/mini_shell.h"

static void	ft_minishell_create_t_pars(t_shell *shell, char *line, char *buf)
{
	shell->t_pars = ft_memalloc(sizeof(t_track));
	if (!shell->t_pars)
	{
		ft_putstr_fd("parsing allocation problem [track]\n", 2);
		free(line);
		free(buf);
		ft_track_free_all(&(shell->t_env));
		rl_clear_history();
		exit(1);
	}
}

static void	ft_minishell_exec_cmd(t_shell *shell, char *line, char *buf)
{
	errno = 0;
	if (ft_check_syntax_prompt(line))
		shell->last_exit_status = 1;
	else
	{
		ft_minishell_create_t_pars(shell, line, buf);
		if (ft_parse(shell, line))
		{
			ft_putstr_fd("oups parsing error\n", 2);
			shell->last_exit_status = 1;
		}
		else
		{
			if (DEBUG)
				debug_tree(shell->operation, 0);
			exec_mode_ignore();
			ft_exec(shell, shell->operation);
			interactive_mode();
		}
		ft_track_free_all(&(shell->t_pars));
	}
}

static void	ft_minishell(t_shell *shell)
{
	char	*line;
	char	*buf;

	line = NULL;
	buf = ft_create_str_read_line(shell);
	line = readline(buf);
	while (line)
	{
		if (line && line[0] != '\0')
		{
			add_history(line);
			ft_minishell_exec_cmd(shell, line, buf);
		}
		free(line);
		free(buf);
		buf = ft_create_str_read_line(shell);
		line = readline(buf);
	}
	free(line);
	free(buf);
}

/**
 * @brief add or update the shell level
 * 
 * @param shell the shell
 */
static void	ft_shell_level(t_shell *shell)
{
	char	**cmd;
	t_env	*tmp;

	cmd = (char **)ft_memalloc(sizeof(char *) * 3);
	cmd[1] = ft_strdup("SHLVL");
	tmp = ft_get_env_key(shell->env, "SHLVL");
	if (tmp && tmp->value)
		cmd[2] = ft_itoa(ft_atoi(tmp->value) + 1);
	if (cmd[2] == NULL)
		cmd[2] = ft_strdup("1");
	cmd[0] = ft_join("%s=%s", cmd[1], cmd[2]);
	free(cmd[1]);
	free(cmd[2]);
	cmd[1] = NULL;
	ft_export(shell, cmd);
	free(cmd[0]);
	free(cmd);
}

int	main(int ac, char **av, char **ev)
{
	t_shell	shell;

	if (ac > 2)
	{
		ft_putstr_fd(av[0], 2);
		ft_putstr_fd("to many arguments\n", 2);
		return (1);
	}
	interactive_mode();
	ft_bzero(&shell, sizeof(t_shell));
	shell.t_env = ft_memalloc(sizeof(t_track));
	if (!shell.t_env)
	{
		ft_putstr_fd("parsing allocation problem [track]\n", 2);
		exit(1);
	}
	ft_init_env(&shell, ev);
	ft_init_oldpwd(&shell);
	ft_shell_level(&shell);
	ft_minishell(&shell);
	ft_track_free_all(&(shell.t_env));
	rl_clear_history();
	printf("exit\n");
	return (shell.last_exit_status);
}
