#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Tokenizer for text X-Files
//
//////////////////////////////////////////////////////////////////////////////

class XToken : public IObjectSingle {
public:
    virtual bool EnterSection(ZString& strType, ZString& strName) = 0;
    virtual bool LeaveSection() = 0;
    virtual bool ReadFloat(float& value) = 0;
    virtual bool ReadInt(int& value) = 0;
    virtual bool ReadString(ZString& str) = 0;
    virtual bool ReadReference(ZString& str) = 0;
    virtual bool Error() = 0;
    virtual void SetError(const ZString& str) = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// Tokenizer for text X-Files
//
//////////////////////////////////////////////////////////////////////////////

const WORD TOKEN_TEMPLATE     = 31;
const WORD TOKEN_NAME         = 1;
const WORD TOKEN_INTEGER_LIST = 6;
const WORD TOKEN_REALNUM_LIST = 7;
const WORD TOKEN_OBRACE       = 10;
const WORD TOKEN_CBRACE       = 11;

class XBinaryToken : public XToken {
private:
    PCC m_p;
    PCC m_pend;
    int m_intCount;
    int m_floatCount;
    ZString m_strError;

    bool MoreTokens()
    {
        return m_p < m_pend;
    }

    bool Is(WORD id)
    {
        if (*(WORD*)m_p == id) {
            m_p += sizeof(WORD);
            return true;
        }

        return false;
    }

    int GetInt()
    {
        int value = *((int*)m_p);
        m_p += sizeof(int);
        return value;
    }

    bool ReadString(ZString& str)
    {
        if (Is(TOKEN_NAME)) {
            int length = GetInt();
            str = ZString(m_p, length);
            m_p += length;
            return true;
        }

        return false;
    }

public:
    XBinaryToken(PCC p, int length) :
        m_p(p),
        m_pend(p + length),
        m_intCount(0),
        m_floatCount(0)
    {
    }

    bool EnterSection(ZString& strType, ZString& strName)
    {
        // skip any templates

        while (Is(TOKEN_TEMPLATE)) {
            while (!Is(TOKEN_CBRACE)) m_p += 2;
        }

        if (ReadString(strType)) {
            ReadString(strName);
            return Is(TOKEN_OBRACE);
        }

        return false;
    }

    bool LeaveSection()
    {
        int level = 1;

        ZString str;

        while (MoreTokens() && level > 0) {
            while (ReadString(str));
            if (Is(TOKEN_REALNUM_LIST)) {
                m_p += sizeof(float) * GetInt();
            } else if (Is(TOKEN_INTEGER_LIST)) {
                m_p += sizeof(int) * GetInt();
            } else if (Is(TOKEN_OBRACE)) {
                level++;
            } else if (Is(TOKEN_CBRACE)) {
                level--;
            } else {
                m_p += 2;
            }
        }

        return level == 0;
    }

    bool ReadFloat(float& value)
    {
        ZAssert(m_intCount == 0);
        if (m_floatCount == 0) {
            if (Is(TOKEN_REALNUM_LIST)) {
                m_floatCount = GetInt();
            } else {
                return false;
            }
        }

        m_floatCount--;
        value = *((float*)m_p);
        m_p += sizeof(float);
        return true;
    }

    bool ReadInt(int& value)
    {
        ZAssert(m_floatCount == 0);
        if (m_intCount == 0) {
            if (Is(TOKEN_INTEGER_LIST)) {
                m_intCount = GetInt();
            } else {
                return false;
            }
        }

        m_intCount--;
        value = *((int*)m_p);
        m_p += sizeof(int);
        return true;
    }

    bool ReadReference(ZString& str)
    {
        ZUnimplemented();
        return false;
    }

    bool Error()
    {
        return !m_strError.IsEmpty();
    }

    void SetError(const ZString& str)
    {
        m_strError = str;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Tokenizer for text X-Files
//
//////////////////////////////////////////////////////////////////////////////

class XTextToken : public XToken, public TextTokenImpl {
private:
    int Comma       ;
    int SemiColon   ;
    int LeftParen   ;
    int RightParen  ;
    int LeftBracket ;
    int RightBracket;
    int LeftBrace   ;
    int RightBrace  ;
    int Minus       ;
    int Dummy       ;

public:
    XTextToken(PCC pcc, int length) :
        TextTokenImpl(pcc, length)
    {
        Comma        = AddToken("','");
        SemiColon    = AddToken("';'");
        LeftParen    = AddToken("'('");
        RightParen   = AddToken("')'");
        LeftBracket  = AddToken("'['");
        RightBracket = AddToken("']'");
        LeftBrace    = AddToken("'{'");
        RightBrace   = AddToken("'}'");
        Minus        = AddToken("'-'");
        Dummy        = AddToken("dummy");

        Next();
    }

    int ParseToken(PCC& pcc)
    {
        switch (pcc[0]) {
            case '[': pcc++; return LeftBracket;
            case ']': pcc++; return RightBracket;
            case '{': pcc++; return LeftBrace;
            case '}': pcc++; return RightBrace;
            case '(': pcc++; return LeftParen;
            case ')': pcc++; return RightParen;
            case ',': pcc++; return Comma;
            case ';': pcc++; return SemiColon;
            case '-': pcc++; return Minus;
            case '<':
            case '>': pcc++; return Dummy;
        }

        return 0;
    }

    bool IsSymbolChar(char ch)
    {
        return TextTokenImpl::IsSymbolChar(ch) || ch == '-' || ch == '.';
    }

    bool Error()
    {
        return TextTokenImpl::Error();
    }

    void SetError(const ZString& str)
    {
        TextTokenImpl::SetError(str);
    }

    bool EnterSection(ZString& strType, ZString& strName)
    {
        if (IsSymbol(strType, false)) {
            IsSymbol(strName, false);
            return Is(LeftBrace, true);
        }

        return false;
    }

    bool LeaveSection()
    {
        int level = 1;

        while (MoreTokens() && level > 0) {
            if (Is(LeftBrace, false)) {
                level++;
            } else if (Is(RightBrace, false)) {
                level--;
            } else {
                Next();
            }
        }

        return level == 0;
    }

    bool ReadFloat(float& value)
    {
        bool fMinus = Is(Minus, false);

        if (IsNumber(value, true)) {
            while (Is(Comma, false) || Is(SemiColon, false));

            if (fMinus) {
                value = -value;
            }
            return true;
        }

        return false;
    }

    bool ReadInt(int& i)
    {
        float value;

        if (ReadFloat(value)) {
            i = (int)value;
            return true;
        }

        return false;
    }

    bool ReadString(ZString& str)
    {
        return IsString(str, true);
    }

    bool ReadReference(ZString& str)
    {
        if (Is(TOKEN_OBRACE, true)) {
            if (IsSymbol(str, true)) {
                if (Is(TOKEN_CBRACE, true)) {
                    return true;
                }
            }
        }

        return false;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Parser for X-Files
//
//////////////////////////////////////////////////////////////////////////////

class XParser {
    TRef<Modeler> m_pmodeler;
    TRef<XToken>  m_ptoken;
    TMap<ZString, TRef<TransformGeo> > m_namedFrames;

    int m_countMesh;
    int m_countFrame;

public:
    XParser(
        Modeler* pmodeler,
        PCC pcc,
        int length
    ) :
        m_pmodeler(pmodeler),
        m_countMesh(0),
        m_countFrame(0)
    {
        if (pcc[8] == 't') {
            m_ptoken = new XTextToken(pcc + 17, length - 17);
        } else if (pcc[8] == 'b') {
            m_ptoken = new XBinaryToken(pcc + 16, length - 16);
        }
    }

    ~XParser()
    {
    }

    bool ReadTransform(TRef<Transform>& ptrans)
    {
        ZString strType;
        ZString strName;

        if (m_ptoken->EnterSection(strType, strName)) {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;

            if (
                   m_ptoken->ReadFloat(m00)
                && m_ptoken->ReadFloat(m10)
                && m_ptoken->ReadFloat(m20)
                && m_ptoken->ReadFloat(m30)

                && m_ptoken->ReadFloat(m01)
                && m_ptoken->ReadFloat(m11)
                && m_ptoken->ReadFloat(m21)
                && m_ptoken->ReadFloat(m31)

                && m_ptoken->ReadFloat(m02)
                && m_ptoken->ReadFloat(m12)
                && m_ptoken->ReadFloat(m22)
                && m_ptoken->ReadFloat(m32)

                && m_ptoken->ReadFloat(m03)
                && m_ptoken->ReadFloat(m13)
                && m_ptoken->ReadFloat(m23)
                && m_ptoken->ReadFloat(m33)
            ) {
                Matrix
                    mat(
                        m00, m01, m02, m03,
                        m10, m11, m12, m13,
                        m20, m21, m22, m23,
                        m30, m31, m32, m33
                    );

                /*  , this should change a LH Transform to a RH Transform
                mat.PreScale(Vector(1, 1, -1));
                mat.Scale(Vector(1, 1, -1));
                */

                ptrans = new Transform(mat);

                m_ptoken->LeaveSection();

                return true;
            }
        }

        return false;
    }

    bool ReadMesh(const ZString strMeshName, TRef<Geo>& pgeo)
    {
        m_countMesh++;

        //
        // Read in the vertices
        //

        int vcount;
        if (!m_ptoken->ReadInt(vcount)) {
            return false;
        }

        TVector<Vertex> ppositions(vcount, vcount);

        int index;
        for(index = 0; index < vcount; index++) {
            Vertex& vertex = ppositions.Get(index);
            float z;

            if (
                   !m_ptoken->ReadFloat(vertex.x)
                || !m_ptoken->ReadFloat(vertex.y)
                || !m_ptoken->ReadFloat(z)
            ) {
                return false;
            }

            vertex.z = -z;
            vertex.u = 0;
            vertex.v = 0;
        }

        //
        // Read in the vertices for the faces
        //

        int countFace;
        if (!m_ptoken->ReadInt(countFace)) {
            return false;
        }

        TVector<WORD> pvi;
        TVector<int> pfaceVCounts(countFace, countFace);

        for(index = 0; index < countFace; index++) {
            int firstIndex ;
            int secondIndex;
            if (
                   !m_ptoken->ReadInt(pfaceVCounts.Get(index))
                || !m_ptoken->ReadInt(firstIndex )
                || !m_ptoken->ReadInt(secondIndex)
            ) {
                return false;
            }

            for (int faceVIndex = 0; faceVIndex < pfaceVCounts[index] - 2; faceVIndex++) {
                int thirdIndex;
                if (!m_ptoken->ReadInt(thirdIndex)) {
                    return false;
                }

                pvi.PushEnd(firstIndex);
                pvi.PushEnd(secondIndex);
                pvi.PushEnd(thirdIndex);

                secondIndex = thirdIndex;
            }
        }

        //
        // Read in the materials
        //

        int                      mcount = 0;
        int                      matCountFace = 0;
        TVector<TRef<Material> > pmaterials;
        TVector<TRef<Image> >    pimageTextures;
        TVector<WORD>            pmi;

        int             ncount = 0;
        TVector<Vector> pnormals;
        TVector<WORD>   pni;

        ZString strType;
        ZString strName;

        while (m_ptoken->EnterSection(strType, strName)) {
            if (strType == "MeshMaterialList") {
                if (
                       !m_ptoken->ReadInt(mcount)
                    || !m_ptoken->ReadInt(matCountFace)
                ) {
                    return false;
                }

                ZAssert(matCountFace == 1 || countFace == matCountFace);

                //
                // read in per face material indicies
                //

                for(index = 0; index < matCountFace; index++) {
                    int mi;
                    if (!m_ptoken->ReadInt(mi)) {
                        return false;
                    }
                    pmi.PushEnd(mi);
                }

                //
                // read in the materials
                //

                for (index = 0; index < mcount; index++) {
                    ZVerify(m_ptoken->EnterSection(strType, strName));
                    ZAssert(strType == "Material");

                    float rd, gd, bd, ad;
                    float power;
                    float rs, gs, bs;
                    float re, ge, be;

                    if (
                           !m_ptoken->ReadFloat(rd)
                        || !m_ptoken->ReadFloat(gd)
                        || !m_ptoken->ReadFloat(bd)
                        || !m_ptoken->ReadFloat(ad)
                        || !m_ptoken->ReadFloat(power)
                        || !m_ptoken->ReadFloat(rs)
                        || !m_ptoken->ReadFloat(gs)
                        || !m_ptoken->ReadFloat(bs)
                        || !m_ptoken->ReadFloat(re)
                        || !m_ptoken->ReadFloat(ge)
                        || !m_ptoken->ReadFloat(be)
                    ) {
                        return false;
                    }

                    Color colorDiffuse(rd, gd, bd, ad);
                    Color colorSpecular(rs, gs, bs);
                    Color colorEmissive(re, ge, be);

                    TRef<Image> pimageTexture;

                    if (m_ptoken->EnterSection(strType, strName)) {
                        ZAssert(strType == "TextureFilename");
                        PathString strFilename;
                        if (!m_ptoken->ReadString(strFilename)) {
                            return false;
                        }

                        ZAssert(
                               strFilename.GetExtension() == "ppm"
                            || strFilename.GetExtension() == "bmp"
                        );

                        strFilename = strFilename.LeftOf(4).ToLower() + "bmp";

                        pimageTexture = m_pmodeler->LoadImage(strFilename, false);
                        m_ptoken->LeaveSection();
                    } else if (m_ptoken->Error()) {
                        return false;
                    }

                    pmaterials.PushEnd(
                        CreateMaterial(
                            colorDiffuse,
                            colorSpecular,
                            colorEmissive,
                            power
                        )
                    );
                    pimageTextures.PushEnd(pimageTexture);
                    m_ptoken->LeaveSection();
                }
            } else if (strType == "MeshVertexColors" ) {
                //  , parse this?
            } else if (strType == "MeshFaceWraps"    ) {
                //  , parse this?
            } else if (strType == "MeshTextureCoords") {
                int vcountTextureCoords;
                if (!m_ptoken->ReadInt(vcountTextureCoords)) {
                    return false;
                }
                ZAssert(vcount == vcountTextureCoords);

                for(index = 0; index < vcount; index++) {
                    Vertex& vertex = ppositions.Get(index);

                    if (
                           !m_ptoken->ReadFloat(vertex.u)
                        || !m_ptoken->ReadFloat(vertex.v)
                    ) {
                        return false;
                    }
                }
            } else if (strType == "MeshNormals") {
                //
                // Read in the normals
                //

                if (!m_ptoken->ReadInt(ncount)) {
                    return false;
                }

                pnormals.SetCount(ncount);

                for(index = 0; index < ncount; index++) {
                    float x, y, z;
                    if (
                           !m_ptoken->ReadFloat(x)
                        || !m_ptoken->ReadFloat(y)
                        || !m_ptoken->ReadFloat(z)
                    ) {
                        return false;
                    }

                    pnormals.Get(index) = Vector(x, y, -z);
                }

                //
                // Read in the normals for the faces
                //

                int countFaceNormals;
                if (!m_ptoken->ReadInt(countFaceNormals)) {
                    return false;
                }

                ZAssert(countFace == countFaceNormals);

                for(index = 0; index < countFace; index++) {
                    int faceVCount ;
                    int firstIndex ;
                    int secondIndex;

                    if (
                           !m_ptoken->ReadInt(faceVCount )
                        || !m_ptoken->ReadInt(firstIndex )
                        || !m_ptoken->ReadInt(secondIndex)
                    ) {
                        return false;
                    }

                    for (int faceVIndex = 0; faceVIndex < faceVCount - 2; faceVIndex++) {
                        int thirdIndex;
                        if (!m_ptoken->ReadInt(thirdIndex)) {
                            return false;
                        }

                        pni.PushEnd(firstIndex);
                        pni.PushEnd(secondIndex);
                        pni.PushEnd(thirdIndex);

                        secondIndex = thirdIndex;
                    }
                }
            }
            m_ptoken->LeaveSection();
        }

        if (m_ptoken->Error()) {
            return false;
        }

        //
        // Create unique Position Normal pairs
        //

        TVector<Vertex> vertices;
        TVector<WORD> indices;

        if (ncount > 0) {
            TVector<TList<WORD> > vertexTree(vcount, vcount);
            int count = pvi.GetCount();

            for (index = 0; index < count; index++) {
                int iVertex = pvi[index];
                int iNormal = pni[index];

                TList<WORD>::Iterator iter(vertexTree.Get(iVertex));

                do {
                    if (iter.End()) {
                        // create a new vertex

                        Vertex vertex;

                        vertex.x  = ppositions[iVertex].x;
                        vertex.y  = ppositions[iVertex].y;
                        vertex.z  = ppositions[iVertex].z;
                        vertex.u  = ppositions[iVertex].u;
                        vertex.v  = ppositions[iVertex].v;
                        vertex.nx = pnormals[iNormal].X();
                        vertex.ny = pnormals[iNormal].Y();
                        vertex.nz = pnormals[iNormal].Z();

                        vertices.PushEnd(vertex);
                        indices.PushEnd(vertices.GetCount() - 1);
                        vertexTree.Get(iVertex).PushFront(vertices.GetCount() - 1);
                        break;
                    }

                    Vertex& vertex = vertices.Get(iter.Value());

                    if (
                        vertex.nx == pnormals[iNormal].X()
                     && vertex.ny == pnormals[iNormal].Y()
                     && vertex.nz == pnormals[iNormal].Z()
                    ) {
                        indices.PushEnd(iter.Value());
                        break;
                    }

                    iter.Next();
                } while (true);
            }
        } else {
            vertices = ppositions;
            indices   = pvi;
        }

        //
        //  Split up the mesh based on materials
        //

        if (mcount == 0) {
            pgeo = Geo::CreateMesh(vertices, indices);
            return true;
        } else if (matCountFace == 1 || mcount == 1) {
            if (pimageTextures[0]) {
                pgeo =
                    new TextureGeo(
                        new MaterialGeo(
                            Geo::CreateMesh(vertices, indices),
                            pmaterials[0]
                        ),
                        pimageTextures[0]
                    );
                return true;
            } else {
                pgeo =
                    new MaterialGeo(
                        Geo::CreateMesh(vertices, indices),
                        pmaterials[0]
                    );
                return true;
            }
        }

        //
        // Multiple Materials
        //

        TRef<GroupGeo> pgroup = GroupGeo::Create();
        TVector<Vertex> componentVertices;
        TVector<WORD> componentIndices;
        TVector<WORD> map(vertices.GetCount(), vertices.GetCount());

        //
        // for each material create a smaller mesh for all the faces that have that material
        //

        for (int indexMaterial = 0; indexMaterial < mcount; indexMaterial++) {
            for (index = 0; index < map.GetCount(); index++) {
                map.Set(index, 0xffff);
            }

            //
            // set the small mesh to empty
            //

            componentVertices.SetCount(0);
            componentIndices.SetCount(0);

            //
            // add all of the verticies and faces for which the material matches
            //

            int indexStart = 0;
            for (int indexFace = 0; indexFace < countFace; indexFace++) {
                int indexEnd = indexStart + (pfaceVCounts[indexFace] - 2) * 3;

                if (pmi[indexFace] == indexMaterial) {
                    for (
                        index = indexStart;
                        index < indexEnd;
                        index++
                    ) {
                        int indexVertex = indices[index];
                        if (map[indexVertex] == 0xffff) {
                            map.Set(indexVertex, componentVertices.GetCount());
                            componentVertices.PushEnd(vertices[indexVertex]);
                        }
                        componentIndices.PushEnd(map[indexVertex]);
                    }
                }

                indexStart = indexEnd;
            }

            //
            // create the mesh and stick it into the group
            //

            if (pimageTextures[indexMaterial]) {
                pgroup->AddGeo(
                    new TextureGeo(
                        new MaterialGeo(
                            Geo::CreateMesh(componentVertices, componentIndices),
                            pmaterials[indexMaterial]
                        ),
                        pimageTextures[indexMaterial]
                    )
                );
            } else {
                pgroup->AddGeo(
                    new MaterialGeo(
                        Geo::CreateMesh(componentVertices, componentIndices),
                        pmaterials[indexMaterial]
                    )
                );
            }
        }

        pgeo = pgroup;
        return true;
    }

    bool ReadFrame(const ZString& strFrameName, TRef<Geo>& pgeo)
    {
        m_countFrame++;

        //
        // Create a group
        //

        TRef<GroupGeo> pgeoGroup = GroupGeo::Create();
        pgeoGroup->SetName(strFrameName);

        //
        // Create the group transform
        //

        TRef<Transform> ptrans;
        if (!ReadTransform(ptrans)) {
            if (m_ptoken->Error()) {
                return false;
            }

            ptrans = new Transform();
        }

        TRef<TransformGeo> ptransformGeo = new TransformGeo(pgeoGroup, ptrans);

        //
        // return the transformGeo
        //

        pgeo = ptransformGeo;

        //
        // remember the the name of the transform so we can replace it
        // if it is animated
        //

        m_namedFrames.Set(strFrameName, ptransformGeo);

        //
        // read the children of this frame
        //

        ZString strType;
        ZString strName;

        while (m_ptoken->EnterSection(strType, strName)) {
            TRef<Geo> pgeoChild;
            if (strType == "Frame") {
                if (!ReadFrame(strName, pgeoChild)) {
                    return false;
                }
            } else if (strType == "Mesh") {
                if (!ReadMesh(strName, pgeoChild)) {
                    return false;
                }
            } else {
                m_ptoken->SetError("Unknown Section Type");
                return false;
            }

            pgeoGroup->AddGeo(pgeoChild);
            m_ptoken->LeaveSection();
        }

        return !m_ptoken->Error();
    }

    bool ReadAnimationKey(
        Number* pnumberFrame,
        TRef<Transform>& ptransformTranslate,
        TRef<Transform>& ptransformScale,
        TRef<Transform>& ptransformRotate
    ) {
        int type ;
        int count;

        if (
               !m_ptoken->ReadInt(type )
            || !m_ptoken->ReadInt(count)
        ) {
            return false;
        }

        switch (type) {
            case 0:
                {
                    // rotation keys

                    TRef<KeyFramedRotateTransform> ptrans = CreateKeyFramedRotateTransform(pnumberFrame);
                    ptransformRotate = ptrans;

                    for (int index = 0; index < count; index++) {
                        int time;
                        int count;
                        float s;
                        float x;
                        float y;
                        float z;

                        if (
                               !m_ptoken->ReadInt(time)
                            || !m_ptoken->ReadInt(count)
                            || !m_ptoken->ReadFloat(s)
                            || !m_ptoken->ReadFloat(x)
                            || !m_ptoken->ReadFloat(y)
                            || !m_ptoken->ReadFloat(z)
                        ) {
                            return false;
                        }

                        ZAssert(count == 4);

                        ptrans->AddKey((float)time, Quaternion(s, x, y, -z));
                    }
                }
                break;

            case 1:
                {
                    // scale keys

                    TRef<KeyFramedScaleTransform> ptrans = CreateKeyFramedScaleTransform(pnumberFrame);
                    ptransformScale = ptrans;

                    for (int index = 0; index < count; index++) {
                        int time;
                        int count;
                        float x;
                        float y;
                        float z;

                        if (
                               !m_ptoken->ReadInt(time)
                            || !m_ptoken->ReadInt(count)
                            || !m_ptoken->ReadFloat(x)
                            || !m_ptoken->ReadFloat(y)
                            || !m_ptoken->ReadFloat(z)
                        ) {
                            return false;
                        }

                        ZAssert(count == 3);

                        ptrans->AddKey((float)time, Vector(x, y, z));
                    }
                }
                break;

            case 2:
                {
                    // Translate keys

                    TRef<KeyFramedTranslateTransform> ptrans = CreateKeyFramedTranslateTransform(pnumberFrame);
                    ptransformTranslate = ptrans;

                    for (int index = 0; index < count; index++) {
                        int time;
                        int count;
                        float x;
                        float y;
                        float z;

                        if (
                               !m_ptoken->ReadInt(time)
                            || !m_ptoken->ReadInt(count)
                            || !m_ptoken->ReadFloat(x)
                            || !m_ptoken->ReadFloat(y)
                            || !m_ptoken->ReadFloat(z)
                        ) {
                            return false;
                        }

                        ZAssert(count == 3);

                        ptrans->AddKey((float)time, Vector(x, y, -z));
                    }
                }
                break;

            default:
                m_ptoken->SetError("Invalid key type");
                return false;
        }

        return true;
    }

    bool ReadAnimation(Number* pnumberFrame)
    {
        ZString strFrameName;
        if (m_ptoken->ReadReference(strFrameName)) {
            //
            // Read in any Animation Keys
            //

            TRef<Transform> ptransformTranslate;
            TRef<Transform> ptransformRotate;
            TRef<Transform> ptransformScale;
            ZString strType;
            ZString strName;

            while (m_ptoken->EnterSection(strType, strName)) {
                if (strType == "AnimationKey") {
                    if (!ReadAnimationKey(pnumberFrame, ptransformTranslate, ptransformScale, ptransformRotate)) {
                        return false;
                    }
                } else if (strType == "AnimationOptions") {
                    ZError("Not Implemented");
                }
                m_ptoken->LeaveSection();
            }

            //
            // if there were any transforms replace the old transform
            //

            if (!m_ptoken->Error()) {
                if (ptransformRotate || ptransformScale || ptransformTranslate) {
                    //
                    // Find the frame we will be animating
                    //

                    TRef<TransformGeo> ptransformGeo;
                    ZVerify(m_namedFrames.Find(strFrameName, ptransformGeo));

                    //
                    // build the new transformed geo
                    //

                    TRef<Geo> pgeo = ptransformGeo->GetGeo();

                    if (ptransformRotate) {
                        pgeo = new TransformGeo(pgeo, ptransformRotate);
                    }

                    if (ptransformScale) {
                        pgeo = new TransformGeo(pgeo, ptransformScale);
                    }

                    if (ptransformTranslate) {
                        pgeo = new TransformGeo(pgeo, ptransformTranslate);
                    }

                    ptransformGeo->ChangeTo(pgeo);
                }

                return true;
            }
        }

        return false;
    }

    bool ReadAnimationSet(Number* pnumberFrame)
    {
        ZString strType;
        ZString strName;

        while (m_ptoken->EnterSection(strType, strName)) {
            if (strType == "Animation") {
                if (!ReadAnimation(pnumberFrame)) {
                    return false;
                }
            }
            m_ptoken->LeaveSection();
        }

        return !m_ptoken->Error();
    }

    TRef<Geo> ReadFile(Number* pnumberFrame, bool& bAnimation)
    {
        TRef<GroupGeo> pgroup = GroupGeo::Create();

        ZString strType;
        ZString strName;

        while (m_ptoken->EnterSection(strType, strName)) {
            TRef<Geo> pgeoChild;

            if (strType == "Frame") {
                if (!ReadFrame(strName, pgeoChild)) {
                    return false;
                }
            } else if (strType == "Mesh") {
                if (!ReadMesh(strName, pgeoChild)) {
                    return false;
                }
            } else if (strType == "AnimationSet") {
                ZAssert(!bAnimation);
                bAnimation = true;
                if (!ReadAnimationSet(pnumberFrame)) {
                    return false;
                }
            }

            if (pgeoChild) {
                pgroup->AddGeo(pgeoChild);
            }

            m_ptoken->LeaveSection();
        }

        if (m_ptoken->Error()) {
            return NULL;
        } else {
            return pgroup;
        }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Parse Animation Data in X-File
//
//////////////////////////////////////////////////////////////////////////////

TRef<Geo> ImportXFile(Modeler* pmodeler, ZFile* pfile, Number* pnumberFrame, bool& bAnimation)
{
    XParser parser(pmodeler, (PCC)pfile->GetPointer(), pfile->GetLength());

    bAnimation = false;

    return parser.ReadFile(pnumberFrame, bAnimation);
}
