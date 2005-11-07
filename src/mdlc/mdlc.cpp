//////////////////////////////////////////////////////////////////////////////
//
// MDL Compiler
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// The main entry point
//
//////////////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////////////
//
// ModelerSite
//
//////////////////////////////////////////////////////////////////////////////

class ModelerSiteImpl : public ModelerSite {
public:
    void Error(const ZString& str)
    {
        printf(str);
        _exit(0);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// MDLC Application
//
//////////////////////////////////////////////////////////////////////////////

class MDLCApp : public EffectApp {
public:
    void DoCompile(const PathString& strInput, const PathString& strOutput)
    {
        //
        // Get the basename
        //

        ZString strName       = strInput.GetName();
        ZString strOutputName = strOutput.GetName();

        //
        // Load an mdl file
        //

        TRef<INameSpace> pns = GetModeler()->GetNameSpace(strInput);

        if (pns) {
            //
            // Save the file in mdl format
            //

            {
                TRef<ZFile> pfile = new ZWriteFile(strOutput);

                if (pfile->IsValid()) {
                    pns->WriteToBinaryFile(pfile);
                }
            }

            //
            // Load the compiled mdl file to make sure there are no errors.
            //

            GetModeler()->UnloadNameSpace(strOutput);
            TRef<INameSpace> pnsReloaded = GetModeler()->GetNameSpace(strOutput);
            ZAssert(pnsReloaded != NULL);
        }
    }

    void DoInfo(const ZString& strInput)
    {
        TRef<INameSpace> pns = GetModeler()->GetNameSpace(strInput);

        if (pns) {
            TRef<Geo> pgeo; CastTo(pgeo, pns->FindMemberLocal("object"));
            TRef<LODGeo> plodGeo; CastTo(plodGeo, pgeo);

            if (plodGeo) {
                printf(plodGeo->GetInfo());
                return;
            }

            printf("mdlc: Unrecognized object type. No info avaliable");
        }
    }

    void DoGenHeader(const ZString& strInput, const ZString& strOutput)
    {
        TRef<INameSpace> pns = GetModeler()->GetNameSpace(strInput);

        if (pns) {
            TRef<ZFile> pfile = new ZWriteFile(strOutput);

            pns->WriteTypeHeader(pfile);
        }
    }

    void DoOptimize(const ZString& strInput, const ZString& strOutput)
    {
        printf(ZString("Optimizing ") + strInput + "\n");

        //
        // Load an mdl file
        //

        TRef<INameSpace> pns = GetModeler()->GetNameSpace(strInput);

        if (pns) {
            //
            // Optimize it
            //

            TRef<Number> pnumber = Number::Cast(pns->FindMember("frame"));
            TRef<Geo>    pgeo    = Geo::Cast(pns->FindMember("object"));

            TRef<INameSpace> pnsThing = OptimizeThingGeo(strOutput, pgeo, pnumber);

            if (pnsThing) {
                //
                // Save the file in mdl format
                //

                {
                    TRef<ZFile> pfile = new ZWriteFile(strOutput + ".mdl");

                    if (pfile->IsValid()) {
                        pnsThing->WriteToBinaryFile(pfile);
                    }
                }

                //
                // Load the optimized mdl file to make sure there are no errors.
                //

                GetModeler()->UnloadNameSpace(strOutput);
                TRef<INameSpace> pnsReloaded = GetModeler()->GetNameSpace(strOutput);
                ZAssert(pnsReloaded != NULL);
            }
        }
    }

    void DoConvert(const PathString& strInput, const PathString& strOutput)
    {
        printf(ZString("Converting image ") + strInput + "\n");

        //
        // Get the basenames for the bitmap files
        //

        ZString strName       = strInput.GetName().ToLower();
        ZString strOutputName = strOutput.GetName().ToLower();

        //
        // Load the bitmap
        //

        HBITMAP hbitmap = GetModeler()->LoadBitmap(strInput);
        TRef<Surface> psurface = GetEngine()->CreateSurface(hbitmap);
        ZVerify(::DeleteObject(hbitmap));

        //
        // Convert the bitmap to 16bpp
        //

        TRef<Surface> psurface16 =
            GetEngine()->CreateSurface(
                psurface->GetSize(),
                new PixelFormat(16, 0xf800, 0x07e0, 0x001f, 0x0000)
            );

        psurface16->BitBlt(WinPoint(0, 0), psurface);

        //
        // Create a namespace with the bitmap in it
        //

        TRef<INameSpace> pns = GetModeler()->CreateNameSpace(strOutputName, GetModeler()->GetNameSpace("model"));
        pns->AddMember(strOutputName, (Value*)new ConstantImage(psurface16, strName));

        //
        // Write out the namespace
        //

        TRef<ZFile> pfile = new ZWriteFile(strOutput);
        if (pfile->IsValid()) {
            pns->WriteToBinaryFile(pfile);
        }
    }

    void DoCompressAnim(
        int xframes,
        int yframes,
        const PathString& strInput, 
        const PathString& strOutput
    ) {
        printf(ZString("Compressing animation ") + strInput + "\n");

        //
        // Get the basenames for the bitmap files
        //

        ZString strName       = strInput.GetName().ToLower();
        ZString strOutputName = strOutput.GetName().ToLower();

        //
        // Create the frame image
        //

        TRef<Number> pnumberFrame = new ModifiableNumber(0);

        TRef<Image> pimageFrame =
            CreateFrameImage(
                pnumberFrame,
                GetModeler()->LoadSurface(strInput, false),
                xframes,
                yframes
            );

        //
        // Create the name space
        //

        TRef<INameSpace> pns = 
            GetModeler()->CreateNameSpace(
                strOutputName, 
                GetModeler()->GetNameSpace("model")
            );

        pns->AddMember("frame", pnumberFrame);
        pns->AddMember("image", (Value*)pimageFrame);

        //
        // Write out the namespace
        //

        TRef<ZFile> pfile = new ZWriteFile(strOutput);
        if (pfile->IsValid()) {
            pns->WriteToBinaryFile(pfile);
        }
    }

    HRESULT Initialize(const ZString& strCommandLine)
    {
        if (SUCCEEDED(EffectApp::Initialize(strCommandLine + " -software"))) {
            GetModeler()->SetSite(new ModelerSiteImpl());

            //
            // Parse the command line
            //

            PCC pcc = strCommandLine;
            CommandLineToken token(pcc, strCommandLine.GetLength());

            while (token.MoreTokens()) {
                ZString str;

                if (token.IsMinus(str)) {
                    if (str == "optimize") {
                        ZString strInput;
                        ZString strOutput;
                        if (token.IsString(strInput)) {
                            if (token.IsString(strOutput)) {
                                DoOptimize(strInput, strOutput);
                                return S_FALSE;
                            }
                        }
                    } else if (str == "compressanim") {
                        ZString strInput;
                        ZString strOutput;
                        float xframes;
                        float yframes;
                        if (token.IsNumber(xframes)) {
                            if (token.IsNumber(yframes)) {
                                if (token.IsString(strInput)) {
                                    if (token.IsString(strOutput)) {
                                        DoCompressAnim((int)xframes, (int)yframes, strInput, strOutput);
                                        return S_FALSE;
                                    }
                                }
                            }
                        }
                    } else if (str == "convert") {
                        ZString strInput;
                        ZString strOutput;
                        if (token.IsString(strInput)) {
                            if (token.IsString(strOutput)) {
                                DoConvert(strInput, strOutput);
                                return S_FALSE;
                            }
                        }
                    } else if (str == "info") {
                        ZString strInput;
                        if (token.IsString(strInput)) {
                            DoInfo(strInput);
                            return S_FALSE;
                        }
                    } else if (str == "genheader") {
                        ZString strInput;
                        ZString strOutput;
                        if (token.IsString(strInput)) {
                            if (token.IsString(strOutput)) {
                                DoGenHeader(strInput, strOutput);
                                return S_FALSE;
                            }
                        }
                    }
                } else {
                    ZString strInput;
                    ZString strOutput;
                    if (token.IsString(strInput)) {
                        if (token.IsString(strOutput)) {
                            DoCompile(strInput, strOutput);
                            return S_FALSE;
                        }
                    }

                    return E_FAIL;                        
                }
            }
        }

        return E_FAIL;
    }

    void Terminate()
    {
        EffectApp::Terminate();
    }
} g_app;
