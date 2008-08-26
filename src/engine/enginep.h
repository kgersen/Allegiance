#ifndef _enginep_h_
#define _enginep_h_

// KGJV 32 B - FreeImage lib
//#define FREEIMAGE_LIB 1
#include "FreeImage/FreeImage.h"

//////////////////////////////////////////////////////////////////////////////
//
// Global hacks
//
//////////////////////////////////////////////////////////////////////////////

extern bool g_bMDLLog;
extern bool g_bWindowLog;

//////////////////////////////////////////////////////////////////////////////
//
// Direct Draw includes
//
//////////////////////////////////////////////////////////////////////////////

//
// DirectX Stuff
//


typedef IDirectDrawClipper           IDirectDrawClipperX;
typedef IDirectDrawPalette           IDirectDrawPaletteX;
typedef IDirectDrawGammaControl      IDirectDrawGammaControlX;

#ifdef USEDX7

typedef IDirectDraw7                 IDirectDrawX;
typedef IDirectDrawSurface7          IDirectDrawSurfaceX;

typedef IDirect3D7                   IDirect3DX;
typedef IDirect3DDevice7             IDirect3DDeviceX;

typedef IDirectDrawSurface7          IDirect3DTextureX; // with DX7+, tex are surfaces
typedef LPD3DVIEWPORT7               IDirect3DViewportX;
typedef IDirect3DMaterial3           IDirect3DMaterialX;

#define IID_IDirectDrawX             IID_IDirectDraw7
#define IID_IDirectDrawSurfaceX      IID_IDirectDrawSurface7
#define IID_IDirect3DX               IID_IDirect3D7
#define IID_IDirect3DTextureX        IID_IDirect3DTexture2
#define IID_IDirectDrawGammaControlX IID_IDirectDrawGammaControl

#else
typedef IDirectDraw4                 IDirectDrawX;
typedef IDirectDrawSurface4          IDirectDrawSurfaceX;

typedef IDirect3D3                   IDirect3DX;
typedef IDirect3DDevice3             IDirect3DDeviceX;

typedef IDirect3DTexture2            IDirect3DTextureX;
typedef IDirect3DViewport3           IDirect3DViewportX;
typedef IDirect3DMaterial3           IDirect3DMaterialX;

#define IID_IDirectDrawX             IID_IDirectDraw4
#define IID_IDirectDrawSurfaceX      IID_IDirectDrawSurface4
#define IID_IDirect3DX               IID_IDirect3D3
#define IID_IDirect3DTextureX        IID_IDirect3DTexture2
#define IID_IDirectDrawGammaControlX IID_IDirectDrawGammaControl

#endif

typedef IDirect3DLight               IDirect3DLightX;



typedef DDSURFACEDESC2               DDSURFACEDESCX;
typedef DDSCAPS2                     DDSCAPSX;

//
// DirectX Wrapper Classes
//

#include "ddstruct.h"

//////////////////////////////////////////////////////////////////////////////
//
// Direct Draw includes
//
//////////////////////////////////////////////////////////////////////////////

bool FillDDPF(
    DDPixelFormat&        ppdf,
    IDirectDrawX*         pdd,
    HDC                   hdc,
    HBITMAP               hbitmap,
    IDirectDrawPaletteX** pppalette
);

//////////////////////////////////////////////////////////////////////////////
//
// Direct Draw Asserts
//
//////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
    bool DDError(HRESULT hr, const char* pszCall, const char* pszFile, int line, const char* pszModule);
    bool DDSCallImpl(HRESULT hr, const char* pszCall, const char* pszFile, int line, const char* pszModule);

    #define DDCall(hr)  DDError(hr, #hr, __FILE__, __LINE__, __MODULE__)
    #define DDSCall(hr) DDSCallImpl(hr, #hr, __FILE__, __LINE__, __MODULE__)
#else
    bool DDSCallImpl(HRESULT hr);

    #define DDCall(hr) SUCCEEDED(hr)
    #define DDSCall(hr) SUCCEEDED(hr)
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Forward Declarations
//
//////////////////////////////////////////////////////////////////////////////

class PrivatePalette;
class PrivateContext;
class PrivateSurface;
class PrivateEngine;
class VideoSurface;
class DDSurface;
class Rasterizer;
class D3DRasterizer;

//////////////////////////////////////////////////////////////////////////////
//
// PrivatePalette
//
//////////////////////////////////////////////////////////////////////////////

class PrivatePalette : public Palette {
public:
    virtual IDirectDrawPaletteX* GetDDPal() = 0;
};

TRef<PrivatePalette> CreatePaletteImpl(IDirectDrawPaletteX* pddpal);

//////////////////////////////////////////////////////////////////////////////
//
// D3DDevice
//
//////////////////////////////////////////////////////////////////////////////

class D3DDevice : public IObject {
public:
    virtual void              Terminate()             = 0;
    virtual IDirect3DDeviceX* GetD3DDeviceX()         = 0;
    virtual PixelFormat*      GetTextureFormat()      = 0;
    virtual WinPoint          GetMinTextureSize()     = 0;
    virtual WinPoint          GetMaxTextureSize(DWORD dwMaxTextureSize) = 0;// yp Your_Persona August 2 2006 : MaxTextureSize Patch
    virtual bool              IsHardwareAccelerated() = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// Direct Draw Device
//
//////////////////////////////////////////////////////////////////////////////

class DDDevice : public IObject {
public:
    virtual void               Terminate()                                          = 0;
    virtual void               Reset(IDirectDrawX* pdd)                             = 0;
    virtual void               FreeEverything()                                     = 0;

    virtual HRESULT            TestCooperativeLevel()                               = 0;
    virtual void               AddRasterizer(Rasterizer* praster)                   = 0;
    virtual void               RemoveD3DDevice(D3DDevice* pd3ddevice)               = 0;
    virtual void               RemoveRasterizer(Rasterizer* praster)                = 0;
    virtual void               RemoveSurface(DDSurface* pddsurface)                 = 0;

    virtual void               SetPrimaryDevice(DDDevice* pdddevice)                = 0;

    virtual PrivateEngine*     GetEngine()                                          = 0;
    virtual IDirectDrawX*      GetDD()                                              = 0;
    virtual IDirect3DX*        GetD3D()                                             = 0;
    virtual bool               Has3DAcceleration()                                  = 0;
    virtual bool               GetAllow3DAcceleration()                             = 0;
    virtual PixelFormat*       GetZBufferPixelFormat()                              = 0;
    virtual ZString            GetName()                                            = 0;
    virtual void               SetAllow3DAcceleration(bool bAllow3DAcceleration)    = 0;
	virtual void			   SetMaxTextureSize(DWORD bMaxTextureSize)		= 0;// yp Your_Persona August 2 2006 : MaxTextureSize Patch

    virtual WinPoint           NextMode(const WinPoint& size)                       = 0;
    virtual WinPoint           PreviousMode(const WinPoint& size)                   = 0;
    virtual void               EliminateModes(const WinPoint& size)                 = 0;

    virtual int                GetTotalTextureMemory()                              = 0;
    virtual int                GetAvailableTextureMemory()                          = 0;
    virtual int                GetTotalVideoMemory()                                = 0;
    virtual int                GetAvailableVideoMemory()                            = 0;
    virtual IDirect3DTextureX* GetTextureX(D3DDevice* pd3dd, DDSurface* pddsurface) = 0;

    virtual void               BeginScene()                           = 0;
    virtual void               EndScene()                             = 0;
    virtual TRef<D3DDevice>    CreateD3DDevice(DDSurface* pddsurface) = 0;

    virtual TRef<IDirectDrawSurfaceX> CreateSurface(
        const WinPoint& size,
        DWORD caps,
        PixelFormat* ppf,
        bool  bAllocationCanFail
    ) = 0;

    virtual TRef<IDirectDrawSurfaceX> CreateMipMapTexture(
        const WinPoint& size,
        PixelFormat* ppf
    ) = 0;
};

TRef<D3DDevice> CreateD3DDevice(
    DDDevice*         pdddevice,
    IDirect3DDeviceX* pd3dd,
    bool              bHardwareAccelerated,
    PixelFormat*      ppfPrefered
);

TRef<DDDevice> CreateDDDevice(PrivateEngine* pengine, bool bAllow3DAcceleration, IDirectDrawX* pdd);

//////////////////////////////////////////////////////////////////////////////
//
// Rasterizer
//
//////////////////////////////////////////////////////////////////////////////

class Rasterizer : public IObject {
public:
    virtual void Terminate()                                            = 0;

    //
    // State
    //

    virtual void  SetShadeMode(ShadeMode shadeMode)                     = 0;
    virtual void  SetBlendMode(BlendMode blendMode)                     = 0;
    virtual void  SetTexture(Surface* psurfaceTexture)                  = 0;
    virtual void  SetWrapMode(WrapMode wrapMode)                        = 0;
    virtual void  SetCullMode(CullMode cullMode)                        = 0;
    virtual void  SetZTest(bool bZTest)                                 = 0;
    virtual void  SetZWrite(bool bZWrite)                               = 0;
    virtual void  SetLinearFilter(bool bLinearFilter)                   = 0;
    virtual void  SetPerspectiveCorrection(bool bPerspectiveCorrection) = 0;
    virtual void  SetDither(bool bDither)                               = 0;
    virtual void  SetColorKey(bool bColorKey)                           = 0;

    virtual void  BeginScene()                                          = 0;
    virtual void  EndScene()                                            = 0;
    virtual void  ClearZBuffer()                                        = 0;
    virtual void  SetClipRect(const Rect& rectClip)                     = 0;

    //
    // Attributes
    //

    virtual Point GetSurfaceSize()                                      = 0;
    virtual bool  Has3DAcceleration()                                   = 0;

    //
    // Rendering
    //

    virtual void DrawTriangles(const VertexScreen* pvertex, int vcount, const MeshIndex* pindex, int icount) = 0;
    virtual void     DrawLines(const VertexScreen* pvertex, int vcount, const MeshIndex* pindex, int icount) = 0;
    virtual void    DrawPoints(const VertexScreen* pvertex, int vcount)                                 = 0;
};

class D3DRasterizer : public Rasterizer {
public:
    
    virtual TRef<IDirect3DDeviceX>   GetD3DDeviceX()                    = 0;
    virtual TRef<IDirect3DX>         GetD3D()                           = 0;
#ifdef USEDX7
    virtual IDirect3DViewportX       GetViewport()                      = 0;
#else
    virtual TRef<IDirect3DViewportX> GetViewport()                      = 0;
#endif
    
    virtual void DrawTrianglesD3D(const D3DLVertex* psource, int count, const MeshIndex* pindex, int icount) = 0;
    virtual void     DrawLinesD3D(const D3DLVertex* psource, int count, const MeshIndex* pindex, int icount) = 0;
    virtual void    DrawPointsD3D(const D3DLVertex* psource, int count                                ) = 0;

    virtual void DrawTrianglesD3D(const D3DVertex* psource, int count, const MeshIndex* pindex, int icount) = 0;
    virtual void     DrawLinesD3D(const D3DVertex* psource, int count, const MeshIndex* pindex, int icount) = 0;
    virtual void    DrawPointsD3D(const D3DVertex* psource, int count                                ) = 0;
};

TRef<D3DRasterizer> CreateD3DRasterizer(PrivateSurface* psurface);
TRef<Rasterizer>    CreateSoftwareRasterizer(PrivateSurface* psurface);

//////////////////////////////////////////////////////////////////////////////
//
// 3D Device
//
//////////////////////////////////////////////////////////////////////////////

class IDevice3D : public IObject {
public:
    //
    // Capabilities
    //

    virtual bool Has3DAcceleration() = 0;

    //
    // State
    //

    virtual bool GetClipping()                                         = 0;
    virtual const Matrix& GetMatrix()                                  = 0;
    virtual const Matrix& GetInverseModelMatrix()                      = 0;

    virtual void AddClipPlane(const Plane& plane)                      = 0;
    virtual void RemoveClipPlane(int indexRemove)                      = 0;

    virtual void SetMatrix(const Matrix& mat)                          = 0;
    virtual void SetPerspectiveMatrix(const Matrix& mat)               = 0;
    virtual void SetClipping(bool bClip)                               = 0;
    virtual void SetShadeMode(ShadeMode shadeMode)                     = 0;
    virtual void SetBlendMode(BlendMode blendMode)                     = 0;
    virtual void SetTexture(Surface* psurfaceTexture)                  = 0;
    virtual void SetMaterial(Material* pmaterial)                      = 0;
    virtual void SetWrapMode(WrapMode wrapMode)                        = 0;
    virtual void SetCullMode(CullMode cullMode)                        = 0;
    virtual void SetZTest(bool bZTest)                                 = 0;
    virtual void SetZWrite(bool bZWrite)                               = 0;
    virtual void SetLinearFilter(bool bLinearFilter)                   = 0;
    virtual void SetPerspectiveCorrection(bool bPerspectiveCorrection) = 0;
    virtual void SetDither(bool bDither)                               = 0;
    virtual void SetColorKey(bool bColorKey)                           = 0;
    virtual void SetLineWidth(float width)                             = 0;
    virtual void SetDeformation(Deformation* pdeform)                  = 0;

    virtual void SetGlobalColor(const Color& color)                                               = 0;
    virtual void DirectionalLight(const Vector& vec, const Color& color)                          = 0;
    virtual void BidirectionalLight(const Vector& vec, const Color& color, const Color& colorAlt) = 0;
    virtual void SetAmbientLevel(float level)                                                     = 0;

    //
    //
    //

    virtual void SetClipRect(const Rect& rectClip)                     = 0;
    virtual void BeginScene()                                          = 0;
    virtual void EndScene()                                            = 0;
    virtual void ClearZBuffer()                                        = 0;

    //
    // VertexBuffer
    //

    virtual VertexL*      GetVertexLBuffer(int count)      = 0;
    virtual VertexScreen* GetVertexScreenBuffer(int count) = 0;

    //
    // Rendering
    //

    virtual void DrawTriangles(const D3DVertex* pvertex, int vcount, const MeshIndex* pindex, int icount) = 0;
    virtual void     DrawLines(const D3DVertex* pvertex, int vcount, const MeshIndex* pindex, int icount) = 0;
    virtual void    DrawPoints(const D3DVertex* pvertex, int vcount)                                 = 0;

    virtual void DrawTriangles(const Vertex* psource, int count, const MeshIndex* pindex, int icount) = 0;
    virtual void     DrawLines(const Vertex* psource, int count, const MeshIndex* pindex, int icount) = 0;
    virtual void    DrawPoints(const Vertex* psource, int count)                                 = 0;

    virtual void DrawTriangles(const VertexL* psource, int count, const MeshIndex* pindex, int icount) = 0;
    virtual void     DrawLines(const VertexL* psource, int count, const MeshIndex* pindex, int icount) = 0;
    virtual void    DrawPoints(const VertexL* psource, int count                                ) = 0;

    virtual void DrawTriangles(const VertexScreen* pvertex, int vcount, const MeshIndex* pindex, int icount) = 0;
    virtual void     DrawLines(const VertexScreen* pvertex, int vcount, const MeshIndex* pindex, int icount) = 0;
    virtual void    DrawPoints(const VertexScreen* pvertex, int vcount)                                 = 0;

    //
    // Performance Counters
    //

    virtual int GetPerformanceCounter(Counter counter) = 0;
    virtual void ResetPerformanceCounters() = 0;
};

TRef<IDevice3D> CreateDevice3D(Rasterizer* prasterizer);

//////////////////////////////////////////////////////////////////////////////
//
// PrivateContext
//
//////////////////////////////////////////////////////////////////////////////

class PrivateContext : public Context {
public:
	//
	// Called from Surface
	//

    virtual void            BeginRendering() = 0;
    virtual void            EndRendering()   = 0;
    virtual PrivateSurface* GetSurface()     = 0;
};

TRef<PrivateContext> CreateContextImpl(PrivateSurface* psurface);

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class BinarySurfaceInfo {
public:
    WinPoint m_size;
    int      m_pitch;
    DWORD    m_bitCount;
    DWORD    m_redMask;
    DWORD    m_greenMask;
    DWORD    m_blueMask;
    DWORD    m_alphaMask;
    bool     m_bColorKey;
};

//////////////////////////////////////////////////////////////////////////////
//
// VideoSurface
//
//////////////////////////////////////////////////////////////////////////////

class VideoSurface : public IObject {
public:
    //
    // Attributes
    //

    virtual SurfaceType     GetSurfaceType() = 0;
    virtual const WinPoint& GetSize()        = 0;
    virtual int             GetPitch()       = 0;
    virtual PixelFormat*    GetPixelFormat() = 0;
    virtual BYTE*           GetPointer()     = 0;
    virtual void            ReleasePointer() = 0;
    virtual bool            IsMemoryShared() = 0;

    virtual void            SetColorKey(const Color& color) = 0;

    //
    // Stretch Blt
    //

    virtual void UnclippedBlt(const WinRect& rectTarget, VideoSurface* pvideoSurfaceSource, const WinRect& rectSource, bool bHasColorKey) = 0;

    //
    // Regular Blts
    //

    virtual void UnclippedBlt(const WinRect& rectTarget, IDirectDrawSurfaceX* pddsSource, const WinPoint& pointSource, bool bHasColorKey) = 0;
    virtual void UnclippedBlt(const WinRect& rectTarget, VideoSurface* pvideoSurfaceSource, const WinPoint& pointSource) = 0;
    virtual void UnclippedFill(const WinRect& rectTarget, Pixel pixel)                                                   = 0;

    //
    // GDI blts
    //

    virtual void BitBltFromDC(HDC hdc) = 0;

    //
    // Called from Context
    //

    virtual void BeginScene() = 0;
    virtual void EndScene()   = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// DDSurface
//
//////////////////////////////////////////////////////////////////////////////

class DDSurface : public VideoSurface {
public:
    virtual bool                     HasZBuffer()                                                 = 0;
    virtual bool                     HasColorKey()                                                = 0;
    virtual const Color&             GetColorKey()                                                = 0;
    virtual bool                     InVideoMemory()                                              = 0;
    virtual DDDevice*                GetDDDevice()                                                = 0;
    virtual TRef<IDirectDrawSurface> GetDDS()                                                     = 0;
    virtual IDirectDrawSurfaceX*     GetDDSX()                                                    = 0;
    virtual IDirectDrawSurfaceX*     GetDDSX(PixelFormat* ppf)                                    = 0;
    virtual IDirectDrawSurfaceX*     GetDDSXZBuffer()                                             = 0;
    virtual IDirect3DTextureX*       GetTextureX(PixelFormat* ppf, const WinPoint& size, int& id) = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// DDSurface
//
//////////////////////////////////////////////////////////////////////////////

TRef<DDSurface> CreateDDSurface(
          DDDevice*       pdddevice,
          SurfaceType     stype,
          PixelFormat*    ppf,
          PrivatePalette* ppalette,
    const WinPoint&       size
);

TRef<DDSurface> CreateDDSurface(
          DDDevice*       pdddevice,
          SurfaceType     stype,
          PixelFormat*    ppf,
          PrivatePalette* ppalette,
    const WinPoint&       size,
          int             pitch,
          BYTE*           pdata
);

TRef<DDSurface> CreateDDSurface(
    DDDevice*            pdddevice,
    IDirectDrawSurfaceX* pdds,
    IDirectDrawSurfaceX* pddsZBuffer,
    PixelFormat*         ppf,
    PrivatePalette*      ppalette,
    SurfaceType          stype
);

//////////////////////////////////////////////////////////////////////////////
//
// DeviceDependant
//
//////////////////////////////////////////////////////////////////////////////

class DeviceDependant : public IObject {
public:
    virtual void ClearDevice() = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// PrivateSurface
//
//////////////////////////////////////////////////////////////////////////////

class PrivateSurface : 
    public Surface,
    public DeviceDependant
{
public:
    virtual VideoSurface* GetVideoSurface()                            = 0;
    virtual VideoSurface* GetVideoSurfaceNoAlloc()                     = 0;
    virtual void          SetPixelFormat(PixelFormat* ppf)             = 0;
    virtual void          BitBltFromDC(HDC hdc)                        = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// PrivateSurface
//
//////////////////////////////////////////////////////////////////////////////

TRef<PrivateSurface> CreatePrivateSurface(
    PrivateEngine*  pengine,
    PixelFormat*    ppf,
    PrivatePalette* ppalette,
    const WinPoint& size,
    SurfaceType     stype,
    SurfaceSite*    psite
);

TRef<PrivateSurface> CreatePrivateSurface(
          PrivateEngine*  pengine,
          PixelFormat*    ppf,
          PrivatePalette* ppalette,
    const WinPoint&       size,
          int             pitch,
          BYTE*           pdata,
          IObject*        pobjectMemory
);

TRef<PrivateSurface> CreatePrivateSurface(
    PrivateEngine* pengine,
    VideoSurface*  pvideoSurface,
    SurfaceSite*   psite
);

//////////////////////////////////////////////////////////////////////////////
//
// PrivateEngine
//
//////////////////////////////////////////////////////////////////////////////

class PrivateEngine : public Engine {
public:
    virtual void AddDeviceDependant(DeviceDependant* pdeviceDependant)    = 0;
    virtual void RemoveDeviceDependant(DeviceDependant* pdeviceDependant) = 0;
    virtual void RemovePrivateSurface(PrivateSurface* psurface)           = 0;

    virtual DDDevice*       GetCurrentDevice() = 0;
    virtual DDDevice*       GetPrimaryDevice() = 0;
    virtual DDSDescription  GetPrimaryDDSD()   = 0;
    virtual PrivateSurface* GetBackBuffer()    = 0;

    virtual TRef<PrivateSurface> CreateCompatibleSurface(
        PrivateSurface* psurface, 
        const WinPoint& size, 
        SurfaceType     stype, 
        SurfaceSite*    psite
    ) = 0;

    virtual TRef<VideoSurface> CreateVideoSurface(
        SurfaceType     stype,
        PixelFormat*    ppf,
        PrivatePalette* ppalette,
        const WinPoint& size,
        int             pitch,
        BYTE*           pbits
    ) = 0;

    //
    // Pixel Formats
    //

    virtual PixelFormat*      GetPrimaryPixelFormat() = 0;
    virtual TRef<PixelFormat> GetPixelFormat(const DDPixelFormat& ddpf) = 0;
    virtual TRef<PixelFormat> GetPixelFormat(
        int   bits,
        DWORD redMask,
        DWORD greenMask,
        DWORD blueMask,
        DWORD alphaMask
    ) = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// IEngineFont
//
//////////////////////////////////////////////////////////////////////////////

TRef<IEngineFont> CreateEngineFont(HFONT hfont);
TRef<IEngineFont> CreateEngineFont(IBinaryReaderSite* psite);

//////////////////////////////////////////////////////////////////////////////
//
// NameSpace stuff
//
//////////////////////////////////////////////////////////////////////////////

TRef<INameSpaceInfo> CreateNameSpaceInfo(INameSpace* pns, const ZString& str);

TRef<INameSpace> CreateNameSpace(const ZString& strName);
TRef<INameSpace> CreateNameSpace(const ZString& strName, INameSpace* pnsParent);
TRef<INameSpace> CreateNameSpace(const ZString& strName, INameSpaceList& parents);
TRef<INameSpace> CreateNameSpace(const ZString& strName, Modeler* pmodeler, ZFile* pfile);

TRef<INameSpace> CreateBinaryNameSpace(const ZString& strName, Modeler* pmodeler, ZFile* pfile);

//////////////////////////////////////////////////////////////////////////////
//
// Popup stuff
//
//////////////////////////////////////////////////////////////////////////////

class IPopupContainerPrivate : public IPopupContainer {
public:
    virtual void Initialize(Engine* pengine, RectValue* prectValue) = 0;
};

TRef<IPopupContainerPrivate> CreatePopupContainer();

#endif
