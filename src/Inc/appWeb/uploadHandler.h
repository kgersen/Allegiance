///
///	@file	uploadHandler.h
///	@brief	Header for the uploadHandler
//	@copy	default
//	
//	Copyright (c) Mbedthis Software LLC, 2003-2005. All Rights Reserved.
//	
//	This software is distributed under commercial and open source licenses.
//	You may use the GPL open source license described below or you may acquire 
//	a commercial license from Mbedthis Software. You agree to be fully bound 
//	by the terms of either license. Consult the LICENSE.TXT distributed with 
//	this software for full details.
//	
//	This software is open source; you can redistribute it and/or modify it 
//	under the terms of the GNU General Public License as published by the 
//	Free Software Foundation; either version 2 of the License, or (at your 
//	option) any later version. See the GNU General Public License for more 
//	details at: http://www.mbedthis.com/downloads/gplLicense.html
//	
//	This program is distributed WITHOUT ANY WARRANTY; without even the 
//	implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//	
//	This GPL license does NOT permit incorporating this software into 
//	proprietary programs. If you are unable to comply with the GPL, you must
//	acquire a commercial license to use this software. Commercial licenses 
//	for this software and support services are available from Mbedthis 
//	Software at http://www.mbedthis.com 
//	
//	@end
//	This module was developed with the assistance of Guntermann & Drunck GmbH
//	Systementwicklung, Germany
////////////////////////////////// Includes ////////////////////////////////////

#ifndef _h_UPLOAD_HANDLER
#define _h_UPLOAD_HANDLER 1

#include	"http.h"

/////////////////////////////// Forward Definitions ////////////////////////////

class MaUploadHandler;
class MaUploadHandlerService;

extern "C" {
	extern int mprUploadInit(void *handle);
};

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// MaUploadModule ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class MaUploadModule:public MaModule {
  private:
  public:
					MaUploadModule(void *handle);
					~MaUploadModule();
};

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// MaUploadHandlerService ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class MaUploadHandlerService:public MaHandlerService {
  private:
	MprList 		handlerHeaders;	 	// List of handler headers
	MprLogModule 	*log;
	MprStr			uploadDir;			// Default upload directory

#if BLD_FEATURE_MULTITHREAD
	MprMutex 		*mutex;
#endif

  public:
					MaUploadHandlerService();
					~MaUploadHandlerService();
	MaHandler 		*newHandler(MaServer * server, MaHost * host, char *ex);
	char			*getUploadDir() { return uploadDir; };
	void			setUploadDir(char *dir);

#if BLD_FEATURE_MULTITHREAD
	inline void 	lock() { mutex->lock(); };
	inline void 	unlock() { mutex->unlock(); };
#else
	inline void 	lock() {};
	inline void 	unlock() {};
#endif
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// UploadHandler ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//	A master instance of the UploadHandler is create for each referencing host.
//	A request instance is cloned from this for each request.
//

#define UPLOAD_BUF_SIZE			4096		// Post data buffer size

//
//	contentState
//	FUTURE -- should not use a MPR prefix. Use MA
//
#define MPR_UPLOAD_REQUEST_HEADER		1	// Request header
#define MPR_UPLOAD_BOUNDARY				2	// Boundary divider
#define MPR_UPLOAD_CONTENT_HEADER		3	// Content part header
#define MPR_UPLOAD_CONTENT_DATA			4	// Content encoded data
#define MPR_UPLOAD_CONTENT_END			5	// End of multipart message

class MaUploadHandler:public MaHandler {
  private:
	char 			*boundary;			// Boundary signature
	int				boundaryLen;		// Length of boundary
	int 			contentState;		// Input states
	MprStr 			fileName;			// File name from request
	MprStr 			filePath;			// Full incoming filename
	int				fileSize;			// Size of uploaded file
	MprStr			nameField;			// Current name keyword value
	MprLogModule 	*log;		 		// Pointer to the service log
	MprBuf 			*postBuf;			// POST data buffer
	MprFile 		*upfile;			// Incoming file object
	MprVar			currentFile;		// Currently uploading file variable
	char 			*uploadDir;			// Upload dir

  public:
					MaUploadHandler(char *ext, MprLogModule * log);
					~MaUploadHandler();
	MaHandler 		*cloneHandler();
	void 			postData(MaRequest * rq, char *buf, int buflen);
	int 			run(MaRequest * rq);
	int 			setup(MaRequest * rq);

	int 			addParameters(MaRequest *rq, char *str, MaEnvType objType);
	char 			*getHostName();
	char 			*getParameter(char *key);

#if BLD_FEATURE_CONFIG_PARSE
	int 			parseConfig(char *key, char *value, MaServer * server, 
						MaHost * host, MaAuth * auth, MaDir * dir, 
						MaLocation * location);
#endif

private:
	int 			processContentBoundary(MaRequest * rq, char *line);
	int 			processContentHeader(MaRequest * rq, char *line);
	int 			processContentData(MaRequest * rq);
};

////////////////////////////////////////////////////////////////////////////////
#endif // _h_UPLOAD__HANDLER

//
// Local variables:
// tab-width: 4
// c-basic-offset: 4
// End:
// vim:tw=78
// vim600: sw=4 ts=4 fdm=marker
// vim<600: sw=4 ts=4
//
