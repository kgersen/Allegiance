
#include "pch.h"
#include "ui.h"


class IntroScreen2Impl : public IntroScreen2 {

public:
    IntroScreen2Impl() {}

    ~IntroScreen2Impl() {}

    TRef<Image> CreateImage()
    {
        return Image::GetEmpty();
    }
};

IntroScreen2 * IntroScreen2::Create()
{
    return new IntroScreen2Impl();
}