/*
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

#ifndef __AMIVIDEO_PALETTE_H
#define __AMIVIDEO_PALETTE_H
#include "amivideotypes.h"

/**
 * @brief Struct storing values of a color channel.
 * This struct type has the same structure as the ILBM file format's color map.
 */
typedef struct
{
    /** Defines the intensity of the red color channel */
    amiVideo_UByte r;
    
    /** Defines the intensity of the green color channel */
    amiVideo_UByte g;
    
    /** Defines the intensity of the blue color channel */
    amiVideo_UByte b;
}
amiVideo_Color;

/**
 * @brief Struct storing values of a color channel.
 * This struct type has the same structure as SDL_Color.
 */
typedef struct
{
    /** Defines the intensity of the red color channel */
    amiVideo_UByte r;
    
    /** Defines the intensity of the green color channel */
    amiVideo_UByte g;
    
    /** Defines the intensity of the blue color channel */
    amiVideo_UByte b;

    /** Used for alignment with SDL_Color */
    amiVideo_UByte unused; 
}
amiVideo_OutputColor;

/**
 * Coverts the given color palette into a 32-bit color palette. Additionally,
 * it appends half of the color values, if the extra halfbrite screen mode is
 * enabled. The resulting color array must be freed using free().
 *
 * @param color An array of colors representing the original Amiga palette
 * @param colorLength Length of the color array
 * @param numOfColorBits The amount of bits each color components has (OCS/ECS chipsets have 4, AGA has 8)
 * @param viewportMode Amiga viewport value
 * @param resultColorLength The length of the resulting color array
 * @return An array of coverted 32-bit colors
 */
amiVideo_OutputColor *amiVideo_computePalette(const amiVideo_Color *color, const unsigned int colorLength, const unsigned int numOfColorBits, const amiVideo_Long viewportMode, unsigned int *resultColorLength);

#endif
