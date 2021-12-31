/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commandmanager.h"

void register_command(command_t command, command_manager_t *manager)
{
    ++manager->commands_count;
    manager->commands = realloc(manager->commands, manager->commands_count * sizeof(command_t));
    manager->commands[manager->commands_count - 1] = command;
}

command_t get_command(char *name, command_manager_t *manager)
{
    int i;
    for (i = 0; i < manager->commands_count; ++i)
    {
        if (strcmp(manager->commands[i].name, name) == 0)
        {
            return manager->commands[i];
        }
    }
    command_t noret;
    noret.flags |= 1 << 0; // Set error flag
    return noret;
}

void delete_command(char *name, command_manager_t *manager)
{
    if (manager->commands_count != 0){
        int i;
        command_t *temp_commands;
        temp_commands = malloc(manager->commands_count - 1);
        int offset = 0;
        for (i = 0; i < manager->commands_count; ++i)
        {
            if (strcmp(manager->commands[i].name, name) != 0)
            {
                temp_commands[offset] = manager->commands[i];
                ++offset;
            }
        }
        free(manager->commands);
        manager->commands = temp_commands;
		--manager->commands_count;
    }
}

void execute(char *name, int argc, char **argv, command_manager_t *manager){
    command_t command = get_command(name, manager);
	if ((command.flags & 7) == 0) { // check if this command exists
		command.executor(argc, argv);
	}
}