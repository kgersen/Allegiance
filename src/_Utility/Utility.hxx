/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	utility.hxx
**
**  Author: 
**
**  Description:
**      Source for templated methods in the llo library. See llo.h.
**
**  History:
*/

/*
** Definitions for the Slist_utl class:
*/
template<class T, class Sink>       void               Slist_utl<T, Sink>::purge(bool deleteF)
{
    //Note ... do the test for deleteF outside the loop for speed reasons
    if (deleteF)
    {
        //Nuking an item updates the list's first pointer,
        //so this works even if it looks funny.
        Slink_utl<T, Sink>*   f;
        while (f = first()) //not ==
        {
            delete f;
        }
    }
    else
    {
        //ditto for unlinking.
        Slink_utl<T, Sink>*   f;
        while (f = first()) //not ==
        {
            f->unlink();
        }
    }

    sink();
}

template<class T, class Sink>       bool               Slist_utl<T, Sink>::first(const T& t)
{
    Slink_utl<T,Sink>*   l = new Slink_utl<T,Sink>(t);
    if (l)
    {
        first(l);
        return true;
    }
    else
        return false;
}
template<class T, class Sink>       bool               Slist_utl<T, Sink>::last(const T& t)
{
    Slink_utl<T,Sink>*   l = new Slink_utl<T,Sink>(t);
    if (l)
    {
        last(l);
        return true;
    }
    else
        return false;
}

/*
** Definitions for the Slink_utl class:
*/
template<class T, class Sink>       bool            Slink_utl<T, Sink>::next(const T& t)
{
    if (m_list)
        return next(new Slink_utl<T,Sink>(t));
    else
        return false;
}

template<class T, class Sink>       bool            Slink_utl<T, Sink>::txen(const T& t)
{
    if (m_list)
        return txen(new Slink_utl<T,Sink>(t));
    else
        return false;
}

/*
** Definitions for the Mlist_utl class:
*/
template<class T>       void               Mlist_utl<T>::purge(bool deleteF)
{
    lock();
    //Note ... do the test for deleteF outside the loop for speed reasons
    if (deleteF)
    {
        //Nuking an item updates the list's first pointer,
        //so this works even if it looks funny.
        Mlink_utl<T>*   f;
        while (f = first()) //not ==
        {
            delete f;
        }
    }
    else
    {
        //ditto for unlinking.
        Mlink_utl<T>*   f;
        while (f = first()) //not ==
        {
            f->unlink();
        }
    }
    unlock();
}

template<class T>       bool               Mlist_utl<T>::first(const T& t)
{
    Mlink_utl<T>*   l = new Mlink_utl<T>(t);
    if (l)
    {
        first(l);
        return true;
    }
    else
        return false;
}
template<class T>       bool               Mlist_utl<T>::last(const T& t)
{
    Mlink_utl<T>*   l = new Mlink_utl<T>(t);
    if (l)
    {
        last(l);
        return true;
    }
    else
        return false;
}

template<class T>       void               Mlist_utl<T>::first(Mlink_utl<T>*   l)
{
    lock();
    List_utl::first(l);
    unlock();
}
template<class T>       Mlink_utl<T>*      Mlist_utl<T>::first(void) const
{
    lock();
    Mlink_utl<T>*   l = (Mlink_utl<T>*)List_utl::first();
    unlock();

    return l;
}

template<class T>       void               Mlist_utl<T>::last(Mlink_utl<T>*   l)
{
    lock();
    List_utl::last(l);
    unlock();
}
template<class T>       Mlink_utl<T>*      Mlist_utl<T>::last(void) const
{
    lock();
    Mlink_utl<T>*   l = (Mlink_utl<T>*)List_utl::last();
    unlock();

    return l;
}

template<class T>       Mlink_utl<T>*      Mlist_utl<T>::operator [] (int index) const
{
    lock();
    Mlink_utl<T>*   l = (Mlink_utl<T>*)List_utl::operator [](index);
    unlock();

    return l;
}

/*
** Definitions for the Mlink_utl class:
*/
template<class T> inline    bool            Mlink_utl<T>::next(const T& t)
{
    if (m_list)
        return next(new Mlink_utl<T>(t));
    else
        return false;
}

template<class T> inline    bool            Mlink_utl<T>::txen(const T& t)
{
    if (m_list)
        return txen(new Mlink_utl<T>(t));
    else
        return false;
}
