#pragma once

//class IntroScreen2Impl;

class IntroScreen2
{

public:

    static IntroScreen2* Create();

    virtual TRef<Image> CreateImage() = 0;
};

//IntroScreen2* CreateIntroScreen2();