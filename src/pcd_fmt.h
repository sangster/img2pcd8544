#ifndef PCD_FMT_H
#define PCD_FMT_H
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
