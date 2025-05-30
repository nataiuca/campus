/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/26 21:46:55 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_leaks(void)
{
	system("leaks minishell");
}

int main(int argc, char **argv, char **envp)
{
    t_minishell ms;
    char *input;

    prepare_minishell(&ms);
    valid_inital_param(argc, envp, &ms);
    disable_echoctl();
    catch_signal();
    init_history(&ms);
    input = get_input(&ms);
    while (input)
    {
        ms.error_syntax = false;
        catch_interactive(&ms, input, "minishell$");
        input = check_input_valid(input);
        if (*input)
        {
            save_history_file(&ms, input);
            control_cmd(parsecmd(input, &ms), &ms);
        }
        free(input);
        input = get_input(&ms);
    }
    {
        int fd;
        char *path = construct_history_path(HISTFILE_NAME);
        if (path)
        {
            fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd >= 0)
            {
                write_history_lines(fd, ms.history, 0);
                close(fd);
            }
            free(path);
        }
    }
    safe_free_minishell(&ms);
    return (0);
}
