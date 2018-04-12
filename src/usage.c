#include "usage.h"


void fprint_usage(FILE* stream, const char* app)
{
    fprintf(
        stream,
        "usage: %s --bmp       INPUT_IMG CONST_NAME\n"
        "       %s --font      INPUT_IMG FONT_NAME CHAR_W CHAR_H\n"
        "       %s --fmt       INPUT_IMG OUTPUT_IMG\n"
        "       %s --help, -h\n",
        app, app, app, app
    );
}
