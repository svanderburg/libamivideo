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

#include "palette.h"
#include <stdlib.h>
#include "viewportmode.h"

AMI_OutputColor *amiVideo_computePalette(const AMI_Color *color, const unsigned int colorLength, const AMI_Long viewportMode, unsigned int *resultColorLength)
{
    int halfbrite;
    AMI_OutputColor *result;
    unsigned int i;
    
    /* Check whether the extra halfbrite bit is set */
    
    halfbrite = amiVideo_checkExtraHalfbrite(viewportMode);

    if(halfbrite)
	*resultColorLength = 2 * colorLength; /* We need double the amount of color entries if extra halfbrite is enabled */
    else
	*resultColorLength = colorLength;

    /* Allocate memory for the color palette */
    result = (AMI_OutputColor*)malloc(*resultColorLength * sizeof(AMI_OutputColor));
    
    /* Copy the original palette colors */
    
    for(i = 0; i < colorLength; i++)
    {
	result[i].r = color[i].r;
	result[i].g = color[i].g;
	result[i].b = color[i].b;
    }
    
    /* Generate half of the color values for the last half of the color array, if extra half brite is enabled */
    if(halfbrite)
    {
	for(i = 0; i < colorLength; i++)
	{
	    result[i + colorLength].r = color[i].r >> 1;
	    result[i + colorLength].g = color[i].g >> 1;
	    result[i + colorLength].b = color[i].b >> 1;
	}
    }
    
    /* Return the color palette */
    return result;
}
