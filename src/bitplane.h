/**
 * Copyright (c) 2012 Sander van der Burg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __AMIVIDEO_BITPLANE_H
#define __AMIVIDEO_BITPLANE_H

#include "amivideotypes.h"
#include "palette.h"

/**
 * Converts a collection of bitplanes into an array of bytes, in which each byte
 * represents the pixel's color index. The resulting array must be freed using
 * free(). This function cannot be used to display Hold-and-Modify (HAM) screen
 * modes.
 *
 * @param bitplanes An array representing bitplane data
 * @param screenWidth Width of the screen
 * @param screenHeight Height of the screen
 * @param bitplaneDepth Number of bits used for the color palette
 * @return An array of bytes representing pixel index values
 */
amiVideo_UByte *amiVideo_bitplanesToChunky(const amiVideo_UByte *bitplanes, const unsigned int screenWidth, const unsigned int screenHeight, const unsigned int bitplaneDepth);

/**
 * Converts a collection of bitplanes into an array of colors capturing the RGB
 * value of each pixel. The resulting array must be freed using free().
 *
 * @param bitplanes An array representing bitplane data
 * @param screenWidth Width of the screen
 * @param screenHeight Height of the screen
 * @param bitplaneDepth Number of bits used for the color palette
 * @param color An array of colors representing the original Amiga palette
 * @param colorLength Length of the color array
 * @param numOfColorBits The amount of bits each color components has (OCS/ECS chipsets have 4, AGA has 8)
 * @param viewportMode Amiga viewport register value
 * @return An array of bytes representing pixel index values
 */
amiVideo_OutputColor *amiVideo_bitplanesToRGB(const amiVideo_UByte *bitplanes, const unsigned int screenWidth, const unsigned int screenHeight, const unsigned int bitplaneDepth, const amiVideo_Color *color, const unsigned int colorLength, const unsigned int numOfColorBits, const amiVideo_Long viewportMode);

#endif
