#include "VertexGenerator.h"

#include <zassert.h>

#include "UIVertexDefn.h"

//#define APPLY_VERTEX_POSITION_TEXTURING_OFFSET

////////////////////////////////////////////////////////////////////////////////////////////////////
// Class implemented as a singleton (mSingleInstance).
// Use static function Get() to get access to the single instance of this class.

CVertexGenerator CVertexGenerator::mSingleInstance;

////////////////////////////////////////////////////////////////////////////////////////////////////
CVertexGenerator::SPredefinedDynamicBufferConfig CVertexGenerator::m_sDynBufferConfig[ ePDBT_NumPredefinedDynamicBuffers ] =
{
	{ CVBIBManager::eBT_VertexDynamic, 4096, sizeof(UIVERTEX), D3DFVF_UIVERTEX },
	{ CVBIBManager::eBT_VertexDynamic, 4096, sizeof(UICOLOURFILLVERTEX), D3DFVF_UICOLOURVERT },
	{ CVBIBManager::eBT_VertexDynamic, 12288, sizeof(UIFONTVERTEX), D3DFVF_UIFONTVERTEX },
	{ CVBIBManager::eBT_VertexDynamic, 4096, sizeof(UIVERTEX), D3DFVF_UIVERTEX },
	{ CVBIBManager::eBT_VertexDynamic, 4096, sizeof(UIFONTVERTEX), D3DFVF_UIFONTVERTEX },
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// CVertexGenerator()
// Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
CVertexGenerator::CVertexGenerator()
{
	memset( &m_sVGState, 0, sizeof( SVertexGeneratorState ) );
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CVertexGenerator()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
CVertexGenerator::~CVertexGenerator()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialise()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVertexGenerator::Initialise( )
{
	int i;
	bool bResult;

	// Reset state.
	ZAssert( m_sVGState.bInitialised == false );

	// Create our predefined dynamic buffer resources.
	for( i=0; i<ePDBT_NumPredefinedDynamicBuffers; i++ )
	{
		switch( m_sDynBufferConfig[i].dwBufferType )
		{
		case CVBIBManager::eBT_VertexDynamic:
			bResult = CVBIBManager::Get()->AllocateDynamicVertexBuffer(
						&m_sVGState.hPredefinedDynBuffers[i],
						m_sDynBufferConfig[i].dwNumElements,
						m_sDynBufferConfig[i].dwElementSize,
						m_sDynBufferConfig[i].dwFormat );
			ZAssert( bResult == true );
			break;

		default:
			ZAssert( false && "Non dynamic or unsupported format supplied." );
		}
	}
	m_sVGState.bInitialised = true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GenerateUITexturedVertices()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVertexGenerator::GenerateUITexturedVertices( 	const TEXHANDLE hTexture,
													const WinRect & rectToDraw,
													const bool bSetStream )
{
	ZAssert( hTexture != INVALID_TEX_HANDLE );
	float fMinX, fMinY, fMaxX, fMaxY;
	float fU1, fV1;
    uint32_t dwWidth, dwHeight;
	UIVERTEX * pVertArray;

	CVRAMManager::Get()->GetOriginalDimensions( hTexture, &dwWidth, &dwHeight );

	if( CVBIBManager::Get()->LockDynamicVertexBuffer( 
//							&m_sVGState.hUITexVertsVB, 
							&m_sVGState.hPredefinedDynBuffers[ePDBT_UITexVB],
							4,
							(void**) &pVertArray ) == false )
	{
		// Failed to lock the vertex buffer.
		ZAssert( false );
		return;
	}

	// 2d texturing, adjust the vertex positions slightly.
	fMinX = (float) rectToDraw.Min().X() - 0.5f;
	fMinY = (float) rectToDraw.Min().Y() - 0.5f;
	fMaxX = (float) rectToDraw.Max().X() - 0.5f;
	fMaxY = (float) rectToDraw.Max().Y() - 0.5f;

	// Calculate our uv coords.
	fU1 = 1.0f;
	fV1 = 1.0f;
	if( rectToDraw.XSize() < (int)dwWidth )
	{
		fU1 = (float) rectToDraw.XSize() / (float) dwWidth;
	}
	if( rectToDraw.YSize() < (int)dwHeight )
	{
		fV1 = (float) rectToDraw.YSize() / (float) dwHeight;
	}

	pVertArray[0].x = fMinX;
	pVertArray[0].y = fMinY;
	pVertArray[0].z = 0.5f;
	pVertArray[0].rhw = 1.0f;
	pVertArray[0].fU = 0.0f;
	pVertArray[0].fV = 0.0f;

	pVertArray[1].x = fMaxX;
	pVertArray[1].y = fMinY;
	pVertArray[1].z = 0.5f;
	pVertArray[1].rhw = 1.0f;
	pVertArray[1].fU = fU1;
	pVertArray[1].fV = 0.0f;

	pVertArray[2].x = fMinX;
	pVertArray[2].y = fMaxY;
	pVertArray[2].z = 0.5f;
	pVertArray[2].rhw = 1.0f;
	pVertArray[2].fU = 0.0f;
	pVertArray[2].fV = fV1;

	pVertArray[3].x = fMaxX;
	pVertArray[3].y = fMaxY;
	pVertArray[3].z = 0.5f;
	pVertArray[3].rhw = 1.0f;
	pVertArray[3].fU = fU1;
	pVertArray[3].fV = fV1;

	// Finished adding verts, unlock.
	CVBIBManager::Get()->UnlockDynamicVertexBuffer( &m_sVGState.hPredefinedDynBuffers[ePDBT_UITexVB] );

	if( bSetStream == true )
	{
		CVBIBManager::Get()->SetVertexStream( &m_sVGState.hPredefinedDynBuffers[ePDBT_UITexVB] );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GenerateFillVertices()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVertexGenerator::GenerateFillVertices( const WinRect & rectToFill, const bool bSetStream, Color fillColour )
{
	D3DCOLOR d3dColor = D3DCOLOR_RGBA(	(DWORD) (fillColour.R() * 255),
										(DWORD) (fillColour.G() * 255),
										(DWORD) (fillColour.B() * 255),
										255 );
	GenerateFillVerticesD3DColor( rectToFill, bSetStream, d3dColor );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GenerateFillVerticesD3DColor()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVertexGenerator::GenerateFillVerticesD3DColor( const WinRect & rectToFill, const bool bSetStream, D3DCOLOR d3dColor )
{
	UICOLOURFILLVERTEX * pVertArray;
	if( CVBIBManager::Get()->LockDynamicVertexBuffer( 
							&m_sVGState.hPredefinedDynBuffers[ePDBT_UIFillVB],
							4,
							(void**) &pVertArray ) == false )
	{
		// Failed to lock the vertex buffer.
		ZAssert( false );
		return;
	}
	// 2d texturing, adjust the vertex positions slightly.
	float fMinX, fMinY, fMaxX, fMaxY;
	fMinX = (float) rectToFill.Min().X() - 0.5f;
	fMinY = (float) rectToFill.Min().Y() - 0.5f;
	fMaxX = (float) rectToFill.Max().X() - 0.5f;
	fMaxY = (float) rectToFill.Max().Y() - 0.5f;

	pVertArray[0].x		= fMinX;
	pVertArray[0].y		= fMinY;
	pVertArray[0].z		= 0.5f;
	pVertArray[0].rhw	= 1.0f;
	pVertArray[0].color	= d3dColor;

	pVertArray[1].x		= fMaxX;
	pVertArray[1].y		= fMinY;
	pVertArray[1].z		= 0.5f;
	pVertArray[1].rhw	= 1.0f;
	pVertArray[1].color	= d3dColor;

	pVertArray[2].x		= fMinX;
	pVertArray[2].y		= fMaxY;
	pVertArray[2].z		= 0.5f;
	pVertArray[2].rhw	= 1.0f;
	pVertArray[2].color	= d3dColor;

	pVertArray[3].x		= fMaxX;
	pVertArray[3].y		= fMaxY;
	pVertArray[3].z		= 0.5f;
	pVertArray[3].rhw	= 1.0f;
	pVertArray[3].color	= d3dColor;

	// Finished adding verts, unlock.
	CVBIBManager::Get()->UnlockDynamicVertexBuffer( &m_sVGState.hPredefinedDynBuffers[ePDBT_UIFillVB] );

	if( bSetStream == true )
	{
		CVBIBManager::Get()->SetVertexStream( &m_sVGState.hPredefinedDynBuffers[ePDBT_UIFillVB] );
	}
}
