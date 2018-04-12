#ifndef CMD_FMT_H
#define CMD_FMT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MagickWand/MagickWand.h>
#include "pcd_fmt.h"
#include "usage.h"

#define ERR_FMT_ARGS  1

extern char* APP_NAME;


int cmd_fmt(int argc, char *argv[]);

#endif//CMD_FMT_H
