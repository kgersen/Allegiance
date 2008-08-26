#include <windows.h>
#include "decom.h"
#include "jtypes.h"

#define wEndian(x) ( (((x) & 0xFF00) >> 8) | (((x) & 0x00FF) << 8) )
#define PI 3.1415926539f

#define maximum(a,b) (((a)>(b)) ? (a) : (b))
#define minimum(a,b) (((a)>(b)) ? (b) : (a))
#define PAD4(x) (((x)+3) & 0xFFFFFFFC)

//
// TIMER:
//  0 Time nothing
//  1 Time the IDCT transforms
//  2 Time the entire decompress process
//  3 Time the YUV -> RGB conversion
//
#define TIMER 0
#define USE_RANGE_CHEAT
#define USE_YUV_CHEAT
//#define USE_FLOAT

#ifdef USE_RANGE_CHEAT
const static int range_lim[0x400] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

#define range(x, mn, mx) range_lim[(x) & 0x3FF];
#else
#define range(x, mn, mx) ((x) < (mn) ? (mn) : (x) > (mx) ? (mx) : (x))
#endif

//
// we'll assume 10 bits of decimal precision (accurate to 1/1024)
//
#ifndef USE_FLOAT
#define PREC 8
//#define PRECISE_MATH
#define JPEGINT(x) ((int)(((float)(x)) * ((float)(1 << PREC))))

#ifdef PRECISE_MATH
#pragma warning (disable : 4035)
static __inline int MULT(int a, int b)
{
	__asm {
		mov		eax, a;
		mov		edx, b;
		imul	edx;
		shrd	eax, edx, PREC;
	}
	return;
}
#pragma warning (default : 4035)
#define MMULT(x,y) (((x) * (y)) >> PREC)
#else // PRECISE_MATH
#define MULT(x,y) (((x) * (y)) >> PREC)
#define MMULT MULT
#endif // PRECISE_MATH
#endif // USE_FLOAT

#if TIMER
static __int64 dct_timer;
static int dct_count;

#define	StartPerformanceTimer() _asm { \
	_asm push	eax \
	_asm push	ecx \
	_asm push	edx \
	_asm mov	ecx, offset dct_timer \
	_asm _emit	0x0F \
	_asm _emit	0x31 \
	_asm sub	[ecx], eax \
	_asm sbb	[ecx+4], edx \
	_asm pop	edx \
	_asm pop	ecx \
	_asm pop	eax \
}

#define	EndPerformanceTimer() _asm { \
	_asm push	eax \
	_asm push	ecx \
	_asm push	edx \
	_asm mov	ecx, offset dct_timer \
	_asm _emit	0x0F \
	_asm _emit	0x31 \
	_asm add	[ecx], eax \
	_asm adc	[ecx+4], edx \
	_asm pop	edx \
	_asm pop	ecx \
	_asm pop	eax \
}
#endif

//
// xval and yval contain the x and y coordinates of the zig-zag pattern.
//  For example, xval[5], yval[5] is 2,0 to
//  indicate that the sixth huffman coded entry (index 5) is at 2,0
//
const static char xval[]={
	0,
	1, 0,
	0, 1, 2,
	3, 2, 1, 0,
	0, 1, 2, 3, 4,
	5, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5, 6,
	7, 6, 5, 4, 3, 2, 1, 0,
	1, 2, 3, 4, 5, 6, 7,
	7, 6, 5, 4, 3, 2,
	3, 4, 5, 6, 7,
	7, 6, 5, 4,
	5, 6, 7,
	7, 6,
	7};

const static char yval[]={
	0,
	0, 1,
	2, 1, 0,
	0, 1, 2, 3,
	4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5,
	6, 5, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5, 6, 7,
	7, 6, 5, 4, 3, 2, 1,
	2, 3, 4, 5, 6, 7,
	7, 6, 5, 4, 3,
	4, 5, 6, 7,
	7, 6, 5,
	6, 7,
	7};

const static char jpeg_natural_order[] = {
  0,  1,  8, 16,  9,  2,  3, 10,
 17, 24, 32, 25, 18, 11,  4,  5,
 12, 19, 26, 33, 40, 48, 41, 34,
 27, 20, 13,  6,  7, 14, 21, 28,
 35, 42, 49, 56, 57, 50, 43, 36,
 29, 22, 15, 23, 30, 37, 44, 51,
 58, 59, 52, 45, 38, 31, 39, 46,
 53, 60, 61, 54, 47, 55, 62, 63,
};

#ifdef USE_FLOAT
const static float COS[] = {
	 1.000000f * 1.414213562f,	//  0
	 0.980785f * 1.414213562f,	//  1
	 0.923880f * 1.414213562f,	//  2
	 0.831470f * 1.414213562f,	//  3
	 0.707107f * 1.414213562f,	//  4
	 0.555570f * 1.414213562f,	//  5
	 0.382683f * 1.414213562f,	//  6
	 0.195090f * 1.414213562f,	//  7
	-0.000000f * 1.414213562f,	//  8
	-0.195090f * 1.414213562f,	//  9
	-0.382683f * 1.414213562f,	// 10
	-0.555570f * 1.414213562f,	// 11
	-0.707107f * 1.414213562f,	// 12
	-0.831470f * 1.414213562f,	// 13
	-0.923880f * 1.414213562f,	// 14
	-0.980785f * 1.414213562f,	// 15
	-1.000000f * 1.414213562f,	// 16
	-0.980785f * 1.414213562f,	// 17
	-0.923879f * 1.414213562f,	// 18
	-0.831470f * 1.414213562f,	// 19
	-0.707107f * 1.414213562f,	// 20
	-0.555570f * 1.414213562f,	// 21
	-0.382683f * 1.414213562f,	// 22
	-0.195090f * 1.414213562f,	// 23
	 0.000000f * 1.414213562f,	// 24
	 0.195090f * 1.414213562f,	// 25
	 0.382684f * 1.414213562f,	// 26
	 0.555570f * 1.414213562f,	// 27
	 0.707107f * 1.414213562f,	// 28
	 0.831470f * 1.414213562f,	// 29
	 0.923880f * 1.414213562f,	// 30
	 0.980785f * 1.414213562f,	// 31
};
#else
const static int COS[] = {
	JPEGINT( 1.000000f * 1.414213562f),	//  0
	JPEGINT( 0.980785f * 1.414213562f),	//  1
	JPEGINT( 0.923880f * 1.414213562f),	//  2
	JPEGINT( 0.831470f * 1.414213562f),	//  3
	JPEGINT( 0.707107f * 1.414213562f),	//  4
	JPEGINT( 0.555570f * 1.414213562f),	//  5
	JPEGINT( 0.382683f * 1.414213562f),	//  6
	JPEGINT( 0.195090f * 1.414213562f),	//  7
	JPEGINT(-0.000000f * 1.414213562f),	//  8
	JPEGINT(-0.195090f * 1.414213562f),	//  9
	JPEGINT(-0.382683f * 1.414213562f),	// 10
	JPEGINT(-0.555570f * 1.414213562f),	// 11
	JPEGINT(-0.707107f * 1.414213562f),	// 12
	JPEGINT(-0.831470f * 1.414213562f),	// 13
	JPEGINT(-0.923880f * 1.414213562f),	// 14
	JPEGINT(-0.980785f * 1.414213562f),	// 15
	JPEGINT(-1.000000f * 1.414213562f),	// 16
	JPEGINT(-0.980785f * 1.414213562f),	// 17
	JPEGINT(-0.923879f * 1.414213562f),	// 18
	JPEGINT(-0.831470f * 1.414213562f),	// 19
	JPEGINT(-0.707107f * 1.414213562f),	// 20
	JPEGINT(-0.555570f * 1.414213562f),	// 21
	JPEGINT(-0.382683f * 1.414213562f),	// 22
	JPEGINT(-0.195090f * 1.414213562f),	// 23
	JPEGINT( 0.000000f * 1.414213562f),	// 24
	JPEGINT( 0.195090f * 1.414213562f),	// 25
	JPEGINT( 0.382684f * 1.414213562f),	// 26
	JPEGINT( 0.555570f * 1.414213562f),	// 27
	JPEGINT( 0.707107f * 1.414213562f),	// 28
	JPEGINT( 0.831470f * 1.414213562f),	// 29
	JPEGINT( 0.923880f * 1.414213562f),	// 30
	JPEGINT( 0.980785f * 1.414213562f),	// 31
};
#endif // USE_FLOAT

static HUFFTABLE *huff[2][2];	// class (0: DC, 1: AC), id
static MATRIX qtable[2];
static MATRIX m;
static WORD wRestart;

static BITSTREAM	bs;
static FRAME		*frame;

#ifdef USE_FLOAT
extern "C" long _ftol(float);
extern "C" const void _fltused(void);

const void _fltused(void)
{
}

long _ftol(float inp)
{
	DWORD result1[2];

	__asm {
		wait;
		fistp       qword ptr [result1];
	}

	return result1[0];
}
#endif

void JPEG_cleanup(void)
{
	FREE(huff[0][0]);
	FREE(huff[0][1]);
	FREE(huff[1][0]);
	FREE(huff[1][1]);
	frame=NULL;

}

BOOL JPEG_process_headers(void *file, int *width, int *height)
{
	BYTE		*p;
	BOOL		bEOF=FALSE;
	JFIFHEAD	*jfif;
	WORD		Length;
	int			i,j,n;

	jfif=(JFIFHEAD *)file;

	huff[0][0] = NULL;
	huff[0][1] = NULL;
	huff[1][0] = NULL;
	huff[1][1] = NULL;
	frame = NULL;
	wRestart = 0;

	if (!file)
		return NULL;

	//
	// make sure we have a valid JFIF file
	//
	if (
		(*(DWORD *)&jfif->SOI != 0xE0FFD8FF) ||
		(*(DWORD *)&jfif->Identifier != 'FIFJ') ||
		(jfif->Version[0] != 1) )
	{
		SetLastError(ERR_INVALID_JPEG_FILE);
		return FALSE;
	}

	p = (BYTE *)file + 4 + wEndian(jfif->Length);
	Length=wEndian(jfif->Length);

	while (!bEOF)
	{
		if (p[0] == 0xFF)
		{
			p++;

			switch (p[0])
			{
			case 0xDB:	// define quantization tables
				p++;
				Length = wEndian(*(WORD *)p);
				p += 2;

				n=0;
				while (n < Length - 2)
				{
					if (p[n] >> 4)
					{
						for (j=0 ; j<8 ; j++)
						{
							for (i=0 ; i<8 ; i++)
							{
								((COEF *)(qtable[p[n]]))
									[jpeg_natural_order[i+j*8]] =
									((COEF *)(&p[1 + n]))[i + j*8];
							}
						}
					}
					else
					{
						for (j=0 ; j<8 ; j++)
						{
							for (i=0 ; i<8 ; i++)
							{
								((COEF *)(qtable[p[n]]))
									[jpeg_natural_order[i+j*8]] =
									((char *)(&p[1 + n]))[i + j*8];
							}
						}
					}

					n += 1 + ((p[n] + 1) * 64);
				}

				p += Length - 2;

				break;

			case 0xC0:	// baseline DCT frane, huffman coded
				p++;
				Length = wEndian(*(WORD *)p);
				frame = (FRAME *)(p+2);
				p += Length;

				*width = wEndian(frame->wWidth);
				*height = wEndian(frame->wHeight);
				break;

			case 0xC4:	// define huffman tables
				p++;
				Length = wEndian(*(WORD *)p);
				p += 2;
				i=0;


				while (i < Length -2 )
				{
					int huffclass=p[i] >> 4;
					int huffid=p[i] & 0x0F;

					//
					// get the number of entries in the huffman table
					//
					int cnt;
					for (j=0, cnt=0 ; j<16 ; j++)
					{
						cnt += p[i + j + 1];
					}

					huff[huffclass][huffid] = (HUFFTABLE *)malloc(sizeof(HUFFTABLE) + cnt * sizeof(HUFFCODE) );

					if (!huff)
					{
						SetLastError(ERR_COULDNT_ALLOCATE_MEMORY);
						return FALSE;
					}

					//
					// for getting huffman codes
					//
					int curbitcount=1;
					int curnumber=0;
					int num_of_cur_bitcount=0;

					huff[huffclass][huffid]->size = cnt;

					huff[huffclass][huffid]->first9bit = 0;

					for (j=0 ; j<cnt ; j++)
					{
						while (num_of_cur_bitcount >= p[i + curbitcount])
						{
							num_of_cur_bitcount=0;
							curbitcount++;
							curnumber <<= 1;
						}

						//
						// set a marker for the first huffman code that's 9 bits in length
						// (the first one that we can't quick-lookup)
						//
						if ((curbitcount == 9) && (huff[huffclass][huffid]->first9bit))
						{
							huff[huffclass][huffid]->first9bit = j;
						}

						huff[huffclass][huffid]->code[j].dwBits		= curnumber << (32 - curbitcount);
						huff[huffclass][huffid]->code[j].dwMask		= (int)0x80000000 >> (curbitcount - 1);
						huff[huffclass][huffid]->code[j].value		= (char)p[i + 17 + j];
						huff[huffclass][huffid]->code[j].numbits	= (char)curbitcount;

						num_of_cur_bitcount++;
						curnumber++;
					}
					i += 17 + cnt;

					//
					// generate quick lookups
					//
					for (j=0 ; j<256 ; j++)
					{
						int i;

						for (i=0 ; i<huff[huffclass][huffid]->size && huff[huffclass][huffid]->code[i].numbits <=8 ; i++)
						{
							if (((j<<24 & huff[huffclass][huffid]->code[i].dwMask) ==
								huff[huffclass][huffid]->code[i].dwBits) &&
								(!(huff[huffclass][huffid]->code[i].dwBits & 0x00FFFFFF)))
								break;
						}
						if ((i<huff[huffclass][huffid]->size) && (huff[huffclass][huffid]->code[i].numbits<=8))
							huff[huffclass][huffid]->quick_lookup[j] = i;
						else
							huff[huffclass][huffid]->quick_lookup[j] = -1;
					}
				}

				p += Length - 2;
				break;

			case 0xDD:	// define restart interval
				p++;
				wRestart = wEndian(*(WORD *)(p+2));
				Length = wEndian(*(WORD *)p);
				p += Length;
				break;

			case 0xDA:	// define start of scan
				p++;
				Length = wEndian(*(WORD *)p);
				p += Length;

				//
				// set up the bit stream
				//
				bs.dwBits = 0;
				bs.p = (BYTE *)p;
				bs.nEmpty = 0;

				bs.dwBits <<= 8; bs.dwBits |= *bs.p; bs.p++;
				bs.dwBits <<= 8; bs.dwBits |= *bs.p; bs.p++;
				bs.dwBits <<= 8; bs.dwBits |= *bs.p; bs.p++;
				bs.dwBits <<= 8; bs.dwBits |= *bs.p; bs.p++;

				break;

			case 0xD9:	// end of image
				bEOF=TRUE;
				break;

			case 0xC1:
			case 0xC2:
			case 0xC3:
			case 0xC5:
			case 0xC6:
			case 0xC7:
				SetLastError(ERR_EXTENDED_JPEG_FORMAT_NOT_SUPPORTED);
				return FALSE;

			case 0xE0:
			case 0xE1:
			case 0xE2:
			case 0xE3:
			case 0xE4:
			case 0xE5:
			case 0xE6:
			case 0xE7:
			case 0xE8:
			case 0xE9:
			case 0xEA:
			case 0xEB:
			case 0xEC:
			case 0xED:
			case 0xEE:
			case 0xEF:
				p++;
				Length = wEndian(*(WORD *)p);
				p += Length;
				break;

			case 0xD0:
			case 0xD1:
			case 0xD2:
			case 0xD3:
			case 0xD4:
			case 0xD5:
			case 0xD6:
			case 0xD7:
				p++;
				break;

			case 0xFE:
				// comment
				p++;
				Length = wEndian(*(WORD *)p);
				p += Length;
				break;

			case 0x00:
				break;

			case 0xFF:
				break;

			default:
				SetLastError(-1);
				return FALSE;
				break;
			}
		}
		else
		{
			p++;
		}
	}

	if (!frame)
		return FALSE;

	return TRUE;
}

static __inline int get_huffcode(BITSTREAM *bs, HUFFTABLE *ht)
{
	int i;

	if ((i=ht->quick_lookup[bs->dwBits >> 24]) == -1)
	{
		//
		// we couldn't do a quick-lookup, so go through the list
		//
		//for (i=0 ; i<ht->size ; i++)
		for (i=ht->first9bit ; i<ht->size ; i++)
		{
			if ((bs->dwBits & ht->code[i].dwMask) == ht->code[i].dwBits)
				break;
		}
	}
	if (i >= ht->size)
		return 0;

	//
	// we've found a match at i, so remove ht->code[i].numbits
	//
	bs->dwBits <<= ht->code[i].numbits;
	bs->nEmpty += ht->code[i].numbits;

	//
	// handle the need to read another byte into dwBits
	//
	while (bs->nEmpty >= 8)
	{
		int temp;

		temp=*(bs->p);
		(bs->p)++;

		while (temp == 0xFF)
		{
			if (*(bs->p) == 0x00)
			{
				(bs->p)++;
				break;
			}
			else
			{
				(bs->p)++;
				temp=*(bs->p);
				(bs->p)++;
			}
		}

		temp <<= (bs->nEmpty - 8);
		bs->dwBits |= temp;
		bs->nEmpty -= 8;
	}

	return ht->code[i].value;
}

static /*__inline*/ int get_vli(BITSTREAM *bs, int numbits)
{
	DWORD temp;
	int mask;
	int negative;
	int	retval;

	//
	// create a mask
	//
	mask=0x80000000;
	if (bs->dwBits & mask)
		negative=0;
	else
		negative=1;

	mask >>= (numbits - 1);
	temp = bs->dwBits & mask;

	if (negative)
		temp ^= mask;

	temp >>= (32 - numbits);

	retval = (int) temp;

	if (negative)
		retval = -retval;

	//
	// now remove the bits from the bitstream
	//
	bs->dwBits <<= numbits;
	bs->nEmpty += numbits;

	//
	// handle the need to read another byte into dwBits
	//
	while (bs->nEmpty >= 8)
	{
		int temp;

		temp=*(bs->p);
		(bs->p)++;
		while (temp == 0xFF)
		{
			if (*(bs->p) == 0x00)
			{
				(bs->p)++;
				break;
			}
			else
			{
				(bs->p)++;
				temp=*(bs->p);
				(bs->p)++;
			}
		}

		temp <<= (bs->nEmpty - 8);
		bs->dwBits |= temp;
		bs->nEmpty -= 8;
	}

	return retval;
}


#ifdef USE_FLOAT
void __inline IDCT(MATRIX *m, MATRIX *q)
{
	int i,j;
	float	fm[8][8];
	float	ws[8][8];
	float	_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q;

	for (j=0 ; j<8 ; j++)
	{
		for (i=0 ; i<8 ; i++)
		{
			float Cu,Cv;

			Cu = (i==0) ? 1.0f : COS[i & 31];
			Cv = (j==0) ? 1.0f : COS[j & 31];

			fm[i][j] = (float)((*m)[i][j]) * (float)((*q)[i][j]) * Cu * Cv;
		}
	}

	for (i=0 ; i<8 ; i++)
	{
		// Even part
		_a = fm[i][0];
		_b = fm[i][2];
		_c = fm[i][4];
		_d = fm[i][6];

		_i = _a + _c;	// phase 3
		_j = _a - _c;

		_l = _b + _d;	// phases 5-3
		_k = (_b - _d) * (1.414213562f) - _l; // 2*c4

		_a = _i + _l;	// phase 2
		_d = _i - _l;
		_b = _j + _k;
		_c = _j - _k;

		// Odd part
		_e = fm[i][1];
		_f = fm[i][3];
		_g = fm[i][5];
		_h = fm[i][7];

		_q = _g + _f;		// phase 6
		_n = _g - _f;
		_o = _e + _h;
		_p = _e - _h;

		_h = _o + _q;		// phase 5
		_j = (_o - _q) * (1.414213562f); // 2*c4

		_m = (_n + _p) * (1.847759065f); // 2*c2
		_i = (1.082392200f) * _p - _m; // 2*(c2-c6)
		_k = (-2.613125930f) * _n + _m; // -2*(c2+c6)

		_g = _k - _h;	// phase 2
		_f = _j - _g;
		_e = _i + _f;

		ws[i][0] = _a + _h;
		ws[i][7] = _a - _h;
		ws[i][1] = _b + _g;
		ws[i][6] = _b - _g;
		ws[i][2] = _c + _f;
		ws[i][5] = _c - _f;
		ws[i][4] = _d + _e;
		ws[i][3] = _d - _e;

	}

	for (j=0 ; j<8 ; j++)
	{
		// Even part

		_i = ws[0][j] + ws[4][j];
		_j = ws[0][j] - ws[4][j];

		_l = ws[2][j] + ws[6][j];
		_k = (ws[2][j] - ws[6][j]) * (1.414213562f) - _l;

		_a = _i + _l;
		_d = _i - _l;
		_b = _j + _k;
		_c = _j - _k;

		// Odd part

		_q = ws[5][j] + ws[3][j];
		_n = ws[5][j] - ws[3][j];
		_o = ws[1][j] + ws[7][j];
		_p = ws[1][j] - ws[7][j];

		_h = _o + _q;
		_j = (_o - _q) * (1.414213562f);

		_m = (_n + _p) * (1.847759065f); // 2*c2
		_i = (1.082392200f) * _p - _m; // 2*(c2-c6)
		_k = (-2.613125930f) * _n + _m; // -2*(c2+c6)

		_g = _k - _h;
		_f = _j - _g;
		_e = _i + _f;

		// Final output stage: scale down by a factor of 8 and range-limit

		(*m)[0][j] = range(((((int)(_a + _h)) << 19) >> 22) + 128, 0, 255);
		(*m)[7][j] = range(((((int)(_a - _h)) << 19) >> 22) + 128, 0, 255);
		(*m)[1][j] = range(((((int)(_b + _g)) << 19) >> 22) + 128, 0, 255);
		(*m)[6][j] = range(((((int)(_b - _g)) << 19) >> 22) + 128, 0, 255);
		(*m)[2][j] = range(((((int)(_c + _f)) << 19) >> 22) + 128, 0, 255);
		(*m)[5][j] = range(((((int)(_c - _f)) << 19) >> 22) + 128, 0, 255);
		(*m)[4][j] = range(((((int)(_d + _e)) << 19) >> 22) + 128, 0, 255);
		(*m)[3][j] = range(((((int)(_d - _e)) << 19) >> 22) + 128, 0, 255);

	}

	return;

}
#else

void __inline IDCT(MATRIX *m, MATRIX *q)
{
	int i,j;
	int		fm[8][8];
	int		ws[8][8];
	int		_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q;

#if TIMER==1
	dct_count++;
	StartPerformanceTimer();
#endif

	for (j=0 ; j<8 ; j++)
	{
		for (i=0 ; i<8 ; i++)
		{
			int Cu,Cv;
	
			Cu = (i==0) ? JPEGINT(1.0f) : COS[i & 31];
			Cv = (j==0) ? JPEGINT(1.0f) : COS[j & 31];

			fm[i][j] = (int)((*m)[i][j]) * (int)((*q)[i][j]) * MMULT(Cu, Cv);
		}
	}

	for (i=0 ; i<8 ; i++)
	{
		// Even part

		_a = fm[i][0];
		_b = fm[i][2];
		_c = fm[i][4];
		_d = fm[i][6];

		_i = _a + _c;	// phase 3
		_j = _a - _c;

		_l = _b + _d;	// phases 5-3
		_k = MULT(_b - _d, JPEGINT(1.414213562f)) - _l; // 2*c4

		_a = _i + _l;	// phase 2
		_d = _i - _l;
		_b = _j + _k;
		_c = _j - _k;

		// Odd part

		_e = fm[i][1];
		_f = fm[i][3];
		_g = fm[i][5];
		_h = fm[i][7];

		_q = _g + _f;		// phase 6
		_n = _g - _f;
		_o = _e + _h;
		_p = _e - _h;

		_h = _o + _q;		// phase 5
		_j = MULT(_o - _q, JPEGINT(1.414213562f)); // 2*c4

		_m = MULT((_n + _p), JPEGINT(1.847759065f)); // 2*c2
		_i = MULT(JPEGINT(1.082392200f), _p) - _m; // 2*(c2-c6)
		_k = MULT(JPEGINT(-2.613125930f), _n) + _m; // -2*(c2+c6)

		_g = _k - _h;	// phase 2
		_f = _j - _g;
		_e = _i + _f;

		ws[i][0] = _a + _h;
		ws[i][7] = _a - _h;
		ws[i][1] = _b + _g;
		ws[i][6] = _b - _g;
		ws[i][2] = _c + _f;
		ws[i][5] = _c - _f;
		ws[i][4] = _d + _e;
		ws[i][3] = _d - _e;

	}

	for (j=0 ; j<8 ; j++)
	{
		// Even part

		_i = ws[0][j] + ws[4][j];
		_j = ws[0][j] - ws[4][j];

		_l = ws[2][j] + ws[6][j];
		_k = MULT((ws[2][j] - ws[6][j]), JPEGINT(1.414213562f)) - _l;

		_a = _i + _l;
		_d = _i - _l;
		_b = _j + _k;
		_c = _j - _k;

		// Odd part

		_q = ws[5][j] + ws[3][j];
		_n = ws[5][j] - ws[3][j];
		_o = ws[1][j] + ws[7][j];
		_p = ws[1][j] - ws[7][j];

		_h = _o + _q;
		_j = MULT((_o - _q), JPEGINT(1.414213562f));

		_m = MULT((_n + _p), JPEGINT(1.847759065f)); // 2*c2
		_i = MULT(JPEGINT(1.082392200f), _p) - _m; // 2*(c2-c6)
		_k = MULT(JPEGINT(-2.613125930f), _n) + _m; // -2*(c2+c6)

		_g = _k - _h;
		_f = _j - _g;
		_e = _i + _f;

		// Final output stage: scale down by a factor of 8 and range-limit

		(*m)[0][j] = range(((((int)(_a + _h)) << (19 - PREC)) >> 22) + 128, 0, 255);
		(*m)[7][j] = range(((((int)(_a - _h)) << (19 - PREC)) >> 22) + 128, 0, 255);
		(*m)[1][j] = range(((((int)(_b + _g)) << (19 - PREC)) >> 22) + 128, 0, 255);
		(*m)[6][j] = range(((((int)(_b - _g)) << (19 - PREC)) >> 22) + 128, 0, 255);
		(*m)[2][j] = range(((((int)(_c + _f)) << (19 - PREC)) >> 22) + 128, 0, 255);
		(*m)[5][j] = range(((((int)(_c - _f)) << (19 - PREC)) >> 22) + 128, 0, 255);
		(*m)[4][j] = range(((((int)(_d + _e)) << (19 - PREC)) >> 22) + 128, 0, 255);
		(*m)[3][j] = range(((((int)(_d - _e)) << (19 - PREC)) >> 22) + 128, 0, 255);

	}

#if TIMER==1
	EndPerformanceTimer();
#endif

	return;

}
#endif // USE_FLOAT

#ifdef USE_YUV_CHEAT
void __inline YUV2RGB(int maxw, int maxh, void *pbits, int span)
{
	register int i,j;

	//
	// convert from YUV to RGB
	//
	register BYTE *pb;

	for (j=0 ; j<maxh ; j++)
	{
		pb=(BYTE *)((int)pbits + j*span);

		for (i=0 ; i<maxw ; i++)
		{
			int r,g,b,y,u,v;

			y=(int)(pb[0]);
			v=(int)(pb[1]);
			u=(int)(pb[2]);

			u -= 128;
			v -= 128;

			r=(65536*y + 91881*v + 32768) >> 16;
			g=(65536*y - 22554*u - 46802*v + 32768) >> 16;
			b=(65536*y + 116130*u + 32768) >> 16;

			*pb++=(BYTE)range(r, 0, 255);
			*pb++=(BYTE)range(g, 0, 255);
			*pb++=(BYTE)range(b, 0, 255);

		}
	}

	return;
}
void __inline YUV2GS(int maxw, int maxh, void *pbits, int span)
{
	register int i,j;

	//
	// convert from YUV to RGB
	//
	register BYTE *pb;

	for (j=0 ; j<maxh ; j++)
	{
		pb=(BYTE *)((int)pbits + j*span);

		for (i=0 ; i<maxw ; i++)
		{
			register BYTE y=*pb++;
			
			*pb++=y;
			*pb++=y;

		}
	}

	return;
}
#else
void __inline YUV2RGB(int maxw, int maxh, void *pbits, int span)
{
	register int i,j;

	//
	// convert from YUV to RGB
	//
	for (j=0 ; j<maxh ; j++)
	{
		for (i=0 ; i<maxw ; i++)
		{
    		register int offset;
			int r,g,b,y,u,v;

			offset= i*3 + j*span;
			
			y=(int)((BYTE *)pbits)[offset + 0];
			v=(int)((BYTE *)pbits)[offset + 1];
			u=(int)((BYTE *)pbits)[offset + 2];

			u -= 128;
			v -= 128;

			r=(65536*y + 91881*v + 32768) >> 16;
			g=(65536*y - 22554*u - 46802*v + 32768) >> 16;
			b=(65536*y + 116130*u + 32768) >> 16;

			((BYTE *)pbits)[offset + 0]=(BYTE)range(r, 0, 255);
			((BYTE *)pbits)[offset + 1]=(BYTE)range(g, 0, 255);
			((BYTE *)pbits)[offset + 2]=(BYTE)range(b, 0, 255);

		}
	}

	return;
}
#endif


static volatile int baba=0;

BOOL JPEG_decompress(void *pbits)
{
	int	i,j;
	int	u,v,w,h;
	int	s,r;
	int k;
	int	last_dc[3];
	int maxw, maxh, span;
	WORD mcu_restart;

#if TIMER==1
	dct_timer = 0;
	dct_count =0;
#endif

#if TIMER==2
	dct_timer = 0;
	StartPerformanceTimer();
#endif

	//
	// calculate the largest component hor and ver samp factors
	//
	int hsf, vsf;
	for (hsf=0, vsf=0, i=0 ; i<frame->nComps ; i++)
	{
		if ( ((frame->comp[i].samp_factor >> 4) & 0x0F) > hsf)
			hsf = ((frame->comp[i].samp_factor >> 4) & 0x0F);
		if ( ((frame->comp[i].samp_factor     ) & 0x0F) > vsf)
			vsf = ((frame->comp[i].samp_factor     ) & 0x0F);
	}

	//
	// isize and jsize are the hor and ver number of pixels to copy
	//
	int isize[3], jsize[3], comp;
	for (comp=0 ; comp<frame->nComps ; comp++)
	{
		isize[comp] = hsf / ((frame->comp[comp].samp_factor >> 4) & 0x0F);
		jsize[comp] = vsf / ((frame->comp[comp].samp_factor     ) & 0x0F);
	}

	//
	// now set up w and h to be the number of hor and ver 8x8 blocks
	//
	w = ((wEndian(frame->wWidth) + 7) >> 3);
	h = ((wEndian(frame->wHeight) + 7) >> 3);

	//
	// now set up w and h to be the number of horizontal and vertical mcus
	//
	w = (w + hsf - 1) / hsf;
	h = (h + vsf - 1) / vsf;

	last_dc[0]=0;
	last_dc[1]=0;
	last_dc[2]=0;
	mcu_restart=0;

	maxw=wEndian(frame->wWidth);
	maxh=wEndian(frame->wHeight);
	span=PAD4(maxw*3);

	for (v=0 ; v<(h) ; v++)
	{
		for (u=0 ; u<w ; u++)
		{
			if ((wRestart) && (mcu_restart == wRestart))
			{
				mcu_restart=0;
				last_dc[0]=0;
				last_dc[1]=0;
				last_dc[2]=0;

				//
				// align bits on a byte-boundary
				//
				if (bs.nEmpty)
					get_vli(&bs, 8-(bs.nEmpty));

			}
			mcu_restart++;

			for (comp=0 ; comp<frame->nComps ; comp++)
			{
				int xc,yc;

				for (yc=0 ; yc<(frame->comp[comp].samp_factor & 0x0F) ; yc++)
				{
					for (xc=0 ; xc<((frame->comp[comp].samp_factor >> 4) & 0x0F) ; xc++)
					{
						int huffid;
						MATRIX *q;

						huffid=!!comp;
						q=&qtable[!!comp];

						//
						// clear out the matrix
						//
						for (j=0 ; j<8 ; j++)
						{
							for (i=0 ; i<8 ; i++)
							{
								m[i][j]=0;
							}
						}

						s=get_huffcode(&bs, huff[0][huffid]);

						if (s)
						{
							s=get_vli(&bs, s);
						}

						m[0][0]=s + last_dc[comp];
						last_dc[comp] = m[0][0];

						//
						// get AC values
						//
						for (k=1 ; k<64 ; k++)
						{
							s=get_huffcode(&bs, huff[1][huffid]);

							if (s)
							{
								r = ((BYTE)s) >> 4;
								s = s & 0x0F;

								if ((r == 15) && (s==0))
								{
									k += 15;
								}
								else
								{
									k += r;
									s = get_vli(&bs, s);
									m[xval[k]][yval[k]] = s;
								}
							}
							else
							{
								k=64;
							}
						}

						IDCT(&m, q);

						//
						// emit the byte(s)
						//
						for (j=0 ; j<8 ; j++)
						{
							for (i=0 ; i<8 ; i++)
							{
								int w, h;
								BYTE *p;

								w = (((u*hsf) + xc)*8 + i*isize[comp]);
								h = (((v*vsf) + yc)*8 + j*jsize[comp]);
								h = maxh - 1 - h;

								p = &(((BYTE *)pbits)[3*w + h*span + comp]);

								for (int jr=0 ; jr<jsize[comp] ; jr++)
								{
									for (int ir=0 ; ir<isize[comp] ; ir++)
									{
										if (((w+ir)<maxw) && (h>=jr))
										{
											p[ir*3 - jr*span] = (BYTE)m[i][j];
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

#if TIMER==3
	StartPerformanceTimer();
#endif

	if (frame->nComps == 1)
		YUV2GS(maxw, maxh, pbits, span);
	else
		YUV2RGB(maxw, maxh, pbits, span);

#if TIMER==1
	if (1)
	{
		char string[1024];
		double a = (double)dct_timer;
		double b = (double)dct_count;
		a /= b;
		wsprintf(string, "Took %d cycles per DCT\n", (int)(a));
		OutputDebugString(string);
	}
#endif
#if TIMER==2
	EndPerformanceTimer();
	if (1)
	{
		char string[1024];
		double a = (double)dct_timer;
		wsprintf(string, "Took %d cycles total\n", (int)(a));
		OutputDebugString(string);
	}
#endif
#if TIMER==3
	EndPerformanceTimer();
	if (1)
	{
		char string[1024];
		double a = (double)dct_timer;
		wsprintf(string, "Took %d cycles total\n", (int)(a));
		OutputDebugString(string);
	}
#endif


	return TRUE;
}


extern "C" void do_idct(void *, void *);

void do_idct(void *p1, void *p2)
{
	IDCT((MATRIX *)p1, (MATRIX *)p2);
}

