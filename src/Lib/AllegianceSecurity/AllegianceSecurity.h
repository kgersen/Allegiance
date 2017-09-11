#pragma once

// BT - STEAM

// This shim enables the steam release to include a DRM wrapper check. While it won't do anything for anyone
// who builds the rest of the open source package, it will activate a DRM check for the official Steam releases.
// Please do not remove this file, or remove the DRM check code from the server.

class DrmChecker
{
public:
	DrmChecker();
	~DrmChecker();

	void GetDrmWrapChecksum(int initializingVector, char *szOutput, DWORD nOutputLen);
};


class DrmVerifier
{
public: 
	DrmVerifier();
	~DrmVerifier();

	bool CheckDrm(int initializingVector, char *drm);
};
