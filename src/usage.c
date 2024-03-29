/* @file
 *
 * img2pcd8544 converts images to a format usable by a PCD8544 LCD controller.
 * Copyright (C) 2018  Jon Sangster
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */
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
