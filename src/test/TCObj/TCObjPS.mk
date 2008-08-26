
TCObjPS.dll: dlldata.obj TCObj_p.obj TCObj_i.obj
  link /dll /out:TCObjPS.dll /def:TCObjPS.def /entry:DllMain dlldata.obj TCObj_p.obj TCObj_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
  cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
  @del TCObjPS.dll
  @del TCObjPS.lib
  @del TCObjPS.exp
  @del dlldata.obj
  @del TCObj_p.obj
  @del TCObj_i.obj
