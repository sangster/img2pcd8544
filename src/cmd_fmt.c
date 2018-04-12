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
#include "cmd_fmt.h"


int cmd_fmt(int argc, char *argv[])
{
    if (argc != 2) {
        fprint_usage(stderr, APP_NAME);
        return ERR_FMT_ARGS;
    }
    const char* INPUT_IMG  = argv[0];
    const char* OUTPUT_IMG = argv[1];

    MagickWandGenesis();
    MagickWand* mw = NewMagickWand();
    MagickReadImage(mw, INPUT_IMG);

    size_t width, height;
    mw = pcd_filter(mw, &width, &height);

    MagickWriteImage(mw, OUTPUT_IMG);

    if (mw) {
        mw = DestroyMagickWand(mw);
    }
    MagickWandTerminus();
    return 0;
}
