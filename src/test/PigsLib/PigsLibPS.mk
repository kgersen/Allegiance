
PigsLibPS.dll: dlldata.obj PigsLib_p.obj PigsLib_i.obj
  link /dll /out:PigsLibPS.dll /def:PigsLibPS.def /entry:DllMain dlldata.obj PigsLib_p.obj PigsLib_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
  cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
  @del PigsLibPS.dll
  @del PigsLibPS.lib
  @del PigsLibPS.exp
  @del dlldata.obj
  @del PigsLib_p.obj
  @del PigsLib_i.obj
