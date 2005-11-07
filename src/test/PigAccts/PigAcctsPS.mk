
PigAcctsps.dll: dlldata.obj PigAccts_p.obj PigAccts_i.obj
	link /dll /out:PigAcctsps.dll /def:PigAcctsps.def /entry:DllMain dlldata.obj PigAccts_p.obj PigAccts_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del PigAcctsps.dll
	@del PigAcctsps.lib
	@del PigAcctsps.exp
	@del dlldata.obj
	@del PigAccts_p.obj
	@del PigAccts_i.obj
