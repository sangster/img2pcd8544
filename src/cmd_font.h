#ifndef CMD_FONT_H
#define CMD_FONT_H

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MagickWand/MagickWand.h>
#include "usage.h"

#define ERR_FONT_ARGS      1
#define ERR_FONT_BAD_DIMENSION  2

extern char* APP_NAME;


int cmd_font(int argc, char *argv[]);

#endif//CMD_FONT_H
