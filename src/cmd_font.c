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
#include "cmd_font.h"

#define TAB  "    "

static int  parse_char_size(MagickWand*, const char*, const char*, size_t* w,
                            size_t* h);
static void print_header_top(const char* app, const char* name,
                             const char* file);
static void print_data(MagickWand* mw, size_t char_w, size_t*);
static void print_row_of_banks(uint8_t*, size_t w, size_t char_w,
                               PixelIterator*, size_t*);
static void print_header_bot(const char*, int w, char first, size_t);
static void print_rendered_char(uint8_t*, size_t, const char*);


int cmd_font(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Wrong number of arguments\n");
        fprint_usage(stderr, APP_NAME);
        return ERR_FONT_ARGS;
    }
    const char* INPUT_IMG = argv[0];
    const char* FONT_NAME = argv[1];
    const char* CHAR_W    = argv[2];
    const char* CHAR_H    = argv[3];

    MagickWandGenesis();
    MagickWand* mw = NewMagickWand();
    MagickReadImage(mw, INPUT_IMG);

    size_t char_w, char_h;
    int result = parse_char_size(mw, CHAR_W, CHAR_H, &char_w, &char_h);
    if (result != 0) {
        return result;
    }

    size_t codepoint = 0;
    print_header_top(APP_NAME, FONT_NAME, INPUT_IMG);
    print_data(mw, char_w, &codepoint);
    print_header_bot(FONT_NAME, char_w, '\0', codepoint);

    if (mw) {
        mw = DestroyMagickWand(mw);
    }
    MagickWandTerminus();
    return 0;
}


int parse_char_size(MagickWand* mw, const char* arg_w, const char* arg_h,
                    size_t* w, size_t* h)
{
    char* scan_last;

    *w = strtol(arg_w, &scan_last, 10);
    if (*scan_last != '\0') {
        fprintf(stderr, "CHAR_W, \"%s\", is not an integer\n", arg_w);
        return ERR_FONT_BAD_DIMENSION;
    }

    *h = strtol(arg_h, &scan_last, 10);
    if (*scan_last != '\0') {
        fprintf(stderr, "CHAR_H, \"%s\", is not an integer\n", arg_h);
        return ERR_FONT_BAD_DIMENSION;
    }

    if (*h != 8) {
        fprintf(stderr, "CHAR_H must by 8\n");
        return ERR_FONT_BAD_DIMENSION;
    }

    size_t width  = MagickGetImageWidth(mw);
    size_t height = MagickGetImageHeight(mw);

    if (width % *w || height % *h) {
        fprintf(stderr, "Image size (%dx%d) not evenly divisible by character "
                        "size (%dx%d).\n", width, height, *w, *h);
        return ERR_FONT_BAD_DIMENSION;
    }
    return 0;
}


void print_header_top(const char* app, const char* name, const char* file)
{
    printf("#ifndef IMG2PCD8544_%s_H\n"
           "#define IMG2PCD8544_%s_H\n\n"
           "#include <avr/pgmspace.h>\n"
           "#include <sangster/pcd8544_font.h>\n\n",
           name, name);
    printf("/// Automatically generated from \"%s\" with %s\n"
           "const PROGMEM uint8_t %s_CHARS[] = {\n",
            file, app, name);
}


void print_data(MagickWand* mw, size_t char_w, size_t* codepoint)
{
    PixelIterator* iter = NewPixelIterator(mw);

    const size_t w = MagickGetImageWidth(mw),
                 h = MagickGetImageHeight(mw);

    uint8_t* banks = malloc(sizeof(uint8_t) * w);
    for (size_t y = 0; y < h; y += 8) {
        memset(banks, 0x00, w);
        print_row_of_banks(banks, w, char_w, iter, codepoint);
    }

    DestroyPixelIterator(iter);
    free(banks);
}


void print_row_of_banks(uint8_t* banks, size_t width, size_t char_w,
                        PixelIterator* iter, size_t* codepoint)
{
    PixelInfo pixel;
    size_t row_width;

    for (size_t y = 0; y < 8; ++y) {
        PixelWand** row = PixelGetNextIteratorRow(iter, &row_width);

        for (size_t x = 0; x < width; ++x) {
            if (PixelGetRed(row[x]) < 0.5) {
                banks[x] |= 1 << y;
            }
        }
    }

    // FOR each character
    for (size_t x_start = 0; x_start < width; x_start += char_w) {
        if (x_start != 0) {
            printf("\n");
        }
        print_rendered_char(banks + x_start, char_w, TAB "// ");
        printf(TAB);

        // FOR each col in a character
        for (size_t x = x_start; x < x_start + char_w; ++x) {
            if (x != x_start) {
                printf(", ");
            }
            printf("0x%02x", banks[x]);
        }
        printf(",  // ");
        if (isprint(*codepoint)) {
            printf("'%c'", *codepoint);
        } else {
            printf("0x%02x", *codepoint);
        }

        printf("\n");
        ++(*codepoint);
    }
}


void print_rendered_char(uint8_t* banks, size_t char_w, const char* prefix)
{
    for (size_t y = 0; y < 8; ++y) {
        printf(prefix);
        for (size_t x = 0; x < char_w; ++x) {
            if (banks[x] & (1 << y)) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}


void print_header_bot(const char* name, const int char_w, const char first,
                      const size_t count)
{
    printf("};\n\n"
           "const PcdFont %s = {\n"
           TAB ".chars   = %s_CHARS,\n"
           TAB ".width  = %d,\n"
           TAB ".first  = 0x%02x,\n"
           TAB ".count  = %d,\n"
           "};\n"
           "#endif//IMG2PCD8544_%s_H\n",
            name, name, char_w, first, count, name);
}
