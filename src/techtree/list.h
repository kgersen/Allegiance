//
//  File:   list.h
//
//  Description:
//      This file contains codes for a linked list template that
//      can be used as a stack or a queue.
//
//  History:
//      Mar  5, 1997    jfink       Created file.
//      Jan  7, 1998    jfink       Added Empty method.
//


#ifndef _LIST_H_
#define _LIST_H_

#include "base.h"

template <class TElement> class FListIterator
{
public:
    virtual void OnIterate(TElement* pElement, DWORD dw, void* pv) = 0;
} ;

template <class TElement> class FList : public CBase
{
    protected:
        TElement *mpHead, *mpTail;
        DWORD mcElements;

    public:

        FList();
        virtual ~FList();

        DWORD Length();
        VOID Enqueue(TElement *pElement);
        TElement *Dequeue();
        VOID Push(TElement *pElement);
        TElement *Pop();
        TElement *Find(const TElement &Element);
        TElement *Find(TElement *pStart, const TElement &Element);
        TElement *Remove(const TElement &Element);
        TElement *Remove(TElement *pELement);
        void Iterate(FListIterator<TElement>* pIterator, DWORD dw, void* pv);
        VOID Empty();
} ;


template <class TElement>
FList<TElement>::FList() : CBase()
{
    mpHead = mpTail = NULL;
    mcElements = 0;
}


template <class TElement>
FList<TElement>::~FList()
{
    this->Empty();
}


template <class TElement>
DWORD FList<TElement>::Length()
{
    return(mcElements);
}


template <class TElement>
VOID FList<TElement>::Enqueue(TElement *pElement)
{
    this->Lock();

    if (NULL == mpHead)
        mpHead = pElement;
    else
        mpTail->mpNext = pElement;

    mpTail = pElement;
    pElement->mpNext = NULL;
    mcElements++;

    this->Unlock();
}


template <class TElement>
TElement *FList<TElement>::Dequeue()
{
    TElement *pTemp;

    this->Lock();

    pTemp = mpHead;
    if (NULL != mpHead)
    {
        mpHead = mpHead->mpNext;
        pTemp->mpNext = NULL;
        mcElements--;
    } else
        mpTail = NULL;

    this->Unlock();
    return(pTemp);
}

template <class TElement>
VOID FList<TElement>::Iterate(FListIterator<TElement>* pIterator, DWORD dw, void* pv)
{
    if (!pIterator)
        return;

    this->Lock();

    for (TElement* pTemp = mpHead; NULL != pTemp; pTemp = pTemp->mpNext)
    {
        pIterator->OnIterate(pTemp, dw, pv);
    }

    this->Unlock();
}

template <class TElement>
VOID FList<TElement>::Push(TElement *pElement)
{
    if (pElement)
    {
        this->Lock();

        if (NULL == mpHead)
            mpTail = pElement;

        pElement->mpNext = mpHead;
        mpHead = pElement;
        mcElements++;

        this->Unlock();
    }
}


template <class TElement>
TElement *FList<TElement>::Pop()
{
    return(this->Dequeue());
}


//
// Find the element in the list, starting a pStart, whose contents
// match Element.
//
template <class TElement>
TElement *FList<TElement>::Find(TElement *pStart, const TElement &Element)
{
    TElement *pLoop;
    
    this->Lock();

    for(pLoop = pStart; NULL != pLoop; pLoop = pLoop->mpNext)
    {
        if (pLoop->Find(Element))
            break;
    }

    this->Unlock();
    return(pLoop);
}


//
// Find the element in the list, starting at the head, whose contents
// match Element.
//
template <class TElement>
TElement *FList<TElement>::Find(const TElement &Element)
{
    return(this->Find(mpHead, Element));
}


//
// Remove the element from the list whose contents match
// Element.
//
template <class TElement>
TElement *FList<TElement>::Remove(const TElement &Element)
{
    TElement *pLoop, *pPrev;
    
    this->Lock();

    for(pPrev = NULL, pLoop = mpHead;
        NULL != pLoop;
        pPrev = pLoop, pLoop = pLoop->mpNext)
    {
        if (*pLoop == Element)
        {
            //
            // Remove the node from the list.
            //
            if (pLoop == mpHead)
                mpHead = pLoop->mpNext;
            else
                pPrev->mpNext = pLoop->mpNext;

            if (pLoop == mpTail)
                mpTail = pPrev;

            pLoop->mpNext = NULL;
            mcElements--;
            break;
        }
    }

    this->Unlock();
    return(pLoop);
}


//
// Remove the element from the list whose pointer matches
// pElement.
//
template <class TElement>
TElement *FList<TElement>::Remove(TElement * pElement)
{
    TElement *pLoop, *pPrev;
    
    this->Lock();

    for(pPrev = NULL, pLoop = mpHead;
        NULL != pLoop;
        pPrev = pLoop, pLoop = pLoop->mpNext)
    {
        if (pLoop == pElement)
        {
            //
            // Remove the node from the list.
            //
            if (pLoop == mpHead)
                mpHead = pLoop->mpNext;
            else
                pPrev->mpNext = pLoop->mpNext;

            if (pLoop == mpTail)
                mpTail = pPrev;

            pLoop->mpNext = NULL;
            mcElements--;
            break;
        }
    }

    this->Unlock();
    return(pLoop);
}


template <class TElement>
VOID FList<TElement>::Empty()
{
    TElement *pElement;

    this->Lock();
    while(pElement = this->Pop())
    {
        delete pElement;
    }
    this->Unlock();
}


#endif

