#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <MagickWand/MagickWand.h>

#include "cmd_bmp.h"
#include "cmd_font.h"
#include "cmd_fmt.h"
#include "usage.h"

#define ERR_ARGS         1
#define ERR_CMD_UNKNOWN  2

enum command
{
    CMD_UNKNOWN,
    CMD_BITMAP,
    CMD_FONT,
    CMD_FORMAT,
    CMD_HELP,
};
typedef enum command Command;


Command parse_command(const char*);


char* APP_NAME;

int main(int argc, char *argv[])
{
    APP_NAME = argv[0];

    if (argc < 2) {
        fprintf(stderr, "No command specified\n");
        fprint_usage(stderr, APP_NAME);
        return ERR_ARGS;
    }

    const char* CMD = argv[1];
    argc -= 2;
    argv += 2;
    switch(parse_command(CMD)) {
        case CMD_BITMAP: return cmd_bmp(argc, argv);
        case CMD_FONT:   return cmd_font(argc, argv);
        case CMD_FORMAT: return cmd_fmt(argc, argv);
        case CMD_HELP:
            fprint_usage(stdout, APP_NAME);
            return 0;
        default:
            fprintf(stderr, "Unknown command: %s\n", CMD);
            /* fprint_usage(stderr, APP_NAME); */
            return ERR_CMD_UNKNOWN;
    }
}


Command parse_command(const char* cmd)
{
    if (strcmp(cmd, "--bmp") == 0) {
        return CMD_BITMAP;
    } else if (strcmp(cmd, "--font") == 0) {
        return CMD_FONT;
    } else if (strcmp(cmd, "--fmt") == 0) {
        return CMD_FORMAT;
    } else if (strcmp(cmd, "--help") == 0 || strcmp(cmd, "-h") == 0) {
        return CMD_HELP;
    }
    return CMD_UNKNOWN;
}
