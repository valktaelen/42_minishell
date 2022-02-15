#include "../../../includes/mini_shell.h"

/**
 * @brief return a new empty node for the tree (add track on node)
 * 
 * @param shell the shell
 * @return t_operation* the node or null if error
 */
t_operation	*ft_get_new_node(t_shell *shell)
{
	t_operation	*op;

	op = ft_track(malloc(sizeof(t_operation)), &(shell->t_pars));
	if (op)
		ft_bzero(op, sizeof(t_operation));
	return (op);
}

/**
 * @brief say which operator in the command line have the priority
 * 
 * @param line the command line
 * @return t_priority the genre of the command line
 */
t_priority	ft_get_top_priority(char *line)
{
	size_t		i;
	t_priority	priority;
	char		quote;

	i = 0;
	quote = 0;
	priority = CMD;
	while (line && line[i])
	{
		if (!ft_pars_quote(line[i], &quote) && !quote)
		{
			if (line[i] == '|')
				return (PIPE);
			if (line[i] == '<' || line[i] == '>')
				priority = REDIRECTION;
		}
		++i;
	}
	return (priority);
}

/**
 * @brief recursive construction of the tree of execution
 * 
 * @param shell the shell
 * @param current the current node
 * @return int 0 if success, 1 if an error occurs
 */
int	ft_construct_child(t_shell *shell, t_operation *current)
{
	t_operation	*node;

	current->genre = ft_get_top_priority(current->to_parse);
	if (current->genre == PIPE && ft_construct_pipe(shell, current))
		return (1);
	else if (current->genre == REDIRECTION
		&& ft_construct_redirection(shell, current))
		return (1);
	else if (current->genre == CMD && ft_construct_cmd(shell, current))
		return (1);
	ft_track_free(&(shell->t_pars), current->to_parse);
	current->to_parse = NULL;
	node = current->childs;
	while (node)
	{
		if (ft_construct_child(shell, node))
			return (1);
		node = node->next;
	}
	return (0);
}
