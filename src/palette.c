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

#include "palette.h"
#include <stdlib.h>
#include "viewportmode.h"

amiVideo_OutputColor *amiVideo_computePalette(const amiVideo_Color *color, const unsigned int colorLength, const unsigned int numOfColorBits, const amiVideo_Long viewportMode, unsigned int *resultColorLength)
{
    int halfbrite;
    amiVideo_OutputColor *result;
    unsigned int i;
    int shiftAmount;
    
    /* Check whether the extra halfbrite bit is set */
    
    halfbrite = amiVideo_checkExtraHalfbrite(viewportMode);

    if(halfbrite)
	*resultColorLength = 2 * colorLength; /* We need double the amount of color entries if extra halfbrite is enabled */
    else
	*resultColorLength = colorLength;

    /* Allocate memory for the color palette */
    result = (amiVideo_OutputColor*)malloc(*resultColorLength * sizeof(amiVideo_OutputColor));
    
    /* Take the original palette colors and convert the to 8-bit color components */
    
    shiftAmount = 8 - numOfColorBits;
    
    for(i = 0; i < colorLength; i++)
    {
	result[i].r = color[i].r << shiftAmount;
	result[i].g = color[i].g << shiftAmount;
	result[i].b = color[i].b << shiftAmount;
    }
    
    /* Generate half of the color values for the last half of the color array, if extra half brite is enabled */
    if(halfbrite)
    {
	for(i = 0; i < colorLength; i++)
	{
	    result[i + colorLength].r = (color[i].r >> 1) << shiftAmount;
	    result[i + colorLength].g = (color[i].g >> 1) << shiftAmount;
	    result[i + colorLength].b = (color[i].b >> 1) << shiftAmount;
	}
    }
    
    /* Return the color palette */
    return result;
}
