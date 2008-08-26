//////////////////////////////////////////////////////////////////////////////
//
// RM Stuff
//
//////////////////////////////////////////////////////////////////////////////

    /*
    static HRESULT TextureCallback(
        char *tex_name,
        void *lpArg,
        LPDIRECT3DRMTEXTURE *lpD3DRMTex
    ) {
        MDLEditWindow* pwindow = (MDLEditWindow*)lpArg;

        lpD3DRMTex = NULL;

        return S_OK;
    }

    void DoPMesh()
    {
        TRef<IDirect3DRM>                m_pd3drm1;
        TRef<IDirect3DRM2>               m_pd3drm;
        TRef<IDirect3DRMProgressiveMesh> m_ppmesh;
        TRef<IDirect3DRMMesh>            m_pmesh;
        TRef<IDirect3DRMMeshBuilder2>    m_pbuilder;

        DDCall(Direct3DRMCreate(&m_pd3drm1));
        DDCall(m_pd3drm1->QueryInterface(IID_IDirect3DRM2, (void**)&m_pd3drm));

        DDCall(m_pd3drm->CreateProgressiveMesh(&m_ppmesh));
        DDCall(m_ppmesh->Load(
            "pm_tpot.x",
            NULL,
            D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE,
            NULL,
            this
        ));

        DDCall(m_ppmesh->SetDetail(0.5f));
        DDCall(m_ppmesh->CreateMesh(&m_pmesh));


        DDCall(m_pd3drm->CreateMeshBuilder(&m_pbuilder));

        DDCall(m_pbuilder->AddMesh(m_pmesh));
        DDCall(m_pbuilder->Save("tpot.x", D3DRMXOF_TEXT, D3DRMXOFSAVE_ALL));
    }
    */

