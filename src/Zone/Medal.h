



class IMedalBase  // inherited by IZoneMedal
{
public:
    virtual void            SetName(const char * szName)                  = 0;
    virtual void            SetDescription(const char * m_szDescription)  = 0;
    virtual void            SetRanking(int nRanking)                      = 0; // used for sorting
    virtual void            SetImageFileName(const char * szFileName)     = 0;

    virtual char *          GetName()           = 0;
    virtual char *          GetDescription()    = 0;
    virtual int             GetRanking()        = 0;
};

