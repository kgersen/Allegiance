#pragma once

#include "igc.h"

class NativeClientClusterSite : public ClusterSite
{
public:
	NativeClientClusterSite(IclusterIGC * pcluster)
		:
		m_pcluster(pcluster)
	{
	}

	virtual void                    AddScanner(SideID   sid, IscannerIGC* scannerNew)
	{
		assert(sid >= 0);
		assert(sid < c_cSidesMax);
		assert(scannerNew);

		AddIbaseIGC((BaseListIGC*)&(m_scanners[sid]), scannerNew);
	}
	virtual void                    DeleteScanner(SideID   sid, IscannerIGC* scannerOld)
	{
		assert(sid >= 0);
		assert(sid < c_cSidesMax);
		assert(scannerOld);

		DeleteIbaseIGC((BaseListIGC*)&(m_scanners[sid]), scannerOld);
	}
	virtual const ScannerListIGC*      GetScanners(SideID   sid) const
	{
		assert(sid >= 0);
		assert(sid < c_cSidesMax);

		return &(m_scanners[sid]);
	}

	virtual void Terminate()
	{
		ClusterSite::Terminate();
	}

private:
	IclusterIGC * m_pcluster;
	ScannerListIGC  m_scanners[c_cSidesMax];
};

