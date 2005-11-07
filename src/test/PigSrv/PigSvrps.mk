
PigSrvps.dll: dlldata.obj PigSrv_p.obj PigSrv_i.obj
	link /dll /out:PigSrvps.dll /def:PigSrvps.def /entry:DllMain dlldata.obj PigSrv_p.obj PigSrv_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del PigSrvps.dll
	@del PigSrvps.lib
	@del PigSrvps.exp
	@del dlldata.obj
	@del PigSrv_p.obj
	@del PigSrv_i.obj
