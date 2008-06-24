
#ifndef _VBIBMANAGER_H_
#define _VBIBMANAGER_H_

struct LITVERTEX
{
    float x, y, z;		// The transformed position for the vertex
	D3DCOLOR color;		// Colour value.
	float fU, fV;		// Texture coords.
};

#define D3DFVF_LITVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
class CVBIBManager
{
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	////////////////////////////////////////////////////////////////////////////////////////////
	enum EBufferType
	{
		eBT_Invalid = -1,
		eBT_Vertex = 0,
		eBT_Index,
		eBT_VertexDynamic,
		eBT_NumTypes
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SVBIBHandle
	{
		EBufferType		eType;
		DWORD			dwBufferFormat;
		DWORD			dwBufferIndex;
		DWORD			dwFirstElementOffset;	// Bytes or elements for dyn vertex buffers.
		DWORD			dwNumElements;

		SVBIBHandle() : eType(eBT_Invalid), 
						dwBufferIndex(0xFFFFFFFF),
						dwFirstElementOffset(0xFFFFFFFF),
						dwNumElements(0xFFFFFFFF)
		{
		}
	};


////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	////////////////////////////////////////////////////////////////////////////////////////////
	static const int c_iMaxNumBuffers = 256;
	static const int c_iDefaultBufferSize = 65535;
	static const D3DPOOL c_iDefaultBufferPool = D3DPOOL_MANAGED;
	static const D3DPOOL c_iDynamicBufferPool = D3DPOOL_DEFAULT;

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SD3DBuffer
	{
		EBufferType		eBufferType;			// Type of this buffer.
		DWORD			dwBufferFormat;			// Format of this buffer.
		DWORD			dwBufferSize;			// Bytes.
		DWORD			dwBufferOffset;			// Bytes.
		DWORD			dwElementSize;			// Size of vertex or index.

		struct									// Flags.
		{
			bool		bDefaultPool : 1;		// Resource creation pool type.
			bool		bLocked : 1;			// Currently locked.
			bool		bResourceEvicted : 1;	// Has this resource been evicted due to a device reset?
		};
		
		// Pointer to specific D3D resource.
		union 
		{
			LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
			LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
		};
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SVBIBManagerState
	{
		// NOTE: this MUST be the FIRST item in the structure.
		bool bInitialised;

		struct
		{
			bool		bStaticVertexBufferLocked : 1;
			bool		bStaticIndexBufferLocked : 1;
		};

		DWORD dwNumBuffersAllocated;
		SD3DBuffer * pBufferArray[ c_iMaxNumBuffers ];
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	static SVBIBManagerState sVBIB;

////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	////////////////////////////////////////////////////////////////////////////////////////////
	static void Initialise( );

	////////////////////////////////////////////////////////////////////////////////////////////
	static void Shutdown( );

	////////////////////////////////////////////////////////////////////////////////////////////
	static void EvictDefaultPoolResources( );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool AllocateVertexRegion(	SVBIBHandle * pResult,
										DWORD dwNumVertices,
										DWORD dwVertexSize,
										DWORD dwFVF,
										void ** ppData );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool AllocateIndexRegion(	SVBIBHandle * pResult,
										DWORD dwNumIndices,
										D3DFORMAT d3dIndexFormat,
										void ** ppData );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool ReleaseRegion( SVBIBHandle * pHandle );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool AddVerticesToBuffer(	SVBIBHandle * pResult,
										BYTE * pVertices,
										DWORD dwNumVertices,
										DWORD dwVertexSize,
										DWORD dwFVF );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool AddIndicesToBuffer(		SVBIBHandle * pResult,
										BYTE * pIndices,
										DWORD dwNumIndices,
										D3DFORMAT d3dIndexFormat );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool AddLegacyVerticesToBuffer(	SVBIBHandle * pResult,
											BYTE * pVertices,
											DWORD dwNumVertices,
											DWORD dwVertexSize,
											DWORD dwFVF );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool AddLegacyLitVerticesToBuffer(	SVBIBHandle * pResult,
												BYTE * pVertices,
												DWORD dwNumVertices,
												DWORD dwVertexSize,
												DWORD dwFVF );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool AllocateDynamicVertexBuffer(	SVBIBHandle * pResult,
												DWORD dwMaxVertices,
												DWORD dwVertexSize,
												DWORD dwFVF );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool LockDynamicVertexBuffer(	SVBIBHandle * pHandle,
											DWORD dwNumVertsRequired,
											void ** ppbData );

	////////////////////////////////////////////////////////////////////////////////////////////
	static bool UnlockDynamicVertexBuffer(	SVBIBHandle * pHandle );

	////////////////////////////////////////////////////////////////////////////////////////////
	static HRESULT SetVertexAndIndexStreams( const SVBIBHandle * pVB, const SVBIBHandle * pIB );

	////////////////////////////////////////////////////////////////////////////////////////////
	static HRESULT SetVertexStream( const SVBIBHandle * pVB );

	////////////////////////////////////////////////////////////////////////////////////////////
	static HRESULT SetIndexStream( const SVBIBHandle * pIB );

////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	static DWORD FindBuffer(	EBufferType eBufferType, 
								DWORD dwFormat, 
								DWORD dwElementSize,
								DWORD dwNumElements );
};

#endif // _VBIBMANAGER_H_