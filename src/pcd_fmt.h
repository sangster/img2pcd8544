#ifndef PCD_FMT_H
#define PCD_FMT_H

#include <stdlib.h>
#include <MagickWand/MagickWand.h>


#define TARGET_W  84 ///< Maximum width of the destination image, in pixels
#define TARGET_H  48 ///< height width of the destination image, in pixels


/**
 * Format the given image's size and color depth to be suitable for a PCD8544
 * LCD screen
 *
 * @param [out] w  The new width of the image
 * @param [out] h  The new height of the image
 */
MagickWand* pcd_filter(MagickWand*, size_t* w, size_t* h);

#endif//PCD_FMT_H
