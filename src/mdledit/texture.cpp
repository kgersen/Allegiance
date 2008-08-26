    #define countTextures 64

    class TextureTestImage : public Image {
    private:
        TVector<TRef<Surface> > m_vecSurfaces;
    public:
        TextureTestImage(Engine* pengine) :
            m_vecSurfaces(countTextures)
        {
            TRef<IEngineFont> pfont =
                CreateEngineFont(
                    CreateFont(
                        128,
                        0, 0, 0,
                        FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN,
                        "tahoma"
                    )
                );

            char pch[2];
            pch[1] = 0;

            for (int index = 0; index < countTextures; index++) {
                int size = PowerOf2(randomInt(1, 256));

                TRef<Surface> psurface =
                    pengine->CreateSurface(
                        WinRect(0, 0, size, size),
                        new PixelFormat(16, 0xf800, 0x07e0, 0x001f, 0x0000)
                    );

                pch[0] = '0' + index;
                Color color;
                color.SetHSBA(random(0, 1), 1, 0.5);
                psurface->FillSurface(color);
                psurface->SetFont(pfont);
                psurface->DrawString(WinPoint(0, 0), pch);

                m_vecSurfaces.Set(index, psurface);
            }
        }

        void DrawTextures(Context* pcontext)
        {
            float scale = 0.125f;
            pcontext->PushState();
            pcontext->Scale(scale);
            int indexTexture = randomInt(0, countTextures - 1);
            ZAssert(indexTexture < countTextures);

            int count = randomInt(1, 16);
            for (int numtri = 0; numtri < count; numtri++) {
                pcontext->DrawImage3D(
                    m_vecSurfaces[indexTexture],
                    true,
                    Point(
                        640 * random(-0.25, 0.25) / scale,
                        480 * random(-0.25, 0.25) / scale
                    )
                );
            }

            pcontext->PopState();
        }

        void BltTextures(Context* pcontext)
        {
            int indexTexture = randomInt(0, countTextures - 1);
            pcontext->DrawImage(
                m_vecSurfaces[indexTexture],
                true,
                Point(
                    random(0, 640),
                    random(0, 480)
                )
            );
        }

        void DrawText(Context* pcontext)
        {
            if (randomInt(0, 10) == 0) {
                pcontext->DrawString("Hello world");
            }
        }

        void Render(Context* pcontext)
        {
            int count = randomInt(1, 32);
            for (int index = 0; index < count; index++) {
                switch(randomInt(0, 2)) {
                    case 0: DrawTextures(pcontext); break;
                    case 1: BltTextures(pcontext);  break;
                    case 2: DrawText(pcontext);     break;
                }
            }
        }
    };

    void StartTextureTest()
    {
        m_pgroupImage->AddImage(new TextureTestImage(GetEngine()));
    }
