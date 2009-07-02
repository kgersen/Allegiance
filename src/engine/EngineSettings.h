
#ifndef _DX9_ENGINE_SETTINGS_
#define _DX9_ENGINE_SETTINGS_

////////////////////////////////////////////////////////////////////////////////////////////////////
// Class is basically just a data container for storing additional engine settings
// for use in the DX9 version of the game.
// Could write accessors etc. but doesn't seem a lot of point.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CDX9EngineSettings
{
public:
	CDX9EngineSettings()
	{
		mbUseTexturePackFiles = false;
		m_refreshrate = -1; //imago added ;-/
	}
	bool			mbUseTexturePackFiles;
	int				m_refreshrate;
};

extern CDX9EngineSettings g_DX9Settings;

#endif // _DX9_ENGINE_SETTINGS_
