#include "pch.h"
#include "slideshow.h"
#include "training.h"


//////////////////////////////////////////////////////////////////////////////
//
// Training Screen
//
//////////////////////////////////////////////////////////////////////////////

class PostTrainingSlideshow :
    public Slideshow
{
public:
    PostTrainingSlideshow (Modeler* pModeler, const ZString& strNamespace) :
    Slideshow (pModeler, strNamespace)
    {
    }

    ~PostTrainingSlideshow (void)
    {
    }

    void    Dismiss (void)
    {
        Slideshow::Dismiss ();
        GetWindow ()->screen(ScreenIDTrainScreen);
    }

};

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<Screen> CreatePostTrainingSlideshow (Modeler* pModeler, const ZString& strNamespace)
{
    return new PostTrainingSlideshow (pModeler, strNamespace);
}
