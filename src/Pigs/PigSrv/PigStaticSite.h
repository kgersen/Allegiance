/////////////////////////////////////////////////////////////////////////////
// PigStaticSite.h : Declaration of the CPigsThingSite and CPigStaticSite
// classes.
//

#ifndef __PigStaticSite_h__
#define __PigStaticSite_h__

#include <Allegiance.h>


/////////////////////////////////////////////////////////////////////////////
// Begin: Allegiance Initialization                                        //
                                                                           //
/////////////////////////////////////////////////////////////////////////////
// CPigsThingSite overrides (side visibility).                             //
//                                                                         //
class CPigsThingSite : public ThingSite                                    //
{                                                                          //
public:                                                                    //
  bool GetSideVisibility(IsideIGC* side)                                   //
  {                                                                        //
    return true;                                                           //
  }                                                                        //
};                                                                         //
                                                                           //
                                                                           //
// End: Allegiance Initialization                                          //
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CPigStaticSite
//
class CPigStaticSite : public IIgcSite
{
// IIgcSite Overrides
public:
  virtual TRef<ThingSite> CreateThingSite(ImodelIGC* pModel)
  {
    return new CPigsThingSite();
  }

  virtual TRef<ClusterSite> CreateClusterSite(IclusterIGC* pCluster)
  {
    return new ClusterSite();
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif !__PigStaticSite_h__



