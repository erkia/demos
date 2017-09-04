#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>

#include "cli/cli.h"

/**
 * Callback for 'help' command.
 */
static int cmd_help (int argc, char **argv)
{
    printf ("HelpHelp\n");
}


/**
 * Callback for 'time' command.
 */
static int cmd_time (int argc, char **argv)
{
    time_t t = time (NULL);
    printf ("Current time: %s", asctime (localtime (&t)));
}


/**
 * Callback for 'test' command. (I.e 'test 1 2 3')
 */
static int cmd_test (int argc, char **argv)
{
    int i;
    for (i = 0; i < argc; i++) {
        printf ("argv[%d]: %s\n", i, argv[i]);
    }
}


/**
 * Disable canonical mode (buffered I/O) and local echo for STDIN
 */
static void stdin_disable_buffering (void)
{
    struct termios tio;
	tcgetattr (STDIN_FILENO, &tio);
	tio.c_lflag &= (~ICANON & ~ECHO);
	tcsetattr (STDIN_FILENO, TCSANOW, &tio);
}


int main (int argc, char **argv)
{
    const cli_cmd_t commands[] = {
        {
            .name       = "help",
            .help       = "This is the Help",
            .callback   = cmd_help
        },
        {
            .name       = "time",
            .help       = "Shows current time",
            .callback   = cmd_time
        },
        {
            .name       = "test",
            .help       = "Test command arguments",
            .callback   = cmd_test
        }
    };

    const cli_cmd_list_t cmdlist = {
        .commands = commands,
        .count = (sizeof (commands) / sizeof (commands[0]))
    };

    stdin_disable_buffering ();

    CLI_Init (&cmdlist);
    CLI_Utf8 (1);

    char c;
    while (1) {
        c = getchar ();
        CLI_Parse (&c, 1);
        CLI_HandleLine ();
    }

    return 0;
}
