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
