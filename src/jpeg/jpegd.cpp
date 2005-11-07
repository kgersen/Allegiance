#include    <windows.h>
#define     CREATELIB
#include    "jpeg.h"
#include    "decom.h"
#include    <stdio.h>
#include    <assert.h>

#define USE_IE_JPEG 1

#define PAD4(x) (((x)+3) & 0xFFFFFFFC)

#pragma pack(1)
#pragma warning(disable : 4200)
typedef struct tagBITMAPFILE
{
	BITMAPFILEHEADER	bmfh;
	BITMAPINFOHEADER	bmih;
	RGBQUAD				pal[];
} BITMAPFILE;
#pragma warning(default : 4200)
#pragma pack()


HRESULT SaveDIBSection(HBITMAP hbm, LPCSTR szFileName)
{
	DIBSECTION ds;
	BITMAPFILE bmf;

	if (GetObject(hbm, sizeof(ds), &ds))
	{
		assert(ds.dsBmih.biPlanes == 1);
		assert(ds.dsBmih.biBitCount == 24);
		assert(ds.dsBmih.biCompression == 0);

		//
		// GetDIBColorTable s.b. here!!
		//

        bmf.bmfh.bfType				= 'MB';
        bmf.bmfh.bfSize				= sizeof(BITMAPFILE) + PAD4(ds.dsBmih.biWidth * 3) * ds.dsBmih.biHeight;
        bmf.bmfh.bfReserved1		= 0;
        bmf.bmfh.bfReserved2		= 0;
        bmf.bmfh.bfOffBits			= sizeof(BITMAPFILE);

		memcpy(&bmf.bmih, &ds.dsBmih, sizeof(BITMAPINFOHEADER));

		remove(szFileName);
		FILE *f=fopen(szFileName, "wb");
		if (f)
		{
			fwrite(&bmf, sizeof(BITMAPFILE), 1, f);
			fwrite(ds.dsBm.bmBits, PAD4(ds.dsBmih.biWidth * 3), ds.dsBmih.biHeight, f);
			fclose(f);

			return S_OK;
		}

	}
	
	return -1;
}


//
// read a file into memory
//
static void *LoadFileIntoMem(const char *szFileName, LPDWORD lpdwFileSize)
{
	HANDLE	hFile;
	DWORD	dwSize, dwRead;
	void *p;

	hFile=CreateFile(
		szFileName,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile==INVALID_HANDLE_VALUE)
		return NULL;

	dwSize=GetFileSize(hFile, NULL);

	if ((p=malloc(dwSize))==NULL)
	{
		CloseHandle(hFile);
		return NULL;
	}

	if (!ReadFile(hFile, p,	dwSize, &dwRead, NULL))
	{
		free(p);
		CloseHandle(hFile);
		return NULL;
	}

	CloseHandle(hFile);

	if (lpdwFileSize)
	{
		*lpdwFileSize=dwSize;
	}

	return p;
}

//
// read a resource into memory
//
static void *LoadResource(HINSTANCE hInst, LPCSTR szItem, DWORD *pdwSize=NULL)
{
	HRSRC	hRsrcInfo;
	HGLOBAL hGlobal;
	LPVOID	pR;
	DWORD	dwSize;

	if (!(hRsrcInfo = FindResource( hInst, szItem, "JPEG" )))
	{
		return NULL;
	}

	dwSize=SizeofResource(hInst, hRsrcInfo);
	
	if (!(hGlobal = LoadResource(hInst, hRsrcInfo)))
	{
		return NULL;
	}
	
	if (!(pR=LockResource(hGlobal)))
	{
		return NULL;
	}

	if (pdwSize)
		*pdwSize = dwSize;

	return pR;
}

#if USE_IE_JPEG

#define ASSERT(x)
#undef ATLTRACE
#define ATLTRACE
//#include "mjl.h"
//#define ATLTRACE dprintf

extern "C"
{
#include "jinclude.h"
#include "jpeglib.h"
#include "jversion.h"
#include "jerror.h"

//#ifdef _DEBUG
#define JPEG_INTERNAL_OPTIONS
//#endif
#include "jmorecfg.h"
}

struct BitmapParameters
{
	HBITMAP		hBmp;
	JSAMPARRAY	ppScanLinePointers;
	int			nStride;
};

// Forward definitions of functions
void CreateColormapAndBitmap(jpeg_decompress_struct* pcinfo, BitmapParameters& bmpParams, HPALETTE hPal);
void DeleteTempBuffers(jpeg_decompress_struct* pcinfo, BitmapParameters& bmpParams);
void InitJPEGSourceMgr(jpeg_decompress_struct* pcinfo, char* pFile, DWORD dwFileSize);
METHODDEF(void) error_exit (j_common_ptr pcinfo);
METHODDEF(void) output_message (j_common_ptr pcinfo);
METHODDEF(void) format_message (j_common_ptr pcinfo, char * buffer);
METHODDEF(void) reset_error_mgr (j_common_ptr pcinfo);
GLOBAL(struct jpeg_error_mgr *) jpeg_std_error (struct jpeg_error_mgr * err);
METHODDEF(void) init_source (j_decompress_ptr pcinfo);
METHODDEF(boolean) fill_input_buffer (j_decompress_ptr pcinfo);
METHODDEF(void) skip_input_data (j_decompress_ptr pcinfo, long num_bytes);
METHODDEF(void) term_source (j_decompress_ptr pcinfo);


extern "C" HBITMAP LoadJPEGImage(HINSTANCE hInst, LPCSTR szItem, UINT fuLoad, HPALETTE hPal)
{
	char*		file;
	DWORD		dwFileSize = 0;

	//
	// set "file" to a memory image of the JPEG file
	//
	if (hInst)
	{
		file = (char*) LoadResource(hInst, szItem, &dwFileSize);
	}
	else if (fuLoad & LR_LOADFROMFILE)
	{
		file = (char*) LoadFileIntoMem(szItem, NULL);
	}
	else
	{
		file = (char*) szItem;
	}

	// Allocate Decompression Object
	jpeg_decompress_struct	cinfo;
	jpeg_error_mgr			jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	// Set up file for reading---some work to be done here
	InitJPEGSourceMgr(&cinfo, (char*)file, dwFileSize);

	// Read Header
	jpeg_read_header(&cinfo, TRUE);

	// Set Decompression Parameters (if desired)

	cinfo.dct_method = JDCT_FLOAT;
	cinfo.out_color_space = JCS_RGB;

	BitmapParameters bmpParams;
	memset(&bmpParams, 0, sizeof(bmpParams));

	CreateColormapAndBitmap(&cinfo, bmpParams, hPal);

	jpeg_start_decompress(&cinfo);

	// Decompress the image
	JSAMPARRAY ppRow = bmpParams.ppScanLinePointers;		// Set to start of array of scan line pointers
	while (cinfo.output_scanline < cinfo.output_height)
	{
		// decompress as much of the image as it can in one call
		int nLinesProcessed = jpeg_read_scanlines(&cinfo, ppRow, cinfo.output_height - cinfo.output_scanline);

		if (cinfo.out_color_space == JCS_GRAYSCALE && hPal == NULL)	// Convert from monochrome to RGB?
		{
			// monochrome image information is packed as 1 byte per pixel.
			// we need to convert this to 24-bit RGB inplace.  So we'll start at the end of each scan
			// line and expand the data

			for (int i = nLinesProcessed; i--;)
			{
				RGBTRIPLE* pRGB = (RGBTRIPLE*) ppRow;
				pRGB += cinfo.output_width - 1;		// point to last RGBTRIPLE;
				BYTE* pSample = (BYTE*) ppRow;
				pSample += cinfo.output_width - 1;	// point to last input sample
				for (int j = cinfo.output_width; j--;)
				{
					pRGB->rgbtRed   = *pSample;		// duplicate sample into all three color components
					pRGB->rgbtGreen = *pSample;
					pRGB->rgbtBlue  = *pSample;
					pRGB--;
					pSample--;
				}
				ppRow++;			// proceed to next row
			}
		}
		else
		{
			ppRow += nLinesProcessed;
		}
		// Repeat until full image has been decompressed
	}

	jpeg_finish_decompress(&cinfo);

	// Release Decompression object
	DeleteTempBuffers(&cinfo, bmpParams);
	jpeg_destroy_decompress(&cinfo);

	//SaveDIBSection(bmpParams.hBmp, "d:\\jpeg.bmp");
	
	return bmpParams.hBmp;
}


void CreateColormapAndBitmap(jpeg_decompress_struct* pcinfo, BitmapParameters& bmpParams, HPALETTE hPal)
{
	int	nColors = hPal == NULL ? 0 : 256;
	int nBitCount = hPal == NULL ? 24 : 8;

	BITMAPINFO* pbmi = (BITMAPINFO*) new char[sizeof(BITMAPINFO) + (nColors * sizeof(RGBQUAD))];
	//
	// create the bitmap
	//
	pbmi->bmiHeader.biSize			= sizeof(pbmi->bmiHeader);
	pbmi->bmiHeader.biWidth			= pcinfo->image_width;
	pbmi->bmiHeader.biHeight		= pcinfo->image_height;
	pbmi->bmiHeader.biPlanes		= 1;
	pbmi->bmiHeader.biBitCount		= nBitCount;
	pbmi->bmiHeader.biCompression	= BI_RGB;
	pbmi->bmiHeader.biSizeImage		= 0;
	pbmi->bmiHeader.biXPelsPerMeter	= 100;
	pbmi->bmiHeader.biYPelsPerMeter	= 100;
	pbmi->bmiHeader.biClrUsed		= nColors;
	pbmi->bmiHeader.biClrImportant	= 0;

	if (hPal != NULL)
	{
		PALETTEENTRY	pe[256];
		int				i;

		GetPaletteEntries(hPal, 0, 256, pe);
		for (i=0 ; i<256 ; i++)
		{
			pbmi->bmiColors[i].rgbRed		= pe[i].peRed;
			pbmi->bmiColors[i].rgbGreen		= pe[i].peGreen;
			pbmi->bmiColors[i].rgbBlue		= pe[i].peBlue;
			pbmi->bmiColors[i].rgbReserved	= 0;
		}

		pcinfo->colormap = (JSAMPARRAY) new JSAMPROW[3];
		pcinfo->colormap[0] = new JSAMPLE[256 * 3];
		pcinfo->colormap[1] = pcinfo->colormap[0] + 256;
		pcinfo->colormap[2] = pcinfo->colormap[1] + 256;

		for (i=0 ; i<256 ; i++)
		{
			// Build the colormap from the palette entries
			*(pcinfo->colormap[RGB_RED] + i)	 = pe[i].peRed;
			*(pcinfo->colormap[RGB_GREEN] + i) = pe[i].peGreen;
			*(pcinfo->colormap[RGB_BLUE] + i)  = pe[i].peBlue;
		}
		pcinfo->quantize_colors = TRUE;
		pcinfo->actual_number_of_colors = 256;
	}

	HDC hDC = GetDC(NULL);
	LPVOID pbits = NULL;
	bmpParams.hBmp = CreateDIBSection(hDC, pbmi, DIB_RGB_COLORS, &pbits, NULL, 0);
	if (bmpParams.hBmp == NULL)
		throw;		// out of memory;

	// Get the actual information from the DibSection
	DIBSECTION ds;
	GetObject(bmpParams.hBmp, sizeof(DIBSECTION), &ds);
#ifdef _DEBUG
	//
	// Validate DibSection Created
	ASSERT(ds.dsBmih.biBitCount == nBitCount);
	if (nBitCount == 24 && ds.dsBitfields[0] != 0)		// if field order is specified, make sure it is the default
	{
		ASSERT(ds.dsBitfields[0] == 0x00ff0000);
		ASSERT(ds.dsBitfields[1] == 0x0000ff00);
		ASSERT(ds.dsBitfields[2] == 0x000000ff);
	}
#endif

	bmpParams.nStride = PAD4(ds.dsBm.bmWidthBytes);
	BYTE* pRow = (BYTE*) pbits;
	if (ds.dsBmih.biHeight > 0)
	{
		// Bottom-Up DIB
		pRow += (ds.dsBmih.biHeight - 1) * bmpParams.nStride;	// Point to beginning of top scanline (last row of information)
		bmpParams.nStride = -bmpParams.nStride;					// Walk backwards
	}
	int nRows = abs(ds.dsBmih.biHeight);

	// Build the row pointer table in the correct order for the DIBSECTION
	bmpParams.ppScanLinePointers = new JSAMPROW[nRows];
	JSAMPARRAY ppRow = bmpParams.ppScanLinePointers;
	while (nRows--)
	{
		*ppRow = pRow;
		pRow += bmpParams.nStride;
		ppRow++;
	}

	delete [] (char*) pbmi;
}

// This routine deletes the temporary buffers created above
void DeleteTempBuffers(jpeg_decompress_struct* pcinfo, BitmapParameters& bmpParams)
{
	delete [] bmpParams.ppScanLinePointers;
	if (pcinfo->colormap != NULL)
	{
		delete [] pcinfo->colormap[0];			// Delete the colormap
		delete [] pcinfo->colormap;
	}
}

/* Expanded data source object for stdio input */

typedef struct {
	struct jpeg_source_mgr pub;	/* public fields */

	JOCTET*	pBuffer;		/* start of buffer */
	size_t	dwSizeBuffer;
	BOOL	bFakeLength;	/* don't know length! */
} my_source_mgr;

typedef my_source_mgr * my_src_ptr;


void InitJPEGSourceMgr(jpeg_decompress_struct* pcinfo, char* pFile, DWORD dwFileSize)
{
	ASSERT(pcinfo->src == NULL);

	my_source_mgr* pSrc = new my_source_mgr;
	pcinfo->src = (jpeg_source_mgr*) pSrc;

	pSrc->bFakeLength = dwFileSize == 0;
	if (pSrc->bFakeLength)
		dwFileSize = 0x7fffffff;		// maximum size

	pSrc->pBuffer = (JOCTET*) pFile;
	pSrc->dwSizeBuffer = (size_t) dwFileSize;
	pSrc->pub.next_input_byte = (BYTE*)pFile;
	pSrc->pub.bytes_in_buffer = pSrc->dwSizeBuffer;

	pSrc->pub.init_source = init_source;
	pSrc->pub.fill_input_buffer = fill_input_buffer;
	pSrc->pub.skip_input_data = skip_input_data;
	pSrc->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	pSrc->pub.term_source = term_source;
}


#ifdef _DEBUG

#define JMESSAGE(code,string)	string ,

const char * const jpeg_std_message_table[] = {
#include "jerror.h"
  NULL
};

#endif

/*
 * Error exit handler: must not return to caller.
 *
 * Applications may override this if they want to get control back after
 * an error.  Typically one would longjmp somewhere instead of exiting.
 * The setjmp buffer can be made a private field within an expanded error
 * handler object.  Note that the info needed to generate an error message
 * is stored in the error object, so you can generate the message now or
 * later, at your convenience.
 * You should make sure that the JPEG object is cleaned up (with jpeg_abort
 * or jpeg_destroy) at some point.
 */

METHODDEF(void) error_exit (j_common_ptr pcinfo)
{
	/* Always display the message */
	(*pcinfo->err->output_message) (pcinfo);

	/* Let the memory manager delete any temp files before we die */
	jpeg_destroy(pcinfo);

	throw;		// Bomb out of here!
}


/*
 * Actual output of an error or trace message.
 * Applications may override this method to send JPEG messages somewhere
 * other than stderr.
 */

METHODDEF(void) output_message (j_common_ptr pcinfo)
{
#ifdef _DEBUG
	char buffer[JMSG_LENGTH_MAX];

	/* Create the message */
	(*pcinfo->err->format_message) (pcinfo, buffer);

	/* Send it to stderr, adding a newline */
	ATLTRACE("%s\n", buffer);
#endif
}


/*
 * Decide whether to emit a trace or warning message.
 * msg_level is one of:
 *   -1: recoverable corrupt-data warning, may want to abort.
 *    0: important advisory messages (always display to user).
 *    1: first level of tracing detail.
 *    2,3,...: successively more detailed tracing messages.
 * An application might override this method if it wanted to abort on warnings
 * or change the policy about which messages to display.
 */

METHODDEF(void)
emit_message (j_common_ptr pcinfo, int msg_level)
{
	jpeg_error_mgr* err = pcinfo->err;

	if (msg_level < 0)
	{
		/* It's a warning message.  Since corrupt files may generate many warnings,
		 * the policy implemented here is to show only the first warning,
		 * unless trace_level >= 3.
		 */
		if (err->num_warnings == 0 || err->trace_level >= 3)
			(*err->output_message) (pcinfo);
		/* Always count warnings in num_warnings. */
		err->num_warnings++;
	}
	else
	{
		/* It's a trace message.  Show it if trace_level >= msg_level. */
		if (err->trace_level >= msg_level)
			(*err->output_message) (pcinfo);
	}
}


/*
 * Format a message string for the most recent JPEG error or message.
 * The message is stored into buffer, which should be at least JMSG_LENGTH_MAX
 * characters.  Note that no '\n' character is added to the string.
 * Few applications should need to override this method.
 */

METHODDEF(void) format_message (j_common_ptr pcinfo, char * buffer)
{
#ifdef _DEBUG
	struct jpeg_error_mgr * err = pcinfo->err;
	int msg_code = err->msg_code;
	const char * msgtext = NULL;
	const char * msgptr;
	char ch;
	boolean isstring;

	/* Look up message string in proper table */
	if (msg_code > 0 && msg_code <= err->last_jpeg_message)
	{
		msgtext = err->jpeg_message_table[msg_code];
	}
	else if (err->addon_message_table != NULL &&
		 msg_code >= err->first_addon_message &&
		 msg_code <= err->last_addon_message)
	{
		msgtext = err->addon_message_table[msg_code - err->first_addon_message];
	}

	/* Defend against bogus message number */
	if (msgtext == NULL)
	{
		err->msg_parm.i[0] = msg_code;
		msgtext = err->jpeg_message_table[0];
	}

	/* Check for string parameter, as indicated by %s in the message text */
	isstring = FALSE;
	msgptr = msgtext;
	while ((ch = *msgptr++) != '\0')
	{
		if (ch == '%')
		{
			if (*msgptr == 's')
				isstring = TRUE;
			break;
		}
	}

	/* Format the message into the passed buffer */
	if (isstring)
		wsprintf(buffer, msgtext, err->msg_parm.s);
	else
		wsprintf(buffer, msgtext,
			err->msg_parm.i[0], err->msg_parm.i[1],
			err->msg_parm.i[2], err->msg_parm.i[3],
			err->msg_parm.i[4], err->msg_parm.i[5],
			err->msg_parm.i[6], err->msg_parm.i[7]);
#endif
}


/*
 * Reset error state variables at start of a new image.
 * This is called during compression startup to reset trace/error
 * processing to default state, without losing any application-specific
 * method pointers.  An application might possibly want to override
 * this method if it has additional error processing state.
 */

METHODDEF(void) reset_error_mgr (j_common_ptr pcinfo)
{
  pcinfo->err->num_warnings = 0;
  /* trace_level is not reset since it is an application-supplied parameter */
  pcinfo->err->msg_code = 0;	/* may be useful as a flag for "no error" */
}


/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

METHODDEF(void) init_source (j_decompress_ptr pcinfo)
{
	my_src_ptr pSrc = (my_src_ptr) pcinfo->src;

	pSrc->pub.next_input_byte = pSrc->pBuffer;
	pSrc->pub.bytes_in_buffer = pSrc->dwSizeBuffer;
}


/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In typical applications, this should read fresh data into the buffer
 * (ignoring the current state of next_input_byte & bytes_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been reloaded.  It is not necessary to
 * fill the buffer entirely, only to obtain at least one more byte.
 *
 * There is no such thing as an EOF return.  If the end of the file has been
 * reached, the routine has a choice of ERREXIT() or inserting fake data into
 * the buffer.  In most cases, generating a warning message and inserting a
 * fake EOI marker is the best course of action --- this will allow the
 * decompressor to output however much of the image is there.  However,
 * the resulting error message is misleading if the real problem is an empty
 * input file, so we handle that case specially.
 *
 * In applications that need to be able to suspend compression due to input
 * not being available yet, a FALSE return indicates that no more data can be
 * obtained right now, but more may be forthcoming later.  In this situation,
 * the decompressor will return to its caller (with an indication of the
 * number of scanlines it has read, if any).  The application should resume
 * decompression after it has loaded more data into the input buffer.  Note
 * that there are substantial restrictions on the use of suspension --- see
 * the documentation.
 *
 * When suspending, the decompressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_input_byte & bytes_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point must be rescanned after resumption, so move it to
 * the front of the buffer rather than discarding it.
 */

//
// In this implementation, we've already read the entire image so a call to this
// indicates a bad file.
//
METHODDEF(boolean) fill_input_buffer (j_decompress_ptr pcinfo)
{
	my_src_ptr src = (my_src_ptr) pcinfo->src;

	if (src->pub.bytes_in_buffer != 0)
		return TRUE;		// Handle possible first call before any bytes have been read.


	WARNMS(pcinfo, JWRN_JPEG_EOF);
	/* Insert a fake EOI marker */
	src->pBuffer[0] = (JOCTET) 0xFF;
	src->pBuffer[1] = (JOCTET) JPEG_EOI;

	src->pub.next_input_byte = src->pBuffer;
	src->pub.bytes_in_buffer = 2;

  return TRUE;
}


/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */

METHODDEF(void) skip_input_data (j_decompress_ptr pcinfo, long num_bytes)
{
	my_src_ptr src = (my_src_ptr) pcinfo->src;

	if (num_bytes > 0)
	{
		if (num_bytes > (long) src->pub.bytes_in_buffer)
			num_bytes = (long) src->pub.bytes_in_buffer;

		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}


/*
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 */


/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

METHODDEF(void) term_source (j_decompress_ptr pcinfo)
{
  /* no work necessary here */
}

#else
HBITMAP LoadJPEGImage(HINSTANCE hInst, LPCSTR szItem, UINT fuLoad, HPALETTE hPal)
{
	HDC			hDC;
	BITMAPINFO	bmi;
	LPVOID		pbits;
	HBITMAP		hbm;
	void *		file;
	int			width, height;

	//
	// set "file" to a memory image of the JPEG file
	//
	if (hInst)
	{
		file=LoadResource(hInst, szItem);
	}
	else
	{
		if (fuLoad & LR_LOADFROMFILE)
		{
			file=LoadFileIntoMem(szItem, NULL);
		}
		else
		{
			file=(void *)szItem;
		}
	}

	//
	// process the JPEG header
	//
	if (!JPEG_process_headers(file, &width, &height))
	{
		return NULL;
	}

	//
	// create the bitmap
	//
	bmi.bmiHeader.biSize			= sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth			= width;
	bmi.bmiHeader.biHeight			= height;
	bmi.bmiHeader.biPlanes			= 1;
	bmi.bmiHeader.biBitCount		= 24;
	bmi.bmiHeader.biCompression		= BI_RGB;
	bmi.bmiHeader.biSizeImage		= 0;
	bmi.bmiHeader.biXPelsPerMeter	= 100;
	bmi.bmiHeader.biYPelsPerMeter	= 100;
	bmi.bmiHeader.biClrUsed			= 0;
	bmi.bmiHeader.biClrImportant	= 0;

	hDC=GetDC(NULL);
	pbits=NULL;
	hbm=CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pbits, NULL, 0);
	if (!(GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE))
	{
		hPal=NULL;
	}

	ReleaseDC(NULL, hDC);

	if (!hbm)
	{
		return NULL;
	}

	JPEG_decompress(pbits);

	JPEG_cleanup();
	if ((!hInst) && (fuLoad & LR_LOADFROMFILE))
	{
		free(file);
	}

	if (hPal)
	{
		LPVOID			pnewbits;
		struct {
		   BITMAPINFOHEADER bmiHeader;
		   RGBQUAD          bmiColors[256];
		} bmi2;
		PALETTEENTRY	pe[256];
		int				i;//,j;
		HBITMAP			hbm2;

		GetPaletteEntries(hPal, 0, 256, pe);
		for (i=0 ; i<256 ; i++)
		{
			bmi2.bmiColors[i].rgbRed		= pe[i].peRed;
			bmi2.bmiColors[i].rgbGreen		= pe[i].peGreen;
			bmi2.bmiColors[i].rgbBlue		= pe[i].peBlue;
			bmi2.bmiColors[i].rgbReserved	= 0;
		}

		bmi2.bmiHeader.biSize			= sizeof(bmi.bmiHeader);
		bmi2.bmiHeader.biWidth			= width;
		bmi2.bmiHeader.biHeight			= height;
		bmi2.bmiHeader.biPlanes			= 1;
		bmi2.bmiHeader.biBitCount		= 8;
		bmi2.bmiHeader.biCompression	= BI_RGB;
		bmi2.bmiHeader.biSizeImage		= 0;
		bmi2.bmiHeader.biXPelsPerMeter	= 100;
		bmi2.bmiHeader.biYPelsPerMeter	= 100;
		bmi2.bmiHeader.biClrUsed		= 0;
		bmi2.bmiHeader.biClrImportant	= 0;

		hDC=GetDC(NULL);
		hbm2=CreateDIBSection(hDC, (BITMAPINFO *)&bmi2, DIB_RGB_COLORS, &pnewbits, NULL, 0);
		ReleaseDC(NULL, hDC);

		//
		// now, copy the original bitmap to this one
		//
		HDC	hdc1, hdc2;

		hdc1=CreateCompatibleDC(NULL);
		hdc2=CreateCompatibleDC(NULL);
		SelectObject(hdc1, hbm);
		SelectObject(hdc2, hbm2);

		BitBlt(hdc2, 0, 0, width, height, hdc1, 0, 0, SRCCOPY);

		DeleteDC(hdc1);
		DeleteDC(hdc2);

		DeleteObject(hbm);
		hbm=hbm2;

	}

	//SaveDIBSection(hbm, "d:\\jpeg.bmp");

	return hbm;
}
#endif
