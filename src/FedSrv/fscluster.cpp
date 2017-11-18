/*-------------------------------------------------------------------------
 * fscluster.cpp
 * 
 * Implementation of FedSrv class Cluster (aka Sector)
 * 
 * Maker: a-markcu
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"

/*-------------------------------------------------------------------------
 * CFSCluster::CFSCluster()
 *-------------------------------------------------------------------------
 */
CFSCluster::CFSCluster(TRef<IclusterIGC> pCluster) :

  m_pIclusterIGC(pCluster),
  m_pClusterGroups(NULL)
{
  pCluster->SetPrivateData((DWORD)this);  // link to IGC CLuster
}


/*-------------------------------------------------------------------------
 * CFSCluster::~CFSCluster()
 *-------------------------------------------------------------------------
 */
CFSCluster::~CFSCluster()
{
  m_pIclusterIGC->SetPrivateData((DWORD)NULL);  // unlink from IGC CLuster
}