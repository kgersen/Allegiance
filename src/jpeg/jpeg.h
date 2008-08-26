#pragma once

extern "C" HBITMAP __cdecl LoadJPEGImage(HINSTANCE hInst, LPCSTR szItem, UINT fuLoad, HPALETTE hPal);

/*
#ifndef CREATELIB
#if USE_IE_JPEG
#ifdef _DEBUG
#pragma comment (lib, "iejpegd.lib")
#else //DEBUG
#pragma comment (lib, "iejpeg.lib")
#endif
#else // USE_IE_JPEG
#ifdef _DEBUG
#pragma comment (lib, "jpegd.lib")
#else //DEBUG
#pragma comment (lib, "jpeg.lib")
#endif // _DEBUG
#endif // USE_IE_JPEG
#endif // CREATELIB
*/