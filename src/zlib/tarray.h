#ifndef _TArray_H_
#define _TArray_H_

//////////////////////////////////////////////////////////////////////////////
//
// Array Template
//
//////////////////////////////////////////////////////////////////////////////

template<class TValue, int count>
class TArray {
protected:
    TValue  m_pvalue[count];

public:
    TArray()
    {
    }

    TArray(TValue pvalue[count])
    {
        for (int index = 0; index < count; index++) {
            m_pvalue[index] = pvalue[index];
        }
    }

    TArray(const TArray& tvec)
    {
        for (int index = 0; index < count; index++) {
            m_pvalue[index] = tvec.m_pvalue[index];
        }
    }

    void operator=(const TArray& tvec)
    {
        for (int index = 0; index < count; index++) {
            m_pvalue[index] = tvec.m_pvalue[index];
        }
    }

    TValue& operator[](int index)
    {
        ZAssert(index >= 0 && index < count);

        return m_pvalue[index];
    }

    int GetCount() const
    {
        return count;
    }
};

#endif
