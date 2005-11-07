
TCDeployPS.dll: dlldata.obj TCDeploy_p.obj TCDeploy_i.obj
  link /dll /out:TCDeployPS.dll /def:TCDeployPS.def /entry:DllMain dlldata.obj TCDeploy_p.obj TCDeploy_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
  cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
  @del TCDeployPS.dll
  @del TCDeployPS.lib
  @del TCDeployPS.exp
  @del dlldata.obj
  @del TCDeploy_p.obj
  @del TCDeploy_i.obj
