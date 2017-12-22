#ifndef __DevStudio_h__
#define __DevStudio_h__

#define __APPAUTO_H_
#define __APPDEFS_H__
#define __APPGUID_H__
#define __BLDAUTO_H__
#define __BLDDEFS_H__
#define __BLDGUID_H__
#define __DBGAUTO_H__
#define __DBGDEFS_H
#define __DBGGUID_H__
#define __TEXTAUTO_H__
#define __TEXTDEFS_H__
#define __TEXTGUID_H__

#import <..\..\..\SharedIDE\bin\devshl.dll> no_namespace named_guids

#import <..\..\..\SharedIDE\bin\ide\devbld.pkg> no_namespace named_guids    \
  exclude("DsWindowState","DsArrangeStyle","DsButtonType","DsSaveChanges",  \
  "DsSaveStatus","IGenericDocument","IGenericWindow","IGenericProject")

#import <..\..\..\SharedIDE\bin\ide\devdbg.pkg> no_namespace named_guids

#import <..\..\..\SharedIDE\bin\devedit.pkg> no_namespace named_guids       \
  exclude("DsWindowState","DsArrangeStyle","DsButtonType","DsSaveChanges",  \
  "DsSaveStatus","IGenericDocument","IGenericWindow","IGenericProject")

#endif // !__DevStudio_h__
