#pragma once
#pragma warning (disable : 4200)
#pragma pack(1)

typedef struct tag_JFIFHeader {
	BYTE	SOI[2];
	BYTE	APP0[2];
	WORD	Length;
	BYTE	Identifier[5];
	BYTE	Version[2];
	BYTE	Units;
	WORD	XDensity;
	WORD	YDensity;
	BYTE	XThumbnail;
	BYTE	YThumbnail;
} JFIFHEAD;

typedef struct tag_JFIFExtension {
	BYTE	APP0[2];
	WORD	Length;
} JFIFEXTENSION;


//
// HUFFCODE and HUFFTABLE store the data needed to decode huffman encoded data.
//
typedef struct tagHUFFCODE {
	DWORD	dwBits;
	DWORD	dwMask;
	char	value;
	char	numbits;
} HUFFCODE;

typedef struct tagHUFFMAN_TABLE {
	int			size;
	int			first9bit;
	short		quick_lookup[256];
	HUFFCODE	code[];
} HUFFTABLE;

//
// a bitstream is basically just a structure containing information needed to extract variable numbers of bits
//
typedef struct tagBITSTREAM {
	DWORD	dwBits;		// holds from 24--32 of the latest bits. bit 31 (the MSB) is always the next bit to extract
	BYTE	*p;			// *p is always the next 8 bits that will go into dwBits
	int		nEmpty;		// holds the number of bits at the bottom of dwBits that are not being currently used. when
						// this value reaches 8, we need to grab another byte (8 bits) from *p and increment p.
} BITSTREAM;


typedef struct tagCOMP {
	BYTE	id;
	BYTE	samp_factor;
	BYTE	table;
} COMP;

typedef struct tagFRAME {
	BYTE	prec;
	WORD	wHeight;
	WORD	wWidth;
	BYTE	nComps;
	COMP	comp[];
} FRAME;

typedef signed short COEF;

typedef COEF MATRIX[8][8];

#ifndef FREE
#define FREE(x) { if (x) { free(x); (x)=NULL; } }
#endif
