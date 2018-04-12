#ifndef CMD_BMP_H
#define CMD_BMP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MagickWand/MagickWand.h>
#include "pcd_fmt.h"
#include "usage.h"

#define ERR_BMP_ARGS  1

extern char* APP_NAME;


int cmd_bmp(int argc, char *argv[]);

#endif//CMD_BMP_H
