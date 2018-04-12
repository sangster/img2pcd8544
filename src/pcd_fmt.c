#include "pcd_fmt.h"


static MagickWand* add_white_background(MagickWand*);
static void make_grayscale(MagickWand*);
static void shrink_image(MagickWand*, size_t* w, size_t* h);


MagickWand* pcd_filter(MagickWand* mw, size_t* width, size_t* height)
{
    mw = add_white_background(mw);
    make_grayscale(mw);
    shrink_image(mw, width, height);
    return mw;
}


MagickWand* add_white_background(MagickWand* mw)
{
    PixelWand* color = NewPixelWand();
    PixelSetColor(color, "white");

    MagickSetImageBackgroundColor(mw, color);
    return MagickMergeImageLayers(mw, FlattenLayer);

}


void make_grayscale(MagickWand* mw)
{
    MagickQuantizeImage(mw, 2, GRAYColorspace, 0, FloydSteinbergDitherMethod,
                        MagickFalse);
}


void shrink_image(MagickWand* mw, size_t* width, size_t* height)
{
    *width  = MagickGetImageWidth(mw);
    *height = MagickGetImageHeight(mw);

    if (*width > TARGET_W || *height > TARGET_H) {
        const double ratio = fmin(((double) TARGET_W) / *width,
                                  ((double) TARGET_H) / *height);

        MagickScaleImage(mw, *width  * ratio + 0.5,
                             *height * ratio + 0.5);

        *width  = MagickGetImageWidth(mw);
        *height = MagickGetImageHeight(mw);
    }
}
