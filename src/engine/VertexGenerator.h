
#ifndef _VERTEXGENERATOR_H_
#define _VERTEXGENERATOR_H_

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
class CVertexGenerator
{
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	////////////////////////////////////////////////////////////////////////////////////////////
	enum EPredefinedDynamicBufferType
	{
		// UI dynamic buffers.
		ePDBT_UITexVB = 0,
		ePDBT_UIFillVB,
		ePDBT_UIFontVB,
		
		// 3D dynamic buffers.
		ePDBT_D3DVB,
		ePDBT_D3DLitVB,

		ePDBT_NumPredefinedDynamicBuffers
	};

////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	struct SPredefinedDynamicBufferConfig
	{
		DWORD dwBufferType;
		DWORD dwNumElements;
		DWORD dwElementSize;
		DWORD dwFormat;
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SVertexGeneratorState
	{
		bool bInitialised;

		CVBIBManager::SVBIBHandle hPredefinedDynBuffers[ePDBT_NumPredefinedDynamicBuffers];
//		CVBIBManager::SVBIBHandle hUITexVertsVB;
//		CVBIBManager::SVBIBHandle hUIFillVertsVB;
//		CVBIBManager::SVBIBHandle hUIFontVertsVB;
//		CVBIBManager::SVBIBHandle hD3DVB;
//		CVBIBManager::SVBIBHandle hD3DLitVB;

	};

	////////////////////////////////////////////////////////////////////////////////////////////
	static SPredefinedDynamicBufferConfig sDynBufferConfig[ ePDBT_NumPredefinedDynamicBuffers ];
	static SVertexGeneratorState sVGState;

////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	////////////////////////////////////////////////////////////////////////////////////////////
	static void Initialise( );

	////////////////////////////////////////////////////////////////////////////////////////////
	static void GenerateUITexturedVertices( const TEXHANDLE hTexture,
											const WinRect & rectToDraw,
											const bool bSetStream );

	////////////////////////////////////////////////////////////////////////////////////////////
	static void GenerateFillVertices(	const WinRect & rectToFill, 
										const bool bSetStream,
										Color fillColour );

	////////////////////////////////////////////////////////////////////////////////////////////
	static void GenerateFillVerticesD3DColor(	const WinRect & rectToFill,
												const bool bSetStream,
												DWORD d3dColor );

	////////////////////////////////////////////////////////////////////////////////////////////
//	static inline CVBIBManager::SVBIBHandle * GetUITexVertsVB()		{ return &sVGState.hUITexVertsVB; }
//	static inline CVBIBManager::SVBIBHandle * GetUIFillVertsVB()	{ return &sVGState.hUIFillVertsVB; }
//	static inline CVBIBManager::SVBIBHandle * GetUIFontVertsVB()	{ return &sVGState.hUIFontVertsVB; }
//	static inline CVBIBManager::SVBIBHandle * GetD3DVertexVB()		{ return &sVGState.hD3DVB; }
//	static inline CVBIBManager::SVBIBHandle * GetD3DLVertexVB()		{ return &sVGState.hD3DLitVB; }
	static inline CVBIBManager::SVBIBHandle * GetPredefinedDynamicBuffer( EPredefinedDynamicBufferType eBufferType )	
	{ 
		return &sVGState.hPredefinedDynBuffers[eBufferType]; 
	}
};

#endif // _VERTEXGENERATOR_H_