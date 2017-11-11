#include "image.h"

#include <base.h>

#include "enginewindow.h"
#include "namespace.h"

//////////////////////////////////////////////////////////////////////////////
//
// Empty Image
//
//////////////////////////////////////////////////////////////////////////////

class EmptyImage : public Image {
public:
    EmptyImage() : Image()
    {
    }

    //
    // Value members
    //

	ZString GetString(int indent)
    {
        return "emptyImage";
    }

    ZString GetFunctionName()
    {
        return "emptyImage";
    }

    void Write(IMDLBinaryFile* pfile)
    {
        pfile->WriteReference("emptyImage");
    }
};

TRef<Image> g_pemptyImage = new EmptyImage();

Image* Image::GetEmpty()
{
    return g_pemptyImage;
}

//////////////////////////////////////////////////////////////////////////////
//
// Image
//
//////////////////////////////////////////////////////////////////////////////

void Image::SetEmpty()
{
    ChangeTo(GetEmpty());
}

const Bounds2& Image::GetBounds()
{
    return m_bounds;
}

void Image::CalcBounds()
{
    m_bounds = Bounds2::GetEmpty();
}

void Image::Evaluate()
{
    CalcBounds();
}

void Image::Render(Context* pcontext)
{
    TRef<Surface> psurface = GetSurface();

    if (psurface) {
        pcontext->DrawImage(psurface);
    }
}

TRef<Surface> Image::GetSurface()
{
    return NULL;
}

MouseResult Image::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    if (m_bounds.GetRect().Inside(point)) {
        return MouseResultHit();
    }

    return MouseResult();
}

//////////////////////////////////////////////////////////////////////////////
//
// Constant Image
//
//////////////////////////////////////////////////////////////////////////////

ConstantImage::ConstantImage(Surface* psurface, const ZString& str) :
    m_psurface(psurface),
    m_str(str)
{
}

const Bounds2& ConstantImage::GetBounds()
{
    CalcBounds();
    return m_bounds;
}

void ConstantImage::CalcBounds()
{
    m_bounds.SetRect(
        Rect(
            Point(0, 0),
            Point::Cast(m_psurface->GetSize())
        )
    );
}

TRef<Surface> ConstantImage::GetSurface()
{
    return m_psurface;
}

void ConstantImage::Render(Context* pcontext)
{
    pcontext->DrawImage3D(m_psurface, Color(1, 1, 1));
}

ZString ConstantImage::GetString(int indent)
{
    return
          "ImportImage(\""
        + m_str + "\", "
        + GetString(m_psurface->HasColorKey()) + ")";
}

ZString ConstantImage::GetFunctionName()
{
    return "ImportImage";
}

void ConstantImage::Write(IMDLBinaryFile* pmdlFile)
{
    pmdlFile->WriteReference("ImportImage");
    TRef<ZFile> pfile = pmdlFile->WriteBinary();
    m_psurface->Write(pfile);
}

//Imago 6/24/09 - removed the color hit-check check, see modeler.cpp(2520), these problems are related.
MouseResult ConstantImage::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{

    if (m_bounds.GetRect().Inside(point)) {
        return MouseResultHit();
    }

    return MouseResult();
}

/*
MouseResult ConstantImage::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    if (m_bounds.GetRect().Inside(point)) {
        if (m_psurface->HasColorKey()) {
            bool bHit = m_psurface->GetColor(WinPoint((int)point.X(), (int)point.Y())) != m_psurface->GetColorKey();
            m_psurface->ReleasePointer();
            
            if (bHit) {
                return MouseResultHit();
            }
        } else {
            return MouseResultHit();
        }
    }

    return MouseResult();
}
*/



//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class ConstantImage3D : public Image {
private:
    TRef<Surface> m_psurface;
    ColorValue*   GetColor()  { return  ColorValue::Cast(GetChild(0)); }

public:
    ConstantImage3D(Surface* psurface, ColorValue* pcolor) :
        Image(pcolor),
        m_psurface(psurface)
    {
    }

    const Bounds2& GetBounds()
    {
        CalcBounds();
        return m_bounds;
    }

    void CalcBounds()
    {
        m_bounds.SetRect(
            Rect(
                Point(0, 0),
                Point::Cast(m_psurface->GetSize())
            )
        );
    }

    TRef<Surface> GetSurface()
    {
        return m_psurface;
    }

    void Render(Context* pcontext)
    {
        pcontext->DrawImage3D(m_psurface, GetColor()->GetValue());
    }

    ZString GetFunctionName()
    {
        return "ImportImage3D";
    }

};

TRef<Image> CreateConstantImage3D(Surface* psurface, ColorValue* pcolor)
{
    return new ConstantImage3D(psurface, pcolor);
}

//////////////////////////////////////////////////////////////////////////////
//
// String Image
//
//////////////////////////////////////////////////////////////////////////////

class StringImage : public Image {
private:
    Justification     m_justification;
    int               m_width;
    int               m_indent;
    TRef<IEngineFont> m_pfont;

    //////////////////////////////////////////////////////////////////////////////
    //
    // value accessors
    //
    //////////////////////////////////////////////////////////////////////////////

    StringValue* GetString() { return StringValue::Cast(GetChild(0)); }
    ColorValue*  GetColor()  { return  ColorValue::Cast(GetChild(1)); }

public:
    StringImage(
        Justification justification,
        IEngineFont*  pfont,
        ColorValue*   pcolor,
        int           width,
        StringValue*  pstring,
        int           indent
    ) :
        Image(pstring, pcolor),
        m_justification(justification),
        m_pfont(pfont),
        m_width(width),
        m_indent(indent)
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // StringImageImpl methods
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString BreakLine(ZString& str, int width)
    {
        ZString strSave = str;

        //
        // Search forward for any new lines
        //

        int indexNewLine = str.Find("\n");
        if (indexNewLine == -1) {
            indexNewLine = str.GetLength();
        }

        //
        // Figure out how many characters fit on the line
        //

        int nchar = m_pfont->GetMaxTextLength(str, width, true);

        //
        // Chop off at the newline
        //

        if (nchar >= indexNewLine) {
            nchar = indexNewLine;
        } else {
            //
            // Search backwards for the first space
            //

            if (nchar != str.GetLength()) {
                for (int index = nchar - 1; index >= 0; index--) {
                    if (str[index] == ' ') {
                        while (index > 0) {
                            if (str[index] != ' ') {
                                nchar = index + 1;
                                break;
                            }
                            index--;
                        }
                        break;
                    }
                }
            }
        }

        if (nchar < str.GetLength()) {
            str = strSave.RightOf(nchar + 1);
            return strSave.Left(nchar);
        } else {
            //
            // remove any leading spaces from the next line
            //

            str = strSave.RightOf(nchar);

            int length = str.GetLength();
            int index = 0;

            while (index < length && str[index] == ' ') {
                index++;
            }

            if (index != 0) {
                str = str.RightOf(index);
            }

            return strSave.Left(nchar);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Image methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void CalcBounds()
    {
        ZString str    = GetString()->GetValue();
        int     xsize  = 0;
        int     ysize  = 0;
        int     indent = 0;

        while (!str.IsEmpty()) {
            ZString strLine = BreakLine(str, m_width - indent);
            WinPoint size   = m_pfont->GetTextExtent(strLine);

            xsize  = std::max(xsize, size.X());
            ysize += size.Y();
            indent = m_indent;
        }

        m_bounds.SetRect(Rect(0, (float)(-ysize), (float)m_width, 0));
    }

    void Render(Context* pcontext)
    {
        ZString     str    = GetString()->GetValue();
        const Rect& rect   = m_bounds.GetRect();
        int         y      = 0;
        int         indent = 0;

        const Color& color = GetColor()->GetValue();

        while (!str.IsEmpty()) {
            ZString  strLine = BreakLine(str, m_width - indent);
            WinPoint size    = m_pfont->GetTextExtent(strLine);
            int      x;

            if (m_justification == JustifyLeft()) {
                x = 0;
            } else if (m_justification == JustifyRight()) {
                x = m_width - size.X();
            } else if (m_justification == JustifyCenter()) {
                x = (m_width - size.X()) / 2;
            } else {
                ZError("Invalid Justification");
            }

            y -= size.Y();

            pcontext->DrawString(
                m_pfont,
                color,
                Point(
                    (float)x + indent,
                    (float)y
                ),
                strLine
            );

            indent = m_indent;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName() { return "StringImage"; }
};

TRef<Image> CreateStringImage(
    Justification justification,
    IEngineFont*  pfont,
    ColorValue*   pcolor,
    int           width,
    StringValue*  pstring,
    int           indent
) {
    return new StringImage(justification, pfont, pcolor, width, pstring, indent);
}

//////////////////////////////////////////////////////////////////////////////
//
// GaugeImage
//
//////////////////////////////////////////////////////////////////////////////

class GaugeImage : public Image {
private:
    Justification m_justification;
    Rect          m_rect;
    bool          m_b3D;

public:
    GaugeImage(
        Justification justification,
        Image*        pimage,
        const         Rect& rect,
        bool          b3D,
        Number*       pnumber
    ) :
        Image(pimage, pnumber),
        m_rect(rect),
        m_b3D(b3D),
        m_justification(justification)
    {
    }

    GaugeImage(
        Justification justification,
        Image*        pimage,
        bool          b3D,
        Number*       pnumber
    ) :
        Image(pimage, pnumber),
        m_rect(pimage->GetBounds().GetRect()),
        m_b3D(b3D),
        m_justification(justification)
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // GaugeImage methods
    //
    //////////////////////////////////////////////////////////////////////////////

    Image*  GetImage()  { return  Image::Cast(GetChild(0)); }
    Number* GetNumber() { return Number::Cast(GetChild(1)); }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Image methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void CalcBounds()
    {
        m_bounds = GetImage()->GetBounds();
    }

    void Render(Context* pcontext)
    {
        TRef<Surface> psurface = GetImage()->GetSurface();
        float         number   = GetNumber()->GetValue();

		// kg - fix NaN & infinity to avoid odd rendering
		if (_isnan(number) || !_finite(number)) number = 0.0f;

        number = bound(number, 0.0f, 1.0f);

        Rect  rect = m_rect;
        Point offset(0, 0);

        float xoffset    = (float)(int)(m_rect.XSize() * number);
        float yoffset    = (float)(int)(m_rect.YSize() * number);
        float xnegOffset = m_rect.XSize() - xoffset;
        float ynegOffset = m_rect.YSize() - yoffset;

        if (m_b3D) {
            if (m_justification == JustifyLeft()) {
                rect.SetXMax(m_rect.XMin() + xoffset);
            } else if (m_justification == JustifyRight()) {
                rect.SetXMin(m_rect.XMin() + xnegOffset);
                offset.SetX(xnegOffset);
            } else if (m_justification == JustifyTop()) {
                rect.SetYMin(m_rect.YMin() + ynegOffset);
                offset.SetY(ynegOffset);
            } else if (m_justification == JustifyBottom()) {
                rect.SetYMax(m_rect.YMin() + yoffset);
            } else {
                ZError("Bad Justification for GaugePane");
            }

            pcontext->DrawImage3D(psurface, rect, Color::White(), false, offset);
        } else {
            if (m_justification == JustifyLeft()) {
                rect.SetXMax(m_rect.XMin() + xoffset);
            } else if (m_justification == JustifyRight()) {
                rect.SetXMin(m_rect.XMin() + xnegOffset);
                offset.SetX(xnegOffset);
            } else if (m_justification == JustifyTop()) {
                rect.SetYMax(m_rect.YMin() + yoffset);
                offset.SetY(ynegOffset);
            } else if (m_justification == JustifyBottom()) {
                rect.SetYMin(m_rect.YMin() + ynegOffset);
            } else {
                ZError("Bad Justification for GaugePane");
            }

            pcontext->DrawImage(psurface, rect, false, offset);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName() { return "GaugeImage"; }
};

TRef<Image> CreateGaugeImage(Justification justification, Image* pimage, bool b3D, Number* pnumber)
{
    return new GaugeImage(justification, pimage, b3D, pnumber);
}

TRef<Image> CreateGaugeImage(
    Justification justification,
    Image* pimage,
    const Rect& rect,
    bool b3D,
    Number* pnumber
) {
    return new GaugeImage(justification, pimage, rect, b3D, pnumber);
}

//////////////////////////////////////////////////////////////////////////////
//
// Animated Image
//
//////////////////////////////////////////////////////////////////////////////

AnimatedImage::AnimatedImage(Number* ptime, AnimatedImage* pimage) :
    Image(ptime),
    m_psurfaces(pimage->m_psurfaces),
    m_index(0)
{
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// AnimatedImage()
// Create the resources for an animated image.
// The original source for the animated image in psurfaceSource, which is one large image.
// The original image is then broken up into square frames, and stored as individual textures, which
// are then added the m_psurfaces list.
////////////////////////////////////////////////////////////////////////////////////////////////////
AnimatedImage::AnimatedImage(Number* ptime, Surface* psurfaceSource, int nRows, int nCols) :
    Image(ptime),
    m_index(0)
{
    if (nRows == 0) {
        nRows = 1;
        WinPoint size = psurfaceSource->GetSize();
        nCols = size.X() / size.Y();
    }

    int h = psurfaceSource->GetSize().X() / nCols;
    int w = psurfaceSource->GetSize().Y() / nRows;

    m_psurfaces.SetCount(nRows * nCols);

    if (m_psurfaces.GetCount() == 1) {
        psurfaceSource->SetColorKey(Color(0, 0, 0));
        m_psurfaces.Set(0, psurfaceSource);
    } else {
        for (int row = 0; row < nRows; row++) {
            for (int col = 0; col < nCols; col++) {
                int y     = row * h;
                int x     = col * w;
                int size  = (w <= h) ? w : h;
                int index = row * nCols + col;

                TRef<Surface> psurfaceTextureSource =
                    psurfaceSource->CreateCompatibleSurface(WinPoint(size, size), SurfaceType2D());

				// Set the colour key, and store in the vector of surfaces.
                psurfaceTextureSource->SetColorKey(Color(0, 0, 0));

				ZAssert(size == w && size == h);

				// Fill the new texture with the colour key.
//                psurfaceTextureSource->FillSurfaceWithColorKey();

				// Copy in a portion of the original image into the new texture.
//				psurfaceTextureSource->BitBlt(	WinPoint(0, 0), psurfaceSource, WinRect(x, y, x + size, y + size) );
				psurfaceTextureSource->CopySubsetFromSrc( WinPoint(0, 0), psurfaceSource, WinRect(x, y, x + size, y + size) );

                m_psurfaces.Set(index, psurfaceTextureSource);
            }
        }
    }
}

void AnimatedImage::Evaluate()
{
    float time = GetTime()->GetValue();

    int count = m_psurfaces.GetCount();
    m_index = (int)(time * count) % count;

    Image::Evaluate();
}

TRef<Surface> AnimatedImage::GetSurface()
{
    return m_psurfaces[m_index];
}

MouseResult AnimatedImage::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    //  , AnimatedImage should be a vector of images instead of a vector of psurfaces

    Surface* psurface = GetSurface();

    bool bHit =
           (!psurface->HasColorKey())
        || psurface->GetColor(WinPoint((int)point.X(), (int)point.Y())) != psurface->GetColorKey();

    psurface->ReleasePointer();

    if (bHit) {
        return MouseResultHit();
    }

    return MouseResult();
}

//////////////////////////////////////////////////////////////////////////////
//
// Wrap Image
//
//////////////////////////////////////////////////////////////////////////////

void WrapImage::SetImage(Image* pvalue)
{
    if (pvalue != GetImage()) {
        if (m_bHasCapture) {
            RemoveCapture();
            m_bHasCapture = false;
        }

        SetChild(0, pvalue);
    }
}

void WrapImage::CalcBounds()
{
    GetImage()->CalcBounds();
    m_bounds = GetImage()->GetBounds();
}

TRef<Surface> WrapImage::GetSurface()
{
    return GetImage()->GetSurface();
}

void WrapImage::Render(Context* pcontext)
{
    ZEnter("WrapImage::Render()");
    GetImage()->Render(pcontext);
    ZExit("WrapImage::Render()");
}

void WrapImage::RemoveCapture()
{
    GetImage()->RemoveCapture();
    m_bHasCapture = false;
}

MouseResult WrapImage::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    if (m_bHasCapture) {
        ZAssert(bCaptured);
    } else {
        if (bCaptured) {
            return MouseResultRelease();
        }
    }

    return GetImage()->HitTest(pprovider, point, bCaptured);
}

void WrapImage::MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
{
    GetImage()->MouseMove(pprovider, point, bCaptured, bInside);
}

void WrapImage::MouseEnter(IInputProvider* pprovider, const Point& point)
{
    GetImage()->MouseEnter(pprovider, point);
}

void WrapImage::MouseLeave(IInputProvider* pprovider)
{
    GetImage()->MouseLeave(pprovider);
}

MouseResult WrapImage::Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
{
    TRef<WrapImage> pthis  = this;
    TRef<Image>     pimage = GetImage();

    MouseResult result = pimage->Button(pprovider, point, button, bCaptured, bInside, bDown);

    if (result.Test(MouseResultRelease())) {
        m_bHasCapture = false;
    } else if (result.Test(MouseResultCapture())) {
        m_bHasCapture = true;
    }

    return result;
}

//////////////////////////////////////////////////////////////////////////////
//
// UndetectableImage
//
//////////////////////////////////////////////////////////////////////////////

class UndetectableImageImpl : public WrapImage {
public:
    UndetectableImageImpl(Image* pimage) :
        WrapImage(pimage)
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IMouseInput methods
    //
    //////////////////////////////////////////////////////////////////////////////

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
    {
        return MouseResult();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName() { return "UndetectableImage"; }
};

TRef<Image> CreateUndetectableImage(Image* pimage)
{
    return new UndetectableImageImpl(pimage);
}

//////////////////////////////////////////////////////////////////////////////
//
// Trans Image
//
//////////////////////////////////////////////////////////////////////////////

TransformImage::TransformImage(Image* pimage, Transform2* ptrans) :
    WrapImage(pimage, ptrans)
{
}

void TransformImage::CalcBounds()
{
    m_bounds = GetImage()->GetBounds().Transform(GetTransform2()->GetValue());
}

void TransformImage::Render(Context* pcontext)
{
    pcontext->Multiply(GetTransform2()->GetValue());
    WrapImage::Render(pcontext);
}

Point TransformImage::TransformPoint(const Point& point)
{
    Matrix2 mat = GetTransform2()->GetValue();

    mat.Inverse();

    return mat.Transform(point);
}

MouseResult TransformImage::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    return GetImage()->HitTest(pprovider, TransformPoint(point), bCaptured);
}

void TransformImage::MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
{
    GetImage()->MouseMove(pprovider, TransformPoint(point), bCaptured, bInside);
}

void TransformImage::MouseEnter(IInputProvider* pprovider, const Point& point)
{
    GetImage()->MouseEnter(pprovider, TransformPoint(point));
}

MouseResult TransformImage::Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
{
    return GetImage()->Button(pprovider, TransformPoint(point), button, bCaptured, bInside, bDown);
}

//////////////////////////////////////////////////////////////////////////////
//
// Trans Image
//
//////////////////////////////////////////////////////////////////////////////

TranslateImage::TranslateImage(Image* pimage, const Point& point) :
    WrapImage(pimage),
    m_point(point)
{
}

void TranslateImage::CalcBounds()
{
    Rect rect = GetImage()->GetBounds().GetRect();
    rect.Offset(m_point);
    m_bounds.SetRect(rect);
}

void TranslateImage::Render(Context* pcontext)
{
    pcontext->Translate(m_point);
    WrapImage::Render(pcontext);
}

Point TranslateImage::TransformPoint(const Point& point)
{
    return point - m_point;
}

MouseResult TranslateImage::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    return GetImage()->HitTest(pprovider, TransformPoint(point), bCaptured);
}

void TranslateImage::MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
{
    GetImage()->MouseMove(pprovider, TransformPoint(point), bCaptured, bInside);
}

void TranslateImage::MouseEnter(IInputProvider* pprovider, const Point& point)
{
    GetImage()->MouseEnter(pprovider, TransformPoint(point));
}

MouseResult TranslateImage::Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
{
    return GetImage()->Button(pprovider, TransformPoint(point), button, bCaptured, bInside, bDown);
}

//////////////////////////////////////////////////////////////////////////////
//
// Group Image
//
//////////////////////////////////////////////////////////////////////////////

GroupImage::GroupImage() :
    Image(new ValueList(this))
{
}

GroupImage::GroupImage(Image* pimage1) :
    Image(new ValueList(this))
{
    AddImage(pimage1);
}

GroupImage::GroupImage(Image* pimage1, Image* pimage2) :
    Image(new ValueList(this))
{
    AddImage(pimage1);
    AddImage(pimage2);
}

GroupImage::GroupImage(Image* pimage1, Image* pimage2, Image* pimage3) :
    Image(new ValueList(this))
{
    AddImage(pimage1);
    AddImage(pimage2);
    AddImage(pimage3);
}

GroupImage::GroupImage(Image* pimage1, Image* pimage2, Image* pimage3, Image* pimage4) :
    Image(new ValueList(this))
{
    AddImage(pimage1);
    AddImage(pimage2);
    AddImage(pimage3);
    AddImage(pimage4);
}

bool GroupImage::RemoveValue(Value* pvalue)
{
    return pvalue == Image::GetEmpty();
}

void GroupImage::AddImage(Image* pimage)
{
    GetValueList()->PushFront(pimage);
}

void GroupImage::AddImageToTop(Image* pimage)
{
    GetValueList()->PushEnd(pimage);
}

void GroupImage::RemoveImage(Image* pimage)
{
    if (m_pimageCapture == pimage) {
        RemoveCapture();
        EngineWindow::DoHitTest();
    }

    if (m_pimageHit == pimage) {
        m_pimageHit = NULL;
        EngineWindow::DoHitTest();
    }

    GetValueList()->Remove(pimage);
}

void GroupImage::CalcBounds()
{
    m_bounds = Bounds2::GetEmpty();

    Image* pimage = Image::Cast(GetValueList()->GetFirst());
    while (pimage) {
        m_bounds = Union(m_bounds, pimage->GetBounds());
        pimage   = Image::Cast(GetValueList()->GetNext());
    }
}

void GroupImage::Render(Context* pcontext)
{
    ZEnter("GroupImage::Render()");

    Image* pimage = Image::Cast(GetValueList()->GetFirst());

    while (pimage) {
        if (GetValueList()->IsLast()) {
            pimage->Render(pcontext);
        } else {
            pcontext->PushState();
            pimage->Render(pcontext);
            pcontext->PopState();
        }

        pimage = Image::Cast(GetValueList()->GetNext());
    }

    ZExit("GroupImage::Render()");
}

void GroupImage::RemoveCapture()
{
    if (m_pimageCapture) {
        m_pimageCapture->RemoveCapture();
        m_pimageCapture = NULL;
    }
}

MouseResult GroupImage::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    //
    // Make sure we aren't deallocated in the middle of this function
    //

    TRef<GroupImage> pthis = this;

    //
    // figure out which image was hit
    //

    Image* pimageHit = NULL;

    if (m_pimageCapture) {
        ZAssert(bCaptured);

        //
        // Is the mouse over the captured image?
        //

        MouseResult mouseResult = m_pimageCapture->HitTest(pprovider, point, true);

        if (mouseResult.Test(MouseResultHit())) {
            pimageHit = m_pimageCapture;
        }

        //
        // Release Capture?
        //

        if (mouseResult.Test(MouseResultRelease())) {
            RemoveCapture();
            return mouseResult;
        }

        //
        // Mouse move
        //

        m_pimageCapture->MouseMove(pprovider, point, true, pimageHit != NULL);
    } else {
        //
        // Our parent thinks we are captured.  Tell them otherwise.
        //

        if (bCaptured) {
            return MouseResultRelease();
        }

        //
        // Which image are we over?
        //

        pimageHit = Image::Cast(GetValueList()->GetLast());

        while (pimageHit) {
            MouseResult mouseResult = pimageHit->HitTest(pprovider, point, false);
            if (mouseResult.Test(MouseResultHit())) {
                break;
            }
            pimageHit = Image::Cast(GetValueList()->GetPrevious());
        }

        //
        // Call MouseMove, MouseEnter, or MouseLeave
        //

        if (m_pimageHit != pimageHit) {
            if (m_pimageHit) {
                m_pimageHit->MouseLeave(pprovider);
            }
            m_pimageHit = pimageHit;
            if (m_pimageHit) {
                m_pimageHit->MouseEnter(pprovider, point);
            }
        } else if (m_pimageHit) {
            m_pimageHit->MouseMove(pprovider, point, false, true);
        }
    }

    if (pimageHit == NULL) {
        return MouseResult();
    } else {
        return MouseResultHit();
    }
}

void GroupImage::MouseLeave(IInputProvider* pprovider)
{
    if (m_pimageHit) {
        m_pimageHit->MouseLeave(pprovider);
        m_pimageHit = NULL;
    }
}

MouseResult GroupImage::Button(
    IInputProvider* pprovider,
    const Point& point,
    int button,
    bool bCaptured,
    bool bInside,
    bool bDown
) {
    MouseResult mouseResult;

    if (m_pimageCapture) {
        mouseResult =
            m_pimageCapture->Button(
                pprovider,
                point,
                button,
                true,
                m_pimageHit != NULL,
                bDown
            );

        if (mouseResult.Test(MouseResultRelease())) {
            m_pimageCapture = NULL;
        }
    } else if (m_pimageHit) {
        mouseResult = m_pimageHit->Button(pprovider, point, button, false, true, bDown);

        if (mouseResult.Test(MouseResultCapture())) {
            m_pimageCapture = m_pimageHit;
        }
    }

    return mouseResult;
}

//////////////////////////////////////////////////////////////////////////////
//
// Geo Image
//
//////////////////////////////////////////////////////////////////////////////

GeoImage::GeoImage(Geo* pgeo, Viewport* pviewport, bool bZBuffer) :
    Image(pgeo, pviewport),
    m_bZBuffer(bZBuffer),
    m_vecLight(0, 0, 1),
    m_bBidirectional(false),
    m_color(Color::White()),
    m_colorAlt(Color::White()),
    m_ambientLevel(0.0f)
{
}

void GeoImage::SetLightDirection(const Vector& vector)
{
    m_vecLight       = vector.Normalize();
}

void GeoImage::SetLight(const Color& color)
{
    m_bBidirectional = false;
    m_color          = color;
}

void GeoImage::SetLight(const Color& color, const Color& colorAlt)
{
    m_bBidirectional = true;
    m_color          = color;
    m_colorAlt       = colorAlt;
}

void GeoImage::SetAmbientLevel(float level)
{
    m_ambientLevel = level;
}

void GeoImage::CalcBounds()
{
    m_bounds.SetRect(Rect::Cast(GetViewRect()->GetValue()));
}

void GeoImage::Render(Context* pcontext)
{
    ZEnter("GeoImage::Render()");
    const Rect& rect = GetViewRect()->GetValue();

    pcontext->Translate(rect.Center());
    pcontext->Scale2(0.5f * rect.XSize());
    pcontext->Begin3DLayer(GetCamera(), m_bZBuffer);
    pcontext->SetAmbientLevel(m_ambientLevel);

    if (m_bBidirectional) {
        pcontext->BidirectionalLight(m_vecLight, m_color, m_colorAlt);
    } else {
        pcontext->DirectionalLight(m_vecLight, m_color);
    }

    GetGeo()->Render(pcontext);

    pcontext->End3DLayer();
    ZExit("GeoImage::Render()");
}

void GeoImage::RemoveCapture()
{
    GetGeo()->RemoveCapture();
}

MouseResult GeoImage::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    return MouseResult();
/*
    HitData3D hitData;

    hitData.m_distance = FLT_MAX;
    hitData.m_vecHit   = Vector::GetZero();
    hitData.m_pgeo     = NULL;

    // !!! what are the correct Vectors

    Geo* pgeo = GetGeo();

    pgeo->HitTest(Vector::GetZero(), Vector(0, 0, -1), hitData, bCaptured);
    Geo* pgeoHit = hitData.m_pgeo;
*/
}

MouseResult GeoImage::Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
{
    //  , implement

    return MouseResult();
}

//////////////////////////////////////////////////////////////////////////////
//
// Switch Image
//
//////////////////////////////////////////////////////////////////////////////

class SwitchImageImpl : public SwitchImage {
protected:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class Data {
    public:
        float       m_number;
        TRef<Image> m_pimage;
    };

    typedef TList<Data, DefaultNoEquals> DataList;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    DataList    m_list;
    TRef<Image> m_pimage;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    SwitchImageImpl(Image* pimage, Number* pnumber) :
        SwitchImage(pimage, pnumber),
        m_pimage(pimage)
    {
    }

    Number* GetNumber() { return Number::Cast(GetChild(1)); }

    //////////////////////////////////////////////////////////////////////////////
    //
    // SwitchImage Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void AddImage(float number, Image* pimage)
    {
        m_list.PushFront();
        m_list.GetFront().m_number = number;
        m_list.GetFront().m_pimage = pimage;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void Evaluate()
    {
        float number = GetNumber()->GetValue();

        DataList::Iterator iter(m_list);

        while (!iter.End()) {
            const Data& data = iter.Value();

            if (data.m_number == number) {
                SetImage(data.m_pimage);
                WrapImage::Evaluate();
                return;
            }

            iter.Next();
        }

        SetImage(m_pimage);
        SwitchImage::Evaluate();
    }
};

TRef<SwitchImage> CreateSwitchImage(Number* pnumber, Image* pimageDefault)
{
    return new SwitchImageImpl(pimageDefault, pnumber);
}


//////////////////////////////////////////////////////////////////////////////
//
// GeoImage2D
//
//////////////////////////////////////////////////////////////////////////////

class GeoImage2D : public Image {
public:
    GeoImage2D(Geo* pgeo) :
        Image(pgeo)
    {
    }

    Geo* GetGeo() { return Geo::Cast(GetChild(0)); }

    //
    // Image methods
    //

    void Render(Context* pcontext)
    {
        GetGeo()->Render(pcontext);
    }

    //
    // Value members
    //

    ZString GetFunctionName() { return "GeoImage2D"; }

    //
    // IMouseInput methods
    //

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
    {
        return MouseResult();
    }
};

TRef<Image> CreateGeoImage2D(Geo* pgeo)
{
    return new GeoImage2D(pgeo);
}

//////////////////////////////////////////////////////////////////////////////
//
// ClipImage
//
//////////////////////////////////////////////////////////////////////////////

class ClipImage : public WrapImage {
public:
    ClipImage(Image* pimage, RectValue* prect) :
        WrapImage(pimage, prect)
    {
    }

    RectValue* GetRect() { return RectValue::Cast(GetChild(1)); }

    //
    // Image methods
    //

    void CalcBounds()
    {
        m_bounds = Intersect(GetImage()->GetBounds(), GetRect()->GetValue());
    }

    void Render(Context* pcontext)
    {
        pcontext->Clip(GetRect()->GetValue());
        WrapImage::Render(pcontext);
    }

    //
    // Value members
    //

    ZString GetFunctionName() { return "ClipImage"; }

};

TRef<Image> CreateClipImage(Image* pimage, RectValue* prect)
{
    return new ClipImage(pimage, prect);
}

//////////////////////////////////////////////////////////////////////////////
//
// PickImage
//
//////////////////////////////////////////////////////////////////////////////

class PickImageImpl : public PickImage {
private:
    TRef<EventSourceImpl> m_peventSource;

public:
    PickImageImpl(Image* pimage) :
        PickImage(pimage),
        m_peventSource(new EventSourceImpl())
    {
    }

    //
    // PickImage
    //

    IEventSource* GetEventSource()
    {
        return m_peventSource;
    }

    //
    // IMouseInput
    //

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        if (button == 0 && bDown) {
            m_peventSource->Trigger();
        }

        return MouseResult();
    }

    //
    // Value members
    //

    ZString GetFunctionName() { return "PickImage"; }
};

TRef<PickImage> CreatePickImage(Image* pimage)
{
    return new PickImageImpl(pimage);
}

//////////////////////////////////////////////////////////////////////////////
//
// BlendImage
//
//////////////////////////////////////////////////////////////////////////////

class BlendImage : public WrapImage {
private:
    BlendMode m_blendMode;

public:
    BlendImage(Image* pimage, BlendMode blendMode) :
        WrapImage(pimage),
        m_blendMode(blendMode)
    {
    }

    //
    // Image methods
    //

    void Render(Context* pcontext)
    {
        pcontext->SetBlendMode(m_blendMode);
        WrapImage::Render(pcontext);
    }

    //
    // Value members
    //

    ZString GetFunctionName() { return "BlendImage"; }

};

TRef<Image> CreateBlendImage(Image* pimage, BlendMode blendMode)
{
    return new BlendImage(pimage, blendMode);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class ColorImage : public Image {
public:
    ColorImage(ColorValue* pcolor) :
        Image(pcolor)
    {
        m_bounds = Rect(-100000, -100000, 100000, 100000);
    }

    ColorValue* GetColor() { return ColorValue::Cast(GetChild(0)); }

    //
    // Image methods
    //

    void CalcBounds()
    {
    }

    void Render(Context* pcontext)
    {
        pcontext->FillInfinite(GetColor()->GetValue());
    }

    //
    // Value members
    //

    ZString GetFunctionName() { return "ColorImage"; }

};

TRef<Image> CreateColorImage(ColorValue* pcolor)
{
    return new ColorImage(pcolor);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class ExtentImage : public Image {
public:
    ExtentImage(RectValue* prect, ColorValue* pcolor) :
        Image(prect, pcolor)
    {
    }

    RectValue*  GetRect()  { return  RectValue::Cast(GetChild(0)); }
    ColorValue* GetColor() { return ColorValue::Cast(GetChild(1)); }

    //
    // Image methods
    //

    void CalcBounds()
    {
        m_bounds = GetRect()->GetValue();
    }

    void Render(Context* pcontext)
    {
        pcontext->FillRect(GetRect()->GetValue(), GetColor()->GetValue());
    }

    //
    // Value members
    //

    ZString GetFunctionName() { return "ExtentImage"; }

};

TRef<Image> CreateExtentImage(RectValue* prect, ColorValue* pcolor)
{
    return new ExtentImage(prect, pcolor);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class TransparentImage : public Image {
public:
    TransparentImage()
    {
        m_bounds = Rect(-100000, -100000, 100000, 100000);
    }

    //
    // Image methods
    //

    void CalcBounds()
    {
    }

    void Render(Context* pcontext)
    {
    }

    //
    // Value members
    //

    ZString GetFunctionName() { return "TransparentImage"; }
};

TRef<Image> CreateTransparentImage()
{
    return new TransparentImage();
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class VisibleImage : public WrapImage {
public:
    VisibleImage(Image* pimage, Boolean* bShow) :
        WrapImage(pimage, bShow)
    {
    }

    Boolean* GetVisible() { return Boolean::Cast(GetChild(1)); }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Image methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void Render(Context* pcontext)
    {
        if (GetVisible()->GetValue())
        {
            WrapImage::Render(pcontext);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value methods
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName() { return "VisibleImage"; }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IMouseInput methods
    //
    //////////////////////////////////////////////////////////////////////////////

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
    {
        MouseResult mouseResult = WrapImage::HitTest(pprovider, point, bCaptured);

        if (!GetVisible()->GetValue()) {
            mouseResult.Clear(MouseResultHit());
        }

        return mouseResult;
    }
};

TRef<Image> CreateVisibleImage(Image* pimage, Boolean* bShow)
{
    return new VisibleImage(pimage, bShow);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class JustifyImage : public WrapImage {
    Justification m_justification;
    Point         m_point;

public:
    JustifyImage(Image* pimage, Justification justification) :
        WrapImage(pimage),
        m_justification(justification)
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Implementation methods
    //
    //////////////////////////////////////////////////////////////////////////////

    Point TransformPoint(const Point& point)
    {
        return point - m_point;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Image methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void CalcBounds()
    {
        Rect rect = GetImage()->GetBounds().GetRect();

        if (m_justification.Test(JustifyLeft())) {
            m_point.SetX(-rect.XMin());
        } else if (m_justification.Test(JustifyRight())) {
            m_point.SetX(-rect.XMax());
        } else if (m_justification.Test(JustifyXCenter())) {
            m_point.SetX(-rect.Center().X());
        } else {
            m_point.SetX(0);
        }

        if (m_justification.Test(JustifyBottom())) {
            m_point.SetY(-rect.YMin());
        } else if (m_justification.Test(JustifyTop())) {
            m_point.SetY(-rect.YMax());
        } else if (m_justification.Test(JustifyYCenter())) {
            m_point.SetY(-rect.Center().Y());
        } else {
            m_point.SetY(0);
        }

        rect.Offset(m_point);
        m_bounds.SetRect(rect);
    }

    void Render(Context* pcontext)
    {
        pcontext->Translate(m_point);
        WrapImage::Render(pcontext);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IMouseInput methods
    //
    //////////////////////////////////////////////////////////////////////////////

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
    {
        return GetImage()->HitTest(pprovider, TransformPoint(point), bCaptured);
    }

    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
    {
        GetImage()->MouseMove(pprovider, TransformPoint(point), bCaptured, bInside);
    }

    void MouseEnter(IInputProvider* pprovider, const Point& point)
    {
        GetImage()->MouseEnter(pprovider, TransformPoint(point));
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        return GetImage()->Button(pprovider, TransformPoint(point), button, bCaptured, bInside, bDown);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value methods
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName() { return "JustifyImage"; }
};

TRef<Image> CreateJustifyImage(Image* pimage, Justification justification)
{
    return new JustifyImage(pimage, justification);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class MDLFileImageImpl : public MDLFileImage {
protected:
    TRef<Modeler> m_pmodeler;
    ZString       m_str;

public:
    MDLFileImageImpl(Modeler* pmodeler, const ZString& str) :
        m_pmodeler(pmodeler),
        m_str(str)
    {
        Load(str);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // MDLFileImage methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void Load(const ZString& str)
    {
        TRef<INameSpace> pns = m_pmodeler->GetNameSpace(str, false);

        if (pns) {
            TRef<Image> pimage = pns->FindImage("image");

            if (pimage) {
                SetImage(pimage);
            }

            m_pmodeler->UnloadNameSpace(pns);
        }
    }

    void Reload()
    {
        Load(m_str);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName() { return "MDLFileImage"; }
};

TRef<MDLFileImage> CreateMDLFileImage(Modeler* pmodeler, const ZString& str)
{
    return new MDLFileImageImpl(pmodeler, str);
}


