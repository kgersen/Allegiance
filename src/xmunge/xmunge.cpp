//////////////////////////////////////////////////////////////////////////////
//
// Engine Application
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// The main entry point
//
//////////////////////////////////////////////////////////////////////////////

#include "main.h"
#ifdef _DEBUG
    bool DDError(HRESULT hr, const char* pszCall, const char* pszFile, int line, const char* pszModule);

    #define DDCall(hr) DDError(hr, #hr, __FILE__, __LINE__, __MODULE__)
#else
    #define DDCall(hr) SUCCEEDED(hr)
#endif

bool    substr(const char*  s1, const char* s2)
{
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int i = len1 - len2;

    while ((i >= 0) && (strncmp(s1 + i, s2, len2) != 0))
    {
        i--;
    }

    return (i >= 0);
}

//////////////////////////////////////////////////////////////////////////////
//
// Engine Application
//
//////////////////////////////////////////////////////////////////////////////
static const DWORD    c_cbBfr = 512;

class XMunge : public Win32App {
protected:
    enum Mode {
        ModeNone,
        ModeReduce,
        ModeFlatten,
        ModeExtract,
        ModeBound
    };

    Mode  m_mode;
    float m_scale;
    float m_frame;

public:
    XMunge() :
        m_mode(ModeNone)
    {
    }

    void ReducePMesh(
        const PathString& strPathIn, 
        const PathString& strPathOut, 
        float scale
    ) {
        TRef<IDirect3DRM>                m_pd3drm1;
        TRef<IDirect3DRM2>               m_pd3drm;
        TRef<IDirect3DRMProgressiveMesh> m_ppmesh;
        TRef<IDirect3DRMMesh>            m_pmesh;
        TRef<IDirect3DRMMeshBuilder2>    m_pbuilder;

        DDCall(Direct3DRMCreate(&m_pd3drm1));
        DDCall(m_pd3drm1->QueryInterface(IID_IDirect3DRM2, (void**)&m_pd3drm));

        DDCall(m_pd3drm->CreateProgressiveMesh(&m_ppmesh));

        DDCall(m_ppmesh->Load((void*)(PCC)strPathIn, NULL, D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE, NULL, this));
        DDCall(m_ppmesh->SetDetail(scale));
        DDCall(m_ppmesh->CreateMesh(&m_pmesh));

        DDCall(m_pd3drm->CreateMeshBuilder(&m_pbuilder));

        DDCall(m_pbuilder->AddMesh(m_pmesh));
        DDCall(m_pbuilder->Save(strPathOut, D3DRMXOF_TEXT, D3DRMXOFSAVE_ALL));
    }

    void Flatten(
        const PathString& strPathIn, 
        const PathString& strPathOut,
        float frame
    )
    {
        HRESULT                          hr;
        TRef<IDirect3DRM>                m_pd3drm1;
        TRef<IDirect3DRM2>               m_pd3drm;
        TRef<IDirect3DRMFrame2>          m_pframe;
        TRef<IDirect3DRMAnimationSet>    m_pset;
        TRef<IDirect3DRMMeshBuilder2>    m_pbuilder;

        DDCall(Direct3DRMCreate(&m_pd3drm1));
        DDCall(m_pd3drm1->QueryInterface(IID_IDirect3DRM2, (void**)&m_pd3drm));

        DDCall(m_pd3drm->CreateFrame(NULL, &m_pframe));
        DDCall(m_pd3drm->CreateAnimationSet(&m_pset));
        DDCall(m_pd3drm->CreateMeshBuilder(&m_pbuilder));

        hr = m_pframe->Load((void*)(PCC)strPathIn, NULL, D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE, NULL, this);
        if (hr != D3DRMERR_NOTFOUND)
        {
            hr = m_pset->Load((void*)(PCC)strPathIn, NULL, D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE, NULL, this, NULL);

            if (hr != D3DRMERR_NOTFOUND)
            {
                DDCall(hr);
                DDCall(m_pset->SetTime(frame));
            }

            //New way ... Create a new root frame with a null transform to make sure
            //m_pbuilder gets the appropriately transformed mesh
            IDirect3DRMFrame2*  pframeNew;
            DDCall(m_pd3drm->CreateFrame(NULL, &pframeNew));
            DDCall(pframeNew->AddChild(m_pframe));

            DDCall(m_pbuilder->AddFrame(pframeNew));

            /* Old way ... less safe?
            DDCall(m_pbuilder->AddFrame(m_pframe));
            */
        }
        else
        {
            DDCall(m_pbuilder->Load((void*)(PCC)strPathIn, NULL, D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE, NULL, this));
        }

        DDCall(m_pbuilder->Save(strPathOut, D3DRMXOF_TEXT, D3DRMXOFSAVE_ALL));
    }

    void GetWeaponFrame(IDirect3DRMFrame2*     pframeTop,
                        IDirect3DRMFrame2*     pframeChild,
                        FILE*                  fOut)
    {
        DWORD   size = c_cbBfr;
        char    bfr[c_cbBfr];
        DDCall(pframeChild->GetName(&size, bfr));
        if (substr(bfr, "wepemt") || substr(bfr, "turwepemt") || substr(bfr, "missemt") ||
            substr(bfr, "cockpt") || substr(bfr, "launch") || substr(bfr, "garage") || substr(bfr, "capgarage"))
        {
            D3DVECTOR   pos;
            D3DVECTOR   dir;
            D3DVECTOR   up;

            pframeChild->GetPosition(pframeTop, &pos);
            pframeChild->GetOrientation(pframeTop, &dir, &up);

            fputs(bfr + 4, fOut);
            fprintf(fOut, "\n%20.10f%20.10f%20.10f\n", pos.x, pos.y, pos.z);
            fprintf(fOut, "%20.10f%20.10f%20.10f\n", dir.x, dir.y, dir.z);

            /*
            IDirect3DRMMeshBuilder2*    pbuilder;
            DDCall(m_pd3drm->CreateMeshBuilder(&pbuilder));
            DDCall(pbuilder->AddFrame(pframe2));

            char    fileout[256];
            sprintf(fileout, "%s_%d.mesh", (const char*)strPathOut, fileID++);
            DDCall(pbuilder->Save(fileout, D3DRMXOF_TEXT, D3DRMXOFSAVE_ALL));

            pbuilder->Release();
            */
        }

        LPDIRECT3DRMFRAMEARRAY pchildren;
        DDCall(pframeChild->GetChildren(&pchildren));

        DWORD nChildren = pchildren->GetSize();
        for (DWORD  i = 0; (i < nChildren); i++)
        {
            IDirect3DRMFrame* pframe;
            DDCall(pchildren->GetElement(i, &pframe));

            IDirect3DRMFrame2*  pframe2;
            DDCall(pframe->QueryInterface(IID_IDirect3DRMFrame2, (void**)&pframe2));

            GetWeaponFrame(pframeTop, pframe2, fOut);

            pframe2->Release();
            pframe->Release();
        }
        pchildren->Release();
    }

    void GetWeaponFrames(IDirect3DRMFrame2*     pframeTop,
                         const char*            pszOut)
    {
        char    bfr[c_cbBfr];
        strcpy(bfr, pszOut);
        strcat(bfr, ".dat");

        FILE*   fOut = fopen(bfr, "w");
        if (fOut)
        {
            LPDIRECT3DRMFRAMEARRAY pchildrenTop;
            DDCall(pframeTop->GetChildren(&pchildrenTop));

            DWORD nChildren = pchildrenTop->GetSize();
            for (DWORD  i = 0; (i < nChildren); i++)
            {
                IDirect3DRMFrame* pframeChild;
                DDCall(pchildrenTop->GetElement(i, &pframeChild));

                IDirect3DRMFrame2*  pframe2Child;
                DDCall(pframeChild->QueryInterface(IID_IDirect3DRMFrame2, (void**)&pframe2Child));

                GetWeaponFrame(pframeTop, pframe2Child, fOut);

                pframe2Child->Release();
                pframeChild->Release();
            }

            pchildrenTop->Release();

            fclose(fOut);
        }
    }

    void Extract(
        const PathString& strPathIn, 
        const PathString& strPathOut,
        float frame
    )
    {
        HRESULT                          hr;
        TRef<IDirect3DRM>                m_pd3drm1;
        TRef<IDirect3DRM2>               m_pd3drm;
        TRef<IDirect3DRMFrame2>          m_pframe;
        TRef<IDirect3DRMAnimationSet>    m_pset;

        DDCall(Direct3DRMCreate(&m_pd3drm1));
        DDCall(m_pd3drm1->QueryInterface(IID_IDirect3DRM2, (void**)&m_pd3drm));

        DDCall(m_pd3drm->CreateFrame(NULL, &m_pframe));
        DDCall(m_pd3drm->CreateAnimationSet(&m_pset));

        hr = m_pframe->Load((void*)(PCC)strPathIn, NULL, D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE, NULL, this);
        if (hr != D3DRMERR_NOTFOUND)
        {
            hr = m_pset->Load((void*)(PCC)strPathIn, NULL, D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE, NULL, this, NULL);

            if (hr != D3DRMERR_NOTFOUND)
            {
                DDCall(hr);
                DDCall(m_pset->SetTime(frame));
            }
            GetWeaponFrames(m_pframe, strPathOut);
        }
    }
    void Bound(
        const PathString& strPathIn, 
        const PathString& strPathOut,
        float frame
    )
    {
        HRESULT                          hr;
        TRef<IDirect3DRM>                m_pd3drm1;
        TRef<IDirect3DRM2>               m_pd3drm;
        TRef<IDirect3DRMFrame2>          m_pframe;
        TRef<IDirect3DRMAnimationSet>    m_pset;

        DDCall(Direct3DRMCreate(&m_pd3drm1));
        DDCall(m_pd3drm1->QueryInterface(IID_IDirect3DRM2, (void**)&m_pd3drm));

        DDCall(m_pd3drm->CreateFrame(NULL, &m_pframe));
        DDCall(m_pd3drm->CreateAnimationSet(&m_pset));

        hr = m_pframe->Load((void*)(PCC)strPathIn, NULL, D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE, NULL, this);
        if (hr != D3DRMERR_NOTFOUND)
        {
            hr = m_pset->Load((void*)(PCC)strPathIn, NULL, D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE, NULL, this, NULL);

            if (hr != D3DRMERR_NOTFOUND)
            {
                DDCall(hr);
                DDCall(m_pset->SetTime(frame));
            }

            {
                LPDIRECT3DRMFRAMEARRAY pchildren1;
                DDCall(m_pframe->GetChildren(&pchildren1));

                assert (pchildren1->GetSize() == 1);
                IDirect3DRMFrame* pframe1;
                DDCall(pchildren1->GetElement(0, &pframe1));

                LPDIRECT3DRMFRAMEARRAY pchildren2;
                DDCall(pframe1->GetChildren(&pchildren2));

                int fileID = 0;
                DWORD nChildren = pchildren2->GetSize();
                for (DWORD  i = 0; (i < nChildren); i++)
                {
                    IDirect3DRMFrame* pframe;
                    DDCall(pchildren2->GetElement(i, &pframe));

                    IDirect3DRMFrame2*  pframe2;
                    DDCall(pframe->QueryInterface(IID_IDirect3DRMFrame2, (void**)&pframe2));

                    static const DWORD  c_cbBfr = 256;
                    DWORD   size = c_cbBfr;
                    char    bfr[c_cbBfr];
                    DDCall(pframe2->GetName(&size, bfr));
                    if (substr(bfr, "bound"))
                    {
                        IDirect3DRMMeshBuilder2*    m_pbuilder;
                        DDCall(m_pd3drm->CreateMeshBuilder(&m_pbuilder));

                        IDirect3DRMFrame2*  pframeNew;
                        DDCall(m_pd3drm->CreateFrame(m_pframe, &pframeNew));
                        DDCall(pframeNew->AddChild(pframe2));

                        DDCall(m_pbuilder->AddFrame(pframeNew));

                        char    fileout[256];
                        sprintf(fileout, "%s_%d.mesh", (const char*)strPathOut, fileID++);
                        DDCall(m_pbuilder->Save(fileout, D3DRMXOF_TEXT, D3DRMXOFSAVE_ALL));

                        m_pbuilder->Release();
                        pframeNew->Release();
                    }

                    pframe2->Release();
                    pframe->Release();
                }

                pchildren2->Release();
                pchildren1->Release();
            }
        }
    }

    HRESULT Initialize(const ZString& strCommandLine)
    {
        PCC pcc = strCommandLine;
        CommandLineToken token(pcc, strCommandLine.GetLength());

        while (token.MoreTokens()) {
            ZString strInput;
            ZString strOutput;
            ZString str;

            if (token.IsMinus(str)) {
                if (m_mode == ModeNone) {
                    if (str == "reduce") {
                        if (token.IsNumber(m_scale)) {
                            m_mode = ModeReduce;
                            continue;
                        }
                    } else if (str == "flatten") {
                        if (token.IsNumber(m_frame)) {
                            m_mode = ModeFlatten;
                            continue;
                        }
                    } else if (str == "extract") {
                        if (token.IsNumber(m_frame)) {
                            m_mode = ModeExtract;
                            continue;
                        }
                    } else if (str == "bound") {
                        if (token.IsNumber(m_frame)) {
                            m_mode = ModeBound;
                            continue;
                        }
                    }
                }
            } else if (token.IsString(strInput)) {
                if (token.IsString(strOutput)) {
                    if (!token.MoreTokens()) {
                        switch (m_mode) {
                            case ModeReduce:
                                printf("Reducing " + strInput + "\n");
                                ReducePMesh(strInput, strOutput, m_scale);
                                return S_FALSE;

                            case ModeNone:
                            case ModeFlatten:
                                printf("Flattening " + strInput + "\n");
                                Flatten(strInput, strOutput, m_frame);
                                return S_FALSE;

                            case ModeExtract:
                                printf("Extracting " + strInput + "\n");
                                Extract(strInput, strOutput, m_frame);
                                return S_FALSE;

                            case ModeBound:
                                printf("Bounding " + strInput + "\n");
                                Bound(strInput, strOutput, m_frame);
                                return S_FALSE;

                        }
                    }
                }
            }
            break;
        }

        printf("Usage: XMunge [-reduce amount | -flatten frame] input output\n");

        return S_FALSE;
    }
} g_app;
