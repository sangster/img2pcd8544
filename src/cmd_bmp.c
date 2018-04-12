#include "cmd_bmp.h"

#define TAB  "    "

static void print_header_top(MagickWand*, const char* app, const char* name,
                             const char* file);
static void print_data(MagickWand* mw, size_t width, size_t height);
static void bitmap_to_banks(uint8_t* banks, size_t width, size_t height,
                            PixelIterator* iter, size_t y);
static void print_row_of_banks(uint8_t*, size_t w);
static void print_header_bot(const char* name, const int w, const int h);
static void print_bmp_render(MagickWand* mw, const char*);


/*
 * Steps:
 *
 *  1. Confirm image is small enough, or resize it
 *  2. Convert to monochrome
 *  3. Convert 1-bit bitmap to PCD 8544 "bank" format
 *  4. Export .h file
 */
int cmd_bmp(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        fprint_usage(stderr, APP_NAME);
        return ERR_BMP_ARGS;
    }
    const char* INPUT_IMG  = argv[0];
    const char* CONST_NAME = argv[1];

    MagickWandGenesis();
    MagickWand* mw = NewMagickWand();
    MagickReadImage(mw, INPUT_IMG);

    size_t width, height;
    mw = pcd_filter(mw, &width, &height);

    print_header_top(mw, APP_NAME, CONST_NAME, INPUT_IMG);
    print_data(mw, width, height);
    print_header_bot(CONST_NAME, width, height);

    if (mw) {
        mw = DestroyMagickWand(mw);
    }
    MagickWandTerminus();
    return 0;
}


void print_header_top(MagickWand* mw, const char* app, const char* name,
                      const char* file)
{
    printf("#ifndef IMG2PCD8544_%s_H\n"
           "#define IMG2PCD8544_%s_H\n\n"
           "#include <sangster/pcd8544_bmp.h>\n\n"
           "/*\n"
           " * Automatically generated from \"%s\" with %s. Preview:\n",
           name, name, file, app);
    print_bmp_render(mw, " * ");
    printf(" */\n"
           "const PROGMEM PCD_BANK_T %s_DATA[] = {\n",
            name);
}


void print_bmp_render(MagickWand* mw, const char* prefix)
{
    size_t width  = MagickGetImageWidth(mw);
    size_t height = MagickGetImageHeight(mw);

    PixelIterator* iter = NewPixelIterator(mw);
    for (size_t y = 0; y < height; y += 8) {
        const size_t bits_left = height - y,
                         nbits = bits_left < 8 ? bits_left : 8;
        size_t row_width;

        for (size_t i = 0; i < nbits; ++i) {
            printf(prefix);
            PixelWand** row = PixelGetNextIteratorRow(iter, &row_width);
            for (size_t x = 0; x < width; ++x) {
                printf(PixelGetRed(row[x]) < 0.5 ? "#" : " ");
            }
            printf("\n");
        }
    }
    DestroyPixelIterator(iter);
}


void print_data(MagickWand* mw, size_t width, size_t height)
{
    uint8_t* banks = malloc(sizeof(uint8_t) * width);
    PixelIterator* iter = NewPixelIterator(mw);

    for (size_t y = 0; y < height; y += 8) {
        bitmap_to_banks(banks, width, height, iter, y);
        print_row_of_banks(banks, width);
    }

    DestroyPixelIterator(iter);
    free(banks);
}

void bitmap_to_banks(uint8_t* banks, size_t width, size_t height,
                        PixelIterator* iter, size_t y)
{
    const size_t bits_left = height - y,
                     nbits = bits_left < 8 ? bits_left : 8;
    size_t row_width;

    memset(banks, 0x00, width);

    for (size_t i = 0; i < nbits; ++i) {
        PixelWand** row = PixelGetNextIteratorRow(iter, &row_width);

        for (size_t x = 0; x < width; ++x) {
            if (PixelGetRed(row[x]) < 0.5) {
                banks[x] |= 1 << i;
            }
        }
    }
}


void print_row_of_banks(uint8_t* banks, size_t width)
{
    printf(TAB);
    for (size_t x = 0; x < width; ++x) {
        if (x != 0) {
            printf(", ");
        }
        printf("0x%02x", banks[x]);
    }
    printf(",\n");
}


void print_header_bot(const char* name, const int width, const int height)
{
    printf("};\n\n"
           "const PcdBitmap %s = {\n"
           TAB ".data   = %s_DATA,\n"
           TAB ".width  = %d,\n"
           TAB ".height = %d,\n"
           "};\n"
           "#endif//IMG2PCD8544_%s_H\n",
            name, name, width, height, name);
}
