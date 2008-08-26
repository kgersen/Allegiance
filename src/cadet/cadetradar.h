#ifndef _CadetRadar_h_
#define _CadetRadar_h_

//////////////////////////////////////////////////////////////////////////////
//
// RadarImage header
//
//////////////////////////////////////////////////////////////////////////////

class RadarImage : public Image {
protected:
    RadarImage(Viewport* pviewport) :
        Image(pviewport)
    {
    }

    void SetViewport(Viewport* pvalue) { SetChild(0, pvalue); }

    Viewport*  GetViewport() { return Viewport::Cast(GetChild(0));  }
    Camera*    GetCamera()   { return GetViewport()->GetCamera();   }
    RectValue* GetViewRect() { return GetViewport()->GetViewRect(); }

public:
    static TRef<RadarImage> Create(Modeler* pmodeler, Viewport* pviewport, CadetGameContext* pcontext);

    enum RadarLOD {
        radarLODNone = -1,
        radarLOD1,
        radarLOD2,
        radarLOD3
        };
        
    virtual void SetRadarLOD(RadarLOD radarLOD) = 0;

    virtual void SetRadarLabels(bool rl) = 0;

    virtual bool GetRadarLabels(void) const = 0;

    //
    // Image methods
    //

    virtual void Render(Context* pcontext) = 0;

    //
    // Value members
    //

    ZString GetFunctionName() { return "RadarImage"; }
};


#endif

