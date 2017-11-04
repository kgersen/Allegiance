//////////////////////////////////////////////////////////////////////////////
//
// HRESULT converter
//
//////////////////////////////////////////////////////////////////////////////

#include <zassert.h>
#include <zstring.h>

#include "ddraw.h"
//#include "d3d.h"
//#include "d3drm.h"
#include "dinput.h"

#ifdef _DEBUG

class HRESULTInfo {
public:
    HRESULT hresult;
    char* pszError;
    char* pszText;
};

HRESULTInfo gpinfo[] =
{
    /*************************/
    /*** DirectDraw Errors ***/
    /*************************/

    {DDERR_ALREADYINITIALIZED, "DDERR_ALREADYINITIALIZED",
        "This object is already initialized"},

    {DDERR_CANNOTATTACHSURFACE, "DDERR_CANNOTATTACHSURFACE",
        "This surface can not be attached to the requested surface"},

    {DDERR_CANNOTDETACHSURFACE, "DDERR_CANNOTDETACHSURFACE",
        "This surface can not be detached from the requested surface"},

    {DDERR_CURRENTLYNOTAVAIL, "DDERR_CURRENTLYNOTAVAIL",
        "Support is currently not available"},

    {DDERR_EXCEPTION, "DDERR_EXCEPTION",
        "An exception was encountered while performing the requested "
        "operation"},

    {DDERR_GENERIC, "DDERR_GENERIC",
        "Generic failure"},

    {DDERR_HEIGHTALIGN, "DDERR_HEIGHTALIGN",
        "Height of rectangle provided is not a multiple of required "
        "alignment"},

    {DDERR_INCOMPATIBLEPRIMARY, "DDERR_INCOMPATIBLEPRIMARY",
        "Unable to match primary surface creation request with existing "
        "primary surface"},

    {DDERR_INVALIDCAPS, "DDERR_INVALIDCAPS",
        "One or more of the caps bits passed to the callback are incorrect"},

    {DDERR_INVALIDCLIPLIST, "DDERR_INVALIDCLIPLIST",
        "DirectDraw does not support provided Cliplist"},

    {DDERR_INVALIDMODE, "DDERR_INVALIDMODE",
        "DirectDraw does not support the requested mode"},

    {DDERR_INVALIDOBJECT, "DDERR_INVALIDOBJECT",
        "DirectDraw received a pointer that was an invalid DIRECTDRAW object"},

    {DDERR_INVALIDPARAMS, "DDERR_INVALIDPARAMS",
        "One or more of the parameters passed to the method are incorrect."},

    {DDERR_INVALIDPIXELFORMAT, "DDERR_INVALIDPIXELFORMAT",
        "Pixel format was invalid as specified"},

    {DDERR_INVALIDRECT, "DDERR_INVALIDRECT",
        "Rectangle provided was invalid"},

    {DDERR_LOCKEDSURFACES, "DDERR_LOCKEDSURFACES",
        "Operation could not be carried out because one or more surfaces "
        "are locked"},

    {DDERR_NO3D, "DDERR_NO3D",
        "There is no 3D present"},

    {DDERR_NOALPHAHW, "DDERR_NOALPHAHW",
        "Operation could not be carried out because there is no alpha "
        "accleration hardware present or available"},

    {DDERR_NOCLIPLIST, "DDERR_NOCLIPLIST",
        "No clip list available"},

    {DDERR_NOCOLORCONVHW, "DDERR_NOCOLORCONVHW",
        "Operation could not be carried out because there is no color "
        "conversion hardware present or available"},

    {DDERR_NOCOOPERATIVELEVELSET, "DDERR_NOCOOPERATIVELEVELSET",
        "Create function called without DirectDraw object method "
        "SetCooperativeLevel being called"},

    {DDERR_NOCOLORKEY, "DDERR_NOCOLORKEY",
        "Surface doesn't currently have a color key"},

    {DDERR_NOCOLORKEYHW, "DDERR_NOCOLORKEYHW",
        "Operation could not be carried out because there is no "
        "hardware support of the destination color key"},

    {DDERR_NODIRECTDRAWSUPPORT, "DDERR_NODIRECTDRAWSUPPORT",
        "No DirectDraw support possible with current display driver"},

    {DDERR_NOEXCLUSIVEMODE, "DDERR_NOEXCLUSIVEMODE",
        "Operation requires the application to have exclusive mode "
        "but the application does not have exclusive mode"},

    {DDERR_NOFLIPHW, "DDERR_NOFLIPHW",
        "Flipping visible surfaces is not supported"},

    {DDERR_NOGDI, "DDERR_NOGDI",
        "There is no GDI present"},

    {DDERR_NOMIRRORHW, "DDERR_NOMIRRORHW",
        "Operation could not be carried out because there is "
        "no hardware present or available"},

    {DDERR_NOTFOUND, "DDERR_NOTFOUND",
        "Requested item was not found"},

    {DDERR_NOOVERLAYHW, "DDERR_NOOVERLAYHW",
        "Operation could not be carried out because there is "
        "no overlay hardware present or available"},

    {DDERR_NORASTEROPHW, "DDERR_NORASTEROPHW",
        "Operation could not be carried out because there is "
        "no appropriate raster op hardware present or available"},

    {DDERR_NOROTATIONHW, "DDERR_NOROTATIONHW",
        "Operation could not be carried out because there is "
        "no rotation hardware present or available"},

    {DDERR_NOSTRETCHHW, "DDERR_NOSTRETCHHW",
        "Operation could not be carried out because there is "
        "no hardware support for stretching"},

    {DDERR_NOT4BITCOLOR, "DDERR_NOT4BITCOLOR",
        "DirectDrawSurface is not in 4 bit color palette and "
        "the requested operation requires 4 bit color palette"},

    {DDERR_NOT4BITCOLORINDEX, "DDERR_NOT4BITCOLORINDEX",
        "DirectDrawSurface is not in 4 bit color index palette "
        "and the requested operation requires 4 bit color index palette"},

    {DDERR_NOT8BITCOLOR, "DDERR_NOT8BITCOLOR",
        "DirectDraw Surface is not in 8 bit color mode "
        "and the requested operation requires 8 bit color"},

    {DDERR_NOTEXTUREHW, "DDERR_NOTEXTUREHW",
        "Operation could not be carried out because there is "
        "no texture mapping hardware present or available"},

    {DDERR_NOVSYNCHW, "DDERR_NOVSYNCHW",
        "Operation could not be carried out because there is "
        "no hardware support for vertical blank synchronized operations"},

    {DDERR_NOZBUFFERHW, "DDERR_NOZBUFFERHW",
        "Operation could not be carried out because there is "
        "no hardware support for zbuffer blting"},

    {DDERR_NOZOVERLAYHW, "DDERR_NOZOVERLAYHW",
        "Overlay surfaces could not be z layered based on their "
        "BltOrder because the hardware does not support z layering of overlays"},

    {DDERR_OUTOFCAPS, "DDERR_OUTOFCAPS",
        "The hardware needed for the requested operation has "
        "already been allocated"},

    {DDERR_OUTOFMEMORY, "DDERR_OUTOFMEMORY",
        "DirectDraw does not have enough memory to perform the operation"},

    {DDERR_OUTOFVIDEOMEMORY, "DDERR_OUTOFVIDEOMEMORY",
        "DirectDraw does not have enough memory to perform the operation"},

    {DDERR_OVERLAYCANTCLIP, "DDERR_OVERLAYCANTCLIP",
        "hardware does not support clipped overlays"},

    {DDERR_OVERLAYCOLORKEYONLYONEACTIVE, "DDERR_OVERLAYCOLORKEYONLYONEACTIVE",
        "Can only have ony color key active at one time for overlays"},

    {DDERR_PALETTEBUSY, "DDERR_PALETTEBUSY",
        "Access to this palette is being refused because the palette "
        "is already locked by another thread"},

    {DDERR_COLORKEYNOTSET, "DDERR_COLORKEYNOTSET",
        "No src color key specified for this operation"},

    {DDERR_SURFACEALREADYATTACHED, "DDERR_SURFACEALREADYATTACHED",
        "This surface is already attached to the surface it is "
        "being attached to"},

    {DDERR_SURFACEALREADYDEPENDENT, "DDERR_SURFACEALREADYDEPENDENT",
        "This surface is already a dependency of the surface it "
        "is being made a dependency of"},

    {DDERR_SURFACEBUSY, "DDERR_SURFACEBUSY",
        "Access to this surface is being refused because the surface "
        "is already locked by another thread"},

    {DDERR_SURFACEISOBSCURED, "DDERR_SURFACEISOBSCURED",
        "Access to Surface refused because Surface is obscured"},

    {DDERR_SURFACELOST, "DDERR_SURFACELOST",
        "The DIRECTDRAWSURFACE object representing this surface "
        "should have Restore called on it.  Access to this surface is "
        "being refused because the surface is gone"},

    {DDERR_SURFACENOTATTACHED, "DDERR_SURFACENOTATTACHED",
        "The requested surface is not attached"},

    {DDERR_TOOBIGHEIGHT, "DDERR_TOOBIGHEIGHT",
        "Height requested by DirectDraw is too large"},

    {DDERR_TOOBIGSIZE, "DDERR_TOOBIGSIZE",
        "Size requested by DirectDraw is too large.  The individual height "
        "and width are OK"},

    {DDERR_TOOBIGWIDTH, "DDERR_TOOBIGWIDTH",
        "Width requested by DirectDraw is too large"},

    {DDERR_UNSUPPORTED, "DDERR_UNSUPPORTED",
        "Action not supported"},

    {DDERR_UNSUPPORTEDFORMAT, "DDERR_UNSUPPORTEDFORMAT",
        "FOURCC format requested is unsupported by DirectDraw"},

    {DDERR_UNSUPPORTEDMASK, "DDERR_UNSUPPORTEDMASK",
        "Bitmask in the pixel format requested is unsupported by DirectDraw"},

    {DDERR_VERTICALBLANKINPROGRESS, "DDERR_VERTICALBLANKINPROGRESS",
        "vertical blank is in progress"},

    {DDERR_WASSTILLDRAWING, "DDERR_WASSTILLDRAWING",
        "Informs DirectDraw that the previous Blt which is "
        "transfering information to or from this Surface is incomplete"},

    {DDERR_XALIGN, "DDERR_XALIGN",
        "Rectangle provided was not horizontally aligned on required boundary"},

    {DDERR_INVALIDDIRECTDRAWGUID, "DDERR_INVALIDDIRECTDRAWGUID",
        "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver "
        "identifier"},

    {DDERR_DIRECTDRAWALREADYCREATED, "DDERR_DIRECTDRAWALREADYCREATED",
        "A DirectDraw object representing this driver has already been "
        "created for this process"},

    {DDERR_NODIRECTDRAWHW, "DDERR_NODIRECTDRAWHW",
        "A hardware only DirectDraw object creation was attempted "
        "but the driver did not support any hardware"},

    {DDERR_PRIMARYSURFACEALREADYEXISTS, "DDERR_PRIMARYSURFACEALREADYEXISTS",
        "This process already has created a primary surface"},

    {DDERR_NOEMULATION, "DDERR_NOEMULATION",
        "Software emulation not available"},

    {DDERR_REGIONTOOSMALL, "DDERR_REGIONTOOSMALL",
        "Region passed to Clipper::GetClipList is too small"},

    {DDERR_CLIPPERISUSINGHWND, "DDERR_CLIPPERISUSINGHWND",
        "An attempt was made to set a clip list for a clipper objec that is "
        "already monitoring an hwnd"},

    {DDERR_NOCLIPPERATTACHED, "DDERR_NOCLIPPERATTACHED",
        "No clipper object attached to surface object"},

    {DDERR_NOHWND, "DDERR_NOHWND",
        "Clipper notification requires an HWND or no HWND has previously "
        "been set as the CooperativeLevel HWND"},

    {DDERR_HWNDSUBCLASSED, "DDERR_HWNDSUBCLASSED",
        "HWND used by DirectDraw CooperativeLevel has been subclassed, "
        "this prevents DirectDraw from restoring state"},

    {DDERR_HWNDALREADYSET, "DDERR_HWNDALREADYSET",
        "The CooperativeLevel HWND has already been set.  It can not be "
        "reset while the process has surfaces or palettes created"},

    {DDERR_NOPALETTEATTACHED, "DDERR_NOPALETTEATTACHED",
        "No palette object attached to this surface"},

    {DDERR_NOPALETTEHW, "DDERR_NOPALETTEHW",
        "No hardware support for 16 or 256 color palettes"},

    {DDERR_BLTFASTCANTCLIP, "DDERR_BLTFASTCANTCLIP",
        "If a clipper object is attached to the source surface passed "
        "into a BltFast call"},

    {DDERR_NOBLTHW, "DDERR_NOBLTHW",
        "No blitter hardware"},

    {DDERR_NODDROPSHW, "DDERR_NODDROPSHW",
        "No DirectDraw ROP hardware"},

    {DDERR_OVERLAYNOTVISIBLE, "DDERR_OVERLAYNOTVISIBLE",
        "GetOverlayPosition called on a hidden overlay"},

    {DDERR_NOOVERLAYDEST, "DDERR_NOOVERLAYDEST",
        "GetOverlayPosition called on a overlay that "
        "UpdateOverlay has never been called on to establish a destination"},

    {DDERR_INVALIDPOSITION, "DDERR_INVALIDPOSITION",
        "The position of the overlay on the destination is "
        "no longer legal for that destination"},

    {DDERR_NOTAOVERLAYSURFACE, "DDERR_NOTAOVERLAYSURFACE",
        "Overlay member called for a non-overlay surface"},

    {DDERR_EXCLUSIVEMODEALREADYSET, "DDERR_EXCLUSIVEMODEALREADYSET",
        "An attempt was made to set the cooperative level when it was "
        "already set to exclusive"},

    {DDERR_NOTFLIPPABLE, "DDERR_NOTFLIPPABLE",
        "An attempt has been made to flip a surface that is not flippable"},

    {DDERR_CANTDUPLICATE, "DDERR_CANTDUPLICATE",
        "Can't duplicate primary & 3D surfaces, or surfaces that are "
        "implicitly created"},

    {DDERR_NOTLOCKED, "DDERR_NOTLOCKED",
        "Surface was not locked.  An attempt to unlock a surface that was "
        "not locked at all, or by this process, has been attempted"},

    {DDERR_CANTCREATEDC, "DDERR_CANTCREATEDC",
        "Windows can not create any more DCs"},

    {DDERR_NODC, "DDERR_NODC",
        "No DC was ever created for this surface"},

    {DDERR_WRONGMODE, "DDERR_WRONGMODE",
        "This surface can not be restored because it was created in a "
        "different mode"},

    {DDERR_IMPLICITLYCREATED, "DDERR_IMPLICITLYCREATED",
        "This surface can not be restored because it is an implicitly "
        "created surface"},

    {DDERR_NOTPALETTIZED, "DDERR_NOTPALETTIZED",
        "The surface being used is not a palette-based surface"},

    {DDERR_UNSUPPORTEDMODE, "DDERR_UNSUPPORTEDMODE",
        "The display is currently in an unsupported mode"},


    /******************/
    /*** D3D Errors ***/
    /******************/
/*

    {D3DERR_BADMAJORVERSION, "D3DERR_BADMAJORVERSION",
        "Bad major version"},

    {D3DERR_BADMINORVERSION, "D3DERR_BADMINORVERSION",
        "Bad minor version"},

    {D3DERR_EXECUTE_CREATE_FAILED, "D3DERR_EXECUTE_CREATE_FAILED",
        "Execute buffer create failed"},

    {D3DERR_EXECUTE_DESTROY_FAILED, "D3DERR_EXECUTE_DESTROY_FAILED",
        "Execute buffer destroy failed"},

    {D3DERR_EXECUTE_LOCK_FAILED, "D3DERR_EXECUTE_LOCK_FAILED",
        "Execute buffer lock failed"},

    {D3DERR_EXECUTE_UNLOCK_FAILED, "D3DERR_EXECUTE_UNLOCK_FAILED",
        "Execute buffer unlock failed"},

    {D3DERR_EXECUTE_LOCKED, "D3DERR_EXECUTE_LOCKED",
        "Execute buffer locked"},

    {D3DERR_EXECUTE_NOT_LOCKED, "D3DERR_EXECUTE_NOT_LOCKED",
        "Execute buffer not locked"},

    {D3DERR_EXECUTE_FAILED, "D3DERR_EXECUTE_FAILED",
        "Execute buffer execute failed"},

    {D3DERR_EXECUTE_CLIPPED_FAILED, "D3DERR_EXECUTE_CLIPPED_FAILED",
        "Execute buffer execute clipped failed"},

    {D3DERR_TEXTURE_NO_SUPPORT, "D3DERR_TEXTURE_NO_SUPPORT",
        "Texture not supported"},

    {D3DERR_TEXTURE_CREATE_FAILED, "D3DERR_TEXTURE_CREATE_FAILED",
        "Texture create failed"},

    {D3DERR_TEXTURE_DESTROY_FAILED, "D3DERR_TEXTURE_DESTROY_FAILED",
        "Texture destroy failed"},

    {D3DERR_TEXTURE_LOCK_FAILED, "D3DERR_TEXTURE_LOCK_FAILED",
        "Texture lock failed"},

    {D3DERR_TEXTURE_UNLOCK_FAILED, "D3DERR_TEXTURE_UNLOCK_FAILED",
        "Texture unlock failed"},

    {D3DERR_TEXTURE_LOAD_FAILED, "D3DERR_TEXTURE_LOAD_FAILED",
        "Texture load failed"},

    {D3DERR_TEXTURE_SWAP_FAILED, "D3DERR_TEXTURE_SWAP_FAILED",
        "Texture swap failed"},

    {D3DERR_TEXTURE_LOCKED, "D3DERR_TEXTURE_LOCKED",
        "Texture locked"},

    {D3DERR_TEXTURE_NOT_LOCKED, "D3DERR_TEXTURE_NOT_LOCKED",
        "Texture not locked"},

    {D3DERR_TEXTURE_GETSURF_FAILED, "D3DERR_TEXTURE_GETSURF_FAILED",
        "Texture get surface failed"},

    {D3DERR_MATRIX_CREATE_FAILED, "D3DERR_MATRIX_CREATE_FAILED",
        "Matrix create failed"},

    {D3DERR_MATRIX_DESTROY_FAILED, "D3DERR_MATRIX_DESTROY_FAILED",
        "Matrix destroy failedj"},

    {D3DERR_MATRIX_SETDATA_FAILED, "D3DERR_MATRIX_SETDATA_FAILED",
        "Matrix set data failed"},

    {D3DERR_MATRIX_GETDATA_FAILED, "D3DERR_MATRIX_GETDATA_FAILED",
        "Matrix get data failed"},

    {D3DERR_SETVIEWPORTDATA_FAILED, "D3DERR_SETVIEWPORTDATA_FAILED",
        "Set viewport data failed"},

    {D3DERR_MATERIAL_CREATE_FAILED, "D3DERR_MATERIAL_CREATE_FAILED",
        "Material create failed"},

    {D3DERR_MATERIAL_DESTROY_FAILED, "D3DERR_MATERIAL_DESTROY_FAILED",
        "Material destroy failed"},

    {D3DERR_MATERIAL_SETDATA_FAILED, "D3DERR_MATERIAL_SETDATA_FAILED",
        "Material set data failed"},

    {D3DERR_MATERIAL_GETDATA_FAILED, "D3DERR_MATERIAL_GETDATA_FAILED",
        "Material get data failed"},

    {D3DERR_LIGHT_SET_FAILED, "D3DERR_LIGHT_SET_FAILED",
        "Light set failed"},

    {D3DERR_SCENE_IN_SCENE, "D3DERR_SCENE_IN_SCENE",
        "Scene in scene"},

    {D3DERR_SCENE_NOT_IN_SCENE, "D3DERR_SCENE_NOT_IN_SCENE",
        "Scene not in scene"},

    {D3DERR_SCENE_BEGIN_FAILED, "D3DERR_SCENE_BEGIN_FAILED",
        "Scene begin failed"},

    {D3DERR_SCENE_END_FAILED, "D3DERR_SCENE_END_FAILED",
        "Scene end failed"},

*/
    /*************************************/
    /*** Direct3D Retained-Mode Errors ***/
    /*************************************/
/*
    {D3DRMERR_BADOBJECT, "D3DRMERR_BADOBJECT",
        "Object expected in argument"},

    {D3DRMERR_BADTYPE, "D3DRMERR_BADTYPE",
        "Bad argument type passed"},

    {D3DRMERR_BADALLOC, "D3DRMERR_ALLOC",
        "Out of memory"},

    {D3DRMERR_FACEUSED, "D3DRMERR_FACEUSED",
        "Face already used in a mesh"},

    {D3DRMERR_NOTFOUND, "D3DRMERR_NOTFOUND",
        "Object not found in specified place"},

    {D3DRMERR_NOTDONEYET, "D3DRMERR_NOTDONEYET",
        "Unimplemented"},

    {D3DRMERR_FILENOTFOUND, "D3DRMERR_FILENOTFOUND",
        "File cannot be opened"},

    {D3DRMERR_BADFILE, "D3DRMERR_BADFILE",
        "Data file is corrupt or has incorrect format"},

    {D3DRMERR_BADDEVICE, "D3DRMERR_BADDEVICE",
        "Device is not compatible with renderer"},

    {D3DRMERR_BADVALUE, "D3DRMERR_BADVALUE",
        "Bad argument value passed"},

    {D3DRMERR_BADMAJORVERSION, "D3DRMERR_BADMAJORVERSION",
        "Bad DLL major version"},

    {D3DRMERR_BADMINORVERSION, "D3DRMERR_BADMINORVERSION",
        "Bad DLL minor version"},

    {D3DRMERR_UNABLETOEXECUTE, "D3DRMERR_UNABLETOEXECUTE",
        "Unable to carry out procedure"},*/

    //
    // DirectInput errors
    //

    { DI_BUFFEROVERFLOW, "DI_BUFFEROVERFLOW", "The device buffer overflowed and some input was lost. This value is equal to the S_FALSE standard COM return value."},
    { DI_DOWNLOADSKIPPED , "DI_DOWNLOADSKIPPED ","The parameters of the effect were successfully updated, but the effect could not be downloaded because the associated device was not acquired in exclusive mode."},
    { DI_EFFECTRESTARTED , "DI_EFFECTRESTARTED ","The effect was stopped, the parameters were updated, and the effect was restarted."},
    { DI_NOEFFECT , "DI_NOEFFECT ","The operation had no effect. This value is equal to the S_FALSE standard COM return value."},
    { DI_NOTATTACHED , "DI_NOTATTACHED ","The device exists but is not currently attached. This value is equal to the S_FALSE standard COM return value."},
    { DI_OK , "DI_OK ","The operation completed successfully. This value is equal to the S_OK standard COM return value."},
    { DI_POLLEDDEVICE , "DI_POLLEDDEVICE ","The device is a polled device. As a result, device buffering will not collect any data and event notifications will not be signaled until the IDirectInputDevice2::Poll method is called."},
    { DI_PROPNOEFFECT , "DI_PROPNOEFFECT ","The change in device properties had no effect. This value is equal to the S_FALSE standard COM return value."},
    { DI_TRUNCATED , "DI_TRUNCATED ","The parameters of the effect were successfully updated, but some of them were beyond the capabilities of the device and were truncated to the nearest supported value."},
    { DI_TRUNCATEDANDRESTARTED , "DI_TRUNCATEDANDRESTARTED ","Equal to DI_EFFECTRESTARTED | DI_TRUNCATED."},
    { DIERR_ACQUIRED , "DIERR_ACQUIRED ","The operation cannot be performed while the device is acquired."},
    { DIERR_ALREADYINITIALIZED , "DIERR_ALREADYINITIALIZED ","This object is already initialized"},
    { DIERR_BADDRIVERVER , "DIERR_BADDRIVERVER ","The object could not be created due to an incompatible driver version or mismatched or incomplete driver components."},
    { DIERR_BETADIRECTINPUTVERSION , "DIERR_BETADIRECTINPUTVERSION ","The application was written for an unsupported prerelease version of DirectInput."},
    { DIERR_DEVICEFULL , "DIERR_DEVICEFULL ","The device is full."},
    { DIERR_DEVICENOTREG , "DIERR_DEVICENOTREG ","The device or device instance is not registered with DirectInput. This value is equal to the REGDB_E_CLASSNOTREG standard COM return value."},
    { DIERR_EFFECTPLAYING , "DIERR_EFFECTPLAYING ","The parameters were updated in memory but were not downloaded to the device because the device does not support updating an effect while it is still playing."},
    { DIERR_HASEFFECTS , "DIERR_HASEFFECTS ","The device cannot be reinitialized because there are still effects attached to it."},
    { DIERR_GENERIC , "DIERR_GENERIC ","An undetermined error occurred inside the DirectInput subsystem. This value is equal to the E_FAIL standard COM return value."},
    { DIERR_HANDLEEXISTS , "DIERR_HANDLEEXISTS ","The device already has an event notification associated with it. This value is equal to the E_ACCESSDENIED standard COM return value."},
    { DIERR_INCOMPLETEEFFECT , "DIERR_INCOMPLETEEFFECT ","The effect could not be downloaded because essential information is missing. For example, no axes have been associated with the effect, or no type-specific information has been supplied."},
    { DIERR_INPUTLOST , "DIERR_INPUTLOST ","Access to the input device has been lost. It must be reacquired."},
    { DIERR_INVALIDPARAM , "DIERR_INVALIDPARAM ","An invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called."},
    { DIERR_MOREDATA , "DIERR_MOREDATA ","Not all the requested information fitted into the buffer."},
    { DIERR_NOAGGREGATION , "DIERR_NOAGGREGATION ","This object does not support aggregation."},
    { DIERR_NOINTERFACE , "DIERR_NOINTERFACE ","The specified interface is not supported by the object. This value is equal to the E_NOINTERFACE standard COM return value."},
    { DIERR_NOTACQUIRED , "DIERR_NOTACQUIRED ","The operation cannot be performed unless the device is acquired."},
    { DIERR_NOTBUFFERED , "DIERR_NOTBUFFERED ","The device is not buffered. Set the DIPROP_BUFFERSIZE property to enable buffering."},
    { DIERR_NOTDOWNLOADED , "DIERR_NOTDOWNLOADED ","The effect is not downloaded."},
    { DIERR_NOTEXCLUSIVEACQUIRED , "DIERR_NOTEXCLUSIVEACQUIRED ","The operation cannot be performed unless the device is acquired in DISCL_EXCLUSIVE mode."},
    { DIERR_NOTFOUND , "DIERR_NOTFOUND ","The requested object does not exist."},
    { DIERR_NOTINITIALIZED , "DIERR_NOTINITIALIZED ","This object has not been initialized."},
    { DIERR_OBJECTNOTFOUND , "DIERR_OBJECTNOTFOUND ","The requested object does not exist."},
    { DIERR_OLDDIRECTINPUTVERSION , "DIERR_OLDDIRECTINPUTVERSION ", "he application requires a newer version of DirectInput."},
    { DIERR_OTHERAPPHASPRIO , "DIERR_OTHERAPPHASPRIO ","Another application has a higher priority level, preventing this call from succeeding."},
    { DIERR_OUTOFMEMORY , "DIERR_OUTOFMEMORY ","The DirectInput subsystem couldn't allocate sufficient memory to complete the call. This value is equal to the E_OUTOFMEMORY standard COM return value."},
    { DIERR_READONLY , "DIERR_READONLY ","The specified property cannot be changed. This value is equal to the E_ACCESSDENIED standard COM return value."},
    { DIERR_UNSUPPORTED , "DIERR_UNSUPPORTED ","The function called is not supported at this time. This value is equal to the E_NOTIMPL standard COM return value."},

    /************************************/
    /*** Miscellaneous Windows Errors ***/
    /************************************/

    {ERROR_INVALID_PARAMETER, "ERROR_INVALID_PARAMETER",
        "Invalid parameter"},

    {ERROR_NOT_ENOUGH_MEMORY, "ERROR_NOT_ENOUGH_MEMORY",
        "Insufficient memory available"},

    {ERROR_OUTOFMEMORY, "ERROR_OUTOFMEMORY",
        "Out of memory"},

    {0,0,0}
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

bool g_bDumpDD = false;

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

bool DDError(HRESULT hresult, const char* pszCall, const char* pszFile, int line, const char* pszModule)
{
    if (g_bDumpDD) {
        ZDebugOutput(ZString(pszCall) + "\n");
    }

    if (FAILED(hresult)) {
        HRESULTInfo* pinfo = gpinfo;

        while (pinfo->hresult != hresult && pinfo->hresult != 0) {
            pinfo++;
        }

        ZDebugOutput(ZString(pszCall) + "\n");
        if (pinfo->hresult == 0) {
            ZAssertImpl(false, "unknown error code", pszFile, line, pszModule);
        } else {
            ZAssertImpl(false, pinfo->pszText, pszFile, line, pszModule);
        }
        
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

bool DDSCallImpl(
    HRESULT hr,
    const char* pszCall,
    const char* pszFile,
    int line,
    const char* pszModule
) {
    if (
           hr == DDERR_SURFACELOST
        || hr == DDERR_SURFACEBUSY
        || hr == DDERR_UNSUPPORTED
    ) {
        return false;
    }

    return DDError(hr, pszCall, pszFile, line, pszModule);
}

#endif // _DEBUG
