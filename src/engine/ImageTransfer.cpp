#include "ImageTransfer.h"

#include <color.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageTransfer::Transfer16BitTo16BitNoColourKey(	const uint8_t * pSrc,
														const int iSrcPitch,
                                                        uint8_t * pDst,
														const int iDstPitch,
														const WinPoint & srcPos,
														const WinPoint & dstPos,
														const WinPoint & size )
{
	int x, y;
    uint32_t dwByteOffset;
    uint16_t wVal;

	// Copy the data over. Need to invert the data in the y direction.
	for( y=0; y<size.Y(); y++ )
	{
		for( x=0; x<size.X(); x +=2 )
		{
			dwByteOffset = ( y * iSrcPitch ) + ( x * 2 );
			wVal = ( pSrc[ dwByteOffset ] ) | ( pSrc[ dwByteOffset + 1 ] << 8 );

			// Store colour.
            pDst[ ( y * iDstPitch ) + ( x * 2 ) + 1 ] = uint8_t(wVal >> 8 );
            pDst[ ( y * iDstPitch ) + ( x * 2 ) + 0 ] = uint8_t(wVal & 0x00FF );

			wVal = ( pSrc[ dwByteOffset + 2 ] ) | ( pSrc[ dwByteOffset + 3 ] << 8 ) ;

			// Store colour.
            pDst[ ( y * iDstPitch ) + ( x * 2 ) + 3 ] = uint8_t(wVal >> 8 );
            pDst[ ( y * iDstPitch ) + ( x * 2 ) + 2 ] = uint8_t(wVal & 0x00FF );
		}
	}
}


void CImageTransfer::Transfer16BitTo16BitCopy(const uint8_t * pSrc,
    const int iSrcPitch,
    uint8_t * pDst,
    const int iDstPitch,
    const WinPoint & srcPos,
    const WinPoint & dstPos,
    const WinPoint & size
)
{
    int x, y;
    uint32_t dwByteOffset;
    uint16_t wVal, wNewColour;

    // Copy the data over. Need to invert the data in the y direction.
    for (y = 0; y<size.Y(); y++)
    {
        for (x = 0; x<size.X(); x += 1)
        {
            dwByteOffset = (y * iSrcPitch) + (x * 2);
            wVal = (pSrc[dwByteOffset]) | (pSrc[dwByteOffset + 1] << 8);

            pDst[(y * iDstPitch) + (x * 2) + 1] = uint8_t(wVal >> 8);
            pDst[(y * iDstPitch) + (x * 2) + 0] = uint8_t(wVal & 0x00FF);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageTransfer::Transfer16BitTo16BitWithColourKey(	const uint8_t * pSrc,
														const int iSrcPitch,
                                                        uint8_t * pDst,
														const int iDstPitch,
														const WinPoint & srcPos,
														const WinPoint & dstPos,
														const WinPoint & size,
														const Color & colorKey )
{
	int x, y;
    uint32_t dwByteOffset;
    uint16_t wVal, wNewColour;
	bool bExtraGreenBit;

	// Generate the 16 bit colour value to test against.
	COLORREF colorRef = colorKey.MakeCOLORREF();
    uint16_t wCompare = (uint16_t)(	( ( ( colorRef & 0x000000FF ) >> 3 ) << 11 ) |		// R
							( ( ( colorRef & 0x0000FF00 ) >> 10 ) << 5 ) |		// G
							( ( ( colorRef & 0x00FF0000 ) >> 19 ) ) );

	// Copy the data over. Need to invert the data in the y direction.
	for( y=0; y<size.Y(); y++ )
	{
		for( x=0; x<size.X(); x +=2 )
		{
			dwByteOffset = ( y * iSrcPitch ) + ( x * 2 );
			wVal = ( pSrc[ dwByteOffset ] ) | ( pSrc[ dwByteOffset + 1 ] << 8 );

			// Additional check needed to make sure we don't lose the bottom G bit
			// as we're going from 6 to 5 bits.
			bExtraGreenBit = ( ( ( wVal >> 5 )& 0x0001 ) ) != 0 ? true : false;

			if( ( wVal == wCompare ) && ( bExtraGreenBit == false ) )
			{
				// Create an A1R5G5B5 entry with alpha 0, from R5G6B5.
				wNewColour = 0;			// No alpha, pixel not shown.
			}
			else
			{
				// Create an A1R5G5B5 entry with alpha 1.
				wNewColour = (wVal & 0xFFC0) >> 1;
				wNewColour |= wVal & 0x001F;
				wNewColour |= 0x8000;
			}
			// Store colour.
            pDst[ ( y * iDstPitch ) + ( x * 2 ) + 1 ] = uint8_t(wNewColour >> 8 );
            pDst[ ( y * iDstPitch ) + ( x * 2 ) + 0 ] = uint8_t(wNewColour & 0x00FF );

			wVal = ( pSrc[ dwByteOffset + 2 ] ) | ( pSrc[ dwByteOffset + 3 ] << 8 ) ;

			// Additional check needed to make sure we don't lose the bottom G bit
			// as we're going from 6 to 5 bits.
			bExtraGreenBit = ( ( ( wVal >> 5 )& 0x0001 ) ) != 0 ? true : false;

			if( ( wVal == wCompare ) && ( bExtraGreenBit == false ) )
			{
				// Create an A1R5G5B5 entry with alpha 0, from R5G6B5.
				wNewColour = ( ( wVal & 0x001F ) | ( wVal & 0xFFC0 >> 1 ) ) & 0x7FFF;
			}
			else
			{
				// Create an A1R5G5B5 entry with alpha 1.
				wNewColour = (wVal & 0xFFC0) >> 1;
				wNewColour |= wVal & 0x001F;
				wNewColour |= 0x8000;
			}
			// Store colour.
            pDst[ ( y * iDstPitch ) + ( x * 2 ) + 3 ] = uint8_t(wNewColour >> 8 );
            pDst[ ( y * iDstPitch ) + ( x * 2 ) + 2 ] = uint8_t(wNewColour & 0x00FF );
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageTransfer::Transfer16BitTo32BitNoColourKey(	const uint8_t * pSrc,
														const int iSrcPitch,
                                                        uint8_t * pDst,
														const int iDstPitch,
														const WinPoint & srcPos,
														const WinPoint & dstPos,
														const WinPoint & size )
{
	int x, y;
	// Copy the data over. Need to invert the data in the y direction.
	for( y=0; y<size.Y(); y++ )
	{
        uint32_t * dwPtr = (uint32_t*) ( pDst + ( y * iDstPitch ) );
		for( x=0; x<size.X(); x ++ )
		{
			*dwPtr++ = 0xFF0000FF;
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageTransfer::Transfer16BitTo32BitWithColourKey(	const uint8_t * pSrc,
														const int iSrcPitch,
                                                        uint8_t * pDst,
														const int iDstPitch,
														const WinPoint & srcPos,
														const WinPoint & dstPos,
														const WinPoint & size,
														const Color & colorKey )
{
	int x, y;

    uint16_t wVal;
    uint32_t dwNewColour;

    COLORREF colorRef = Color(0, 0, 0).MakeCOLORREF();
    uint16_t wCompare = (uint16_t)(((((colorRef & 0x000000FF) >> 3) << 11) |	// R
        (((colorRef & 0x0000FF00) >> 10) << 5) |		// G
        (((colorRef & 0x00FF0000) >> 19))));

	// Copy the data over. Need to invert the data in the y direction.
	for( y=0; y<size.Y(); y++ )
	{
        uint16_t * dwSrcPtrY = (uint16_t*)(pSrc + ((y + srcPos.Y()) * iSrcPitch));
        uint32_t * dwDstPtrY = (uint32_t*)(pDst + ((y + dstPos.Y()) * iDstPitch));
		for( x=0; x<size.X(); x ++ )
		{
            uint16_t * dwSrcPtrYX = dwSrcPtrY + (srcPos.X() + x);
            uint32_t * dwDstPtrYX = dwDstPtrY + (dstPos.X() + x);

            wVal = *dwSrcPtrYX;

            if (wVal == wCompare)
            {
                // Create an A8R8G8B8 entry with alpha 0, from R5G6B5.
                dwNewColour = 0;			// No alpha, pixel not shown.
            }
            else
            {
                // Create an A8R8G8B8 entry with alpha at 255.
                dwNewColour = ((wVal & 0xF800) >> 8) << 16;
                dwNewColour |= ((wVal & 0x07E0) >> 3) << 8;
                dwNewColour |= ((wVal & 0x001F) << 3);
                dwNewColour |= 0xFF000000;
            }

			*dwDstPtrYX = dwNewColour;
		}
	}
}

void CImageTransfer::Transfer32BitTo32BitCopy(
    const uint8_t * pSrc,
    const int iSrcPitch,
    uint8_t * pDst,
    const int iDstPitch,
    const WinPoint & srcPos,
    const WinPoint & dstPos,
    const WinPoint & size)
{
    int x, y;

    // Copy the data over. Need to invert the data in the y direction.
    for (y = 0; y<size.Y(); y++)
    {
        uint32_t * dwSrcPtrY = (uint32_t*)(pSrc + ((y + srcPos.Y()) * iSrcPitch));
        uint32_t * dwDstPtrY = (uint32_t*)(pDst + ((y + dstPos.Y()) * iDstPitch));
        for (x = 0; x<size.X(); x++)
        {
            uint32_t * dwSrcPtrYX = dwSrcPtrY + (srcPos.X() + x);
            uint32_t * dwDstPtrYX = dwDstPtrY + (dstPos.X() + x);

            *dwDstPtrYX = *dwSrcPtrYX;
        }
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// ColourImageFill16Bit()
// Fill in a rectangular area of a surface with a 16-bit colour value.
// Note: the calling function should check colour key etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageTransfer::ColourImageFill16Bit(void * pDst,
                                            const int iDstPitch,
                                            const WinPoint & dstPos,
                                            const WinPoint & size,
                                            const uint16_t wPixelColour )
{
    uint16_t * pDestData = (uint16_t*) pDst;
	int iPitch = iDstPitch >> 1;			// Pitch / 2.
	int iX, iY;

	for( iY = dstPos.Y(); iY<dstPos.Y() + size.Y(); iY++ )
	{
		for( iX = dstPos.X(); iX<dstPos.X() + size.X(); iX++ )
		{
			pDestData[ (iY * iPitch) + iX ] = wPixelColour; 
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// ColourImageFill32Bit()
// Fill in a rectangular area of a surface with a 16-bit colour value.
// Note: the calling function should check colour key etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CImageTransfer::ColourImageFill32Bit(void * pDst,
                                            const int iDstPitch,
                                            const WinPoint & dstPos,
                                            const WinPoint & size,
                                            const uint32_t dwPixelColour )
{
    uint32_t * pDestData = (uint32_t*) pDst;
	int iPitch = iDstPitch >> 2;			// Pitch / 4.
	int iX, iY;

	for( iY = dstPos.Y(); iY<dstPos.Y() + size.Y(); iY++ )
	{
		for( iX = dstPos.X(); iX<dstPos.X() + size.X(); iX++ )
		{
			pDestData[ (iY * iPitch) + iX ] = dwPixelColour; 
		}
	}
}

