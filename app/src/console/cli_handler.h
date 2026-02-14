
#ifndef H_CLI_HANDLER_
#define H_CLI_HANDLER_

#include <stdint.h>

typedef void (*response_cb)(char *data, uint32_t length);

int cli_handler_process_command(char *command, uint32_t command_length, response_cb write_handler);

#endif // H_CLI_HANDLER_
