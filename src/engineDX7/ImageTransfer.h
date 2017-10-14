
#ifndef _IMAGETRANSFER_H_
#define _IMAGETRANSFER_H_

class CImageTransfer
{
public:
	///////////////////////////////////////////////////////////////////////////////////
	static void Transfer16BitTo16BitNoColourKey(	const BYTE * pSrc,
													const int iSrcPitch,
													BYTE * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size );

	///////////////////////////////////////////////////////////////////////////////////
	static void Transfer16BitTo16BitWithColourKey(	const BYTE * pSrc,
													const int iSrcPitch,
													BYTE * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size,
													const Color & colorKey );

	///////////////////////////////////////////////////////////////////////////////////
	static void Transfer16BitTo32BitNoColourKey(	const BYTE * pSrc,
													const int iSrcPitch,
													BYTE * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size );

	///////////////////////////////////////////////////////////////////////////////////
	static void Transfer16BitTo32BitWithColourKey(	const BYTE * pSrc,
													const int iSrcPitch,
													BYTE * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size,
													const Color & colorKey );

	///////////////////////////////////////////////////////////////////////////////////
	static void Transfer24BitTo32BitWithColourKey(	const BYTE * pSrc,
													const int iSrcPitch,
													BYTE * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size,
													const Color & colorKey );

	///////////////////////////////////////////////////////////////////////////////////
	static void Transfer32BitTo32BitWithColourKey(	const BYTE * pSrc,
													const int iSrcPitch,
													BYTE * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size,
													const Color & colorKey );

	///////////////////////////////////////////////////////////////////////////////////
	static void ColourImageFill16Bit(	void * pDst,
										const int iDstPitch,
										const WinPoint & dstPos,
										const WinPoint & size,
										const WORD wPixelColour );

	///////////////////////////////////////////////////////////////////////////////////
	static void ColourImageFill32Bit(	void * pDst,
										const int iDstPitch,
										const WinPoint & dstPos,
										const WinPoint & size,
										const DWORD dwPixelColour );
};

#endif // _IMAGETRANSFER_H_