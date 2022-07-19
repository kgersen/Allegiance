#ifndef _BADWORDS_H_
#define _BADWORDS_H_

//------------------------------------------------------------------------------
// functions to handle bad words
//------------------------------------------------------------------------------
void    LoadBadWords (void);
ZString CensorBadWords (const ZString& string);
void    ToggleCensorDisplay (void);
bool    CensorDisplay (void);

//------------------------------------------------------------------------------

#endif  //  _BADWORDS_H_