#include "pch.h"
#include "trekctrls.h"
#include "loadout.h"
#include "limits.h"
#include "trekmdl.h"

//////////////////////////////////////////////////////////////////////////////
//
// PartListItem
//
//////////////////////////////////////////////////////////////////////////////

#define shipIDLoadout SHRT_MAX  //review: NA doesn't work here

class PartListItem : public ListItem
{
private:
    long                m_lData;
    TRef<IbucketIGC>    m_pBucket;
    TRef<IbuyableIGC>   m_pBuyable;
    Money               m_moneyCost;
    const char*         m_szModel;
    TRef<Image>         m_pImagePart;
    TRef<Image>         m_pImageBkgnd;
    TRef<Image>         m_pImageSelect;
    TRef<Image>         m_pImageBrackets;
    
public:
    
    IbuyableIGC* GetBuyable()
    { return m_pBuyable;}
    
    PartListItem(IbucketIGC* pBucket)
    {
        m_lData = (long)pBucket;
        m_pBucket = pBucket;
        m_pBuyable = pBucket;
        m_moneyCost = m_pBuyable->GetPrice();
        Construct();
    }
    
    void Construct()
    {
        assert(m_pBuyable);
        m_szModel = m_pBuyable->GetModelName();
        
        m_pImageBrackets = GetModeler()->LoadImage("conshadebracketsbmp", true);
        m_pImageBkgnd = GetModeler()->LoadImage(AWF_LOADOUT_ITEM, true);
        m_pImageSelect = GetModeler()->LoadImage("investhighlightbmp", true);
        m_pImagePart = GetModeler()->LoadImage(
            (ZString("i") + ZString(m_szModel) + "bmp").ToLower(), 
            false
            );
        
        
    }
    
    static int CountDronesOfType(IdroneTypeIGC* pDroneType)
    {
        DroneTypeID dtid = pDroneType->GetObjectID();
        SideID      sid = trekClient.GetSideID();
        
        int nCount = 0;
        for (PlayerLink*    ppl = trekClient.GetPlayerList()->first();
        (ppl != NULL);
        ppl = ppl->next())
        {
            //We only know about drones in our mission so only check the side
            const PlayerInfo& pi = ppl->data();
            
            if ((pi.GetDroneTypeID() == dtid) && (pi.SideID() == sid))
            {
                nCount++;
            }
            
        }
        
        return nCount;
    }
    
    static int CountStationsOfType(IstationTypeIGC* pStationtypeTest)
    {
        IsideIGC* pSide =  trekClient.GetSide();
        const StationListIGC* stationList = pSide->GetStations();
        StationClassID classID = pStationtypeTest->GetClassID();
        int nCount = 0;
        
        for (StationLinkIGC* stationLink = (stationList ? stationList->first() : NULL); 
        stationLink; stationLink=stationLink->next())
        {
            IstationTypeIGC* pstationtype = stationLink->data()->GetBaseStationType();
            
            // if this station is in the same class of station
            if (pstationtype->GetClassID() == classID)
                
                nCount++;
        }
        
        return nCount;
    }
    
    static bool BuildSpotFoundForStationType(IstationTypeIGC* pStationtype)
    {
        AsteroidAbilityBitMask aabm = pStationtype->GetBuildAABM();
        
        for (AsteroidLinkIGC* asteriodLink = trekClient.GetCore()->GetAsteroids()->first(); 
        asteriodLink != NULL; asteriodLink = asteriodLink->next())
        {
            if (asteriodLink->data()->HasCapability(aabm))
                return true;
        }
        
        return false;
    }
    
    static IshipIGC* FindBuilderForStationType(IstationTypeIGC* pStationtype)
    {
        if (trekClient.GetSide())
        {
            for (ShipLinkIGC* shipLink = trekClient.GetSide()->GetShips()->first(); 
            shipLink != NULL; shipLink = shipLink->next())
            {
                if (shipLink->data()->GetBaseData() == pStationtype)
                    return shipLink->data();
            }
        }
        
        return NULL;
    }
    
    ZString GetBuildingStatusText()
    {
        IstationTypeIGC* pStationtype;
        CastTo(pStationtype, m_pBucket->GetBuyable());
        IshipIGC* pshipBuilder = FindBuilderForStationType(pStationtype);
        
        if (m_pBucket->GetPercentBought() >= 100)
        {
            return "Preparing builder ...";
        }
        if (pshipBuilder)
        {
            if (pshipBuilder->GetCommandTarget(c_cmdAccepted) &&
                (pshipBuilder->GetCommandID(c_cmdAccepted) == c_cidBuild))
            {
                return "Constructor en route ...";
            }
            else
            {
                return "Constructor waiting ...";
            }
        }
        else if (BuildSpotFoundForStationType(pStationtype))
        {
            return "";
        }
        else
        {
            return ZString("no ") 
                + IasteroidIGC::GetTypeName(pStationtype->GetBuildAABM()) 
                + " asteroid available";
        }
    }
    
    void DrawText(Surface* pSurface, const WinRect& rect)
    {
        Color colorShadow(0, 0, 33.0f / 256.0f);

        char szPrice[20];
        sprintf(szPrice, "$ %d", m_moneyCost);

        pSurface->DrawStringWithShadow(
            TrekResources::SmallFont(),
            Color::White(),
            colorShadow,
            rect.Min() + WinPoint(147,1), 
            szPrice
        );
        
        ZString strName = m_pBuyable->GetName();
        
        if (m_pBucket)
        {
            ObjectType  otBucket = m_pBucket->GetBucketType();
            if (otBucket != OT_development)
            {
                int nTotalNumber;
                switch (otBucket)
                {
                    case OT_stationType:
                    {
                        pSurface->DrawStringWithShadow(
                            TrekResources::SmallFont(),
                            Color::White(),
                            colorShadow,
                            rect.Min() + WinPoint(11,42), 
                            GetBuildingStatusText()
                        );
                
                        IstationTypeIGC* pStationtype;
                        CastTo(pStationtype, m_pBucket->GetBuyable());
                        nTotalNumber = CountStationsOfType(pStationtype);
                        strName = "Build " + strName + " (" + ZString(nTotalNumber) + ")";
                    }
                    break;

                    case OT_droneType:
                    {
                        IdroneTypeIGC* pDronetype;
                        int nMaxMiners = trekClient.MyMission()->GetMissionDef().misparms.nMaxMinersPerTeam;
                        CastTo(pDronetype, m_pBucket->GetBuyable());
                        nTotalNumber = CountDronesOfType(pDronetype);

                        if (nTotalNumber >= nMaxMiners){
                            pSurface->DrawStringWithShadow(
                                TrekResources::SmallFont(),
                                Color::White(),
                                colorShadow,
                                rect.Min() + WinPoint(11,42), 
                                "At maximum " + strName + "s"
                           );
                        }

                        strName = "Build " + strName + " (" + ZString(nTotalNumber) 
                            + "/" + ZString(nMaxMiners) + ")";
                    }
                    break;

                    case OT_partType:
                    case OT_hullType:
                    {
                        nTotalNumber = m_pBucket->GetSide()->GetStockpile(m_pBucket->GetBuyable());
                        strName = "Build " + strName + " (" + ZString(nTotalNumber) + ")";
                    }
                }
            }
        }
        pSurface->DrawStringWithShadow(
            TrekResources::SmallFont(),
            Color::White(),
            colorShadow,
            rect.Min() + WinPoint(11,1), 
            strName
        );
    }
    
    void FillClippedRect(Context* pcontext, const WinRect& rect, const WinRect& rectClip, const Color& color)
    {
        WinRect rectClipped = rect;

        rectClipped.Intersect(rectClip);

        if (!rectClipped.IsEmpty()) {
            pcontext->FillRect(Rect::Cast(rect), color);
        }
    }

    void DrawShade(Surface* pSurface, const WinRect& rect)
    {
        int nWidth = (int)(rect.XSize() - 23);
        int nPaidShadeWidth = (int)(nWidth * m_pBucket->GetPercentBought()/100);
        int nCompletedShadeWidth = (int)(nWidth * m_pBucket->GetPercentComplete()/100);
        
        if (nPaidShadeWidth > nWidth)
            nPaidShadeWidth  = nWidth;

        if (nCompletedShadeWidth > nWidth)
            nCompletedShadeWidth  = nWidth;

        pSurface->BitBlt(rect.Min(), m_pImageBrackets->GetSurface());

        Point offset = Point::Cast(pSurface->GetOffset() + rect.Min() + WinPoint(8, 2));
        const WinPoint& size = pSurface->GetSize();

        WinRect rectClip = pSurface->GetClipRect();
        rectClip.Offset(pSurface->GetOffset());
        rectClip =
            WinRect(
                rectClip.XMin(),
                size.Y() - rectClip.YMax(),
                rectClip.XMax(),
                size.Y() - rectClip.YMin()
            );

        TRef<Context> pcontext = pSurface->GetContext();

        if (pcontext) {
            pcontext->SetBlendMode(BlendModeSourceAlpha);

            float bright = 0.5f;

            FillClippedRect(
                pcontext,
                WinRect(
                    offset.X(),
                    size.Y() - (offset.Y() + rect.YSize()-13),
                    offset.X() + nCompletedShadeWidth,
                    size.Y() - offset.Y()
                ),
                rectClip,
                Color(0.0, bright, 0.0, bright)
            );

            Point offsetPaid = offset + Point(nCompletedShadeWidth, 0);
            FillClippedRect(
                pcontext,
                WinRect(
                    offsetPaid.X(),
                    size.Y() - (offsetPaid.Y() + rect.YSize()-13),
                    offsetPaid.X() + nPaidShadeWidth - nCompletedShadeWidth,
                    size.Y() - offsetPaid.Y()
                ),
                rectClip,
                Color(bright, bright, 0.0, bright)
            );

            pSurface->ReleaseContext(pcontext);
        }
    }
    
    short GetItemHeight()
    {
        return 1;
    }
    
    long GetItemData()
    {
        return m_lData;
    };
    
    bool Update()
    {
        return true;
    }
    
    void SetSortOrder(long lSortOrder)
    {
    }
    
    bool SetFilter(long lFilter)
    {
        return true;
    }
    
    void DrawItem(Surface* pSurface, const WinRect& rect, bool fSelected, int iFirstSlot)
    {   if (m_pImagePart) {
            pSurface->BitBlt(rect.Min() + WinPoint(10, 2), m_pImagePart->GetSurface());
        } else {
            pSurface->BitBlt(rect.Min(), m_pImageBkgnd->GetSurface());
        }
        
        if (fSelected)
        {
            pSurface->BitBlt(rect.Min() + WinPoint(10, 2), m_pImageSelect->GetSurface());
            
            /*Point offset = Point::Cast(pSurface->GetOffset() + rect.Min() + WinPoint(8, 2));
            const WinPoint& size = pSurface->GetSize();
            TRef<Context> pcontext = pSurface->GetContext();

            pcontext->SetBlendMode(BlendModeSourceAlpha);

            pcontext->SetColor(Color(1.0, 0.0, 0.0, 0.5));
            pcontext->FillRect(
                Rect(
                    offset.X(),
                    size.Y() - (offset.Y() + rect.YSize()-13),
                    offset.X() + rect.XSize() - 23,
                    size.Y() - offset.Y()
                )
            );

            pSurface->ReleaseContext(pcontext);
            */
        }       

        if (m_pBucket)
            DrawShade(pSurface, rect);
        
        DrawText(pSurface, rect);
        
    }
    
};

//////////////////////////////////////////////////////////////////////////////
//
// PurchasesPane
//
//////////////////////////////////////////////////////////////////////////////

class PurchasesPaneImpl :   public PurchasesPane, 
public EventTargetContainer<PurchasesPaneImpl>, 
public TrekClientEventSink
{
private:
    //TRef<TabPane>       m_pTabPaneTeamPurchases;
    TRef<ListPaneOld>   m_pListPane;
    int                 m_nTabSel;
    TRef<PartInfoPane>  m_pPartInfoPane;
    int                 m_nLastHoverIndex;
    TRef<IstationIGC>   m_pStation;
    TRef<ButtonPane>    m_pButtonInvest;
    TRef<ButtonPane>    m_pButtonTab0;
    TRef<ButtonPane>    m_pButtonTab1;
    TRef<ButtonPane>    m_pButtonTab2;
    TRef<ButtonPane>    m_pButtonTab3;
    TRef<ButtonPane>    m_pButtonTab4;
    TRef<ButtonPane>    m_pButtonTab5;
    TRef<ButtonPane>    m_pButtonClose;
    Window*             m_pPaneWindow;
    TRef<Pane>          m_pBlankPane;
    TRef<Pane>          m_pBlankPane0;
    TRef<Pane>          m_pBlankPane1;
    TRef<Pane>          m_pBlankPane2;
    TRef<Pane>          m_pBlankPane3;
    TRef<Pane>          m_pBlankPane4;
    TRef<Pane>          m_pBlankPane5;
    
    
public:
    PurchasesPaneImpl(PartInfoPane* pPartInfoPane) :
          m_nTabSel(-1),
          m_nLastHoverIndex(-1),
          m_pPartInfoPane(pPartInfoPane),
          m_pStation(NULL),
          m_pPaneWindow(NULL)
      {
          TRef<ImagePane> pImagePane = new ImagePane(GetModeler()->LoadImage(
              AWF_LOADOUT_FLIGHT_PURCHASE_PANEL, false));
          InsertAtBottom(pImagePane);
          
          // main loadout column pane
          TRef<ColumnPane> pColumnPane = new ColumnPane();
          TRef<RowPane> pRowPane = new RowPane();
          

          const int nTabWidth = 180;

          m_pBlankPane5 = new Pane();
          InternalSetSize(m_pBlankPane5, WinPoint(nTabWidth, 3));
          pColumnPane->InsertAtBottom(m_pBlankPane5);

          m_pButtonTab0 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btndronesbmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab0);
          //m_pBlankPane0 = new Pane();
          //InternalSetSize(m_pBlankPane0, WinPoint(nTabWidth, 1));
          //pColumnPane->InsertAtBottom(m_pBlankPane0);

          m_pButtonTab1 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnstarbasebmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab1);
          m_pBlankPane1 = new Pane();
          InternalSetSize(m_pBlankPane1, WinPoint(nTabWidth, 1));
          pColumnPane->InsertAtBottom(m_pBlankPane1);

          m_pButtonTab2 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnordnancebmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab2);
          m_pBlankPane2 = new Pane();
          InternalSetSize(m_pBlankPane2, WinPoint(nTabWidth, 1));
          pColumnPane->InsertAtBottom(m_pBlankPane2);

          m_pButtonTab3 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnelectronicsbmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab3);
          m_pBlankPane3 = new Pane();
          InternalSetSize(m_pBlankPane3, WinPoint(nTabWidth, 1));
          pColumnPane->InsertAtBottom(m_pBlankPane3);

          m_pButtonTab4 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnresearchbmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab4);
          m_pBlankPane4 = new Pane();
          InternalSetSize(m_pBlankPane4, WinPoint(nTabWidth, 1));
          pColumnPane->InsertAtBottom(m_pBlankPane4);

          m_pButtonTab5 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnshipyardbmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab5);
              

          pRowPane->InsertAtBottom(pColumnPane);
          
          m_pBlankPane = new Pane();
          InternalSetSize(m_pBlankPane, WinPoint(30, 10));
          pRowPane->InsertAtBottom(m_pBlankPane);

          // close button
          m_pButtonClose = 
              CreateTrekButton(
                  CreateButtonFacePane(
                      GetModeler()->LoadSurface("btnclosepanebmp", true),
                      ButtonNormal
                  ),
                  false
              );

          // parts list        
          m_pListPane = ListPaneOld::Create(WinPoint(203, 432), 72, true, m_pButtonClose),
              pRowPane->InsertAtBottom(m_pListPane);
          
          // invest button
          m_pButtonInvest = 
              CreateTrekButton(
                  CreateButtonFacePane(
                      GetModeler()->LoadSurface(AWF_LOADOUT_INVEST_BUTTON, true),
                      ButtonNormal
                  ),
                  false
              );
          
          pImagePane->InsertAtBottom(m_pButtonInvest);
          m_pButtonInvest->SetOffset(WinPoint(63,315));
            
          pImagePane->InsertAtBottom(pRowPane);
          pRowPane->SetOffset(WinPoint(17,9));
          
          //initialize tab selection to Starbase Tab
          m_pButtonTab0->SetChecked(true);
          OnTabSelect0();
          

          AddEventTarget(OnTabSelect0, m_pButtonTab0->GetEventSource());
          AddEventTarget(OnTabSelect1, m_pButtonTab1->GetEventSource());
          AddEventTarget(OnTabSelect2, m_pButtonTab2->GetEventSource());
          AddEventTarget(OnTabSelect3, m_pButtonTab3->GetEventSource());
          AddEventTarget(OnTabSelect4, m_pButtonTab4->GetEventSource());
          AddEventTarget(OnTabSelect5, m_pButtonTab5->GetEventSource());


          //AddEventTarget(OnTabSelect, m_pTabPaneTeamPurchases->GetEventSource());


          AddEventTarget(OnListSelect, m_pListPane->GetEventSource());
          AddEventTarget(OnInvest, m_pListPane->GetDoubleClickEventSource());
          AddEventTarget(OnPartial, m_pListPane->GetRightClickEventSource());	// TE: hooked up RightClickEvent to OnPartial method
          AddEventTarget(OnListHover, m_pListPane->GetMouseOverEvent());
          AddEventTarget(OnInvest, m_pButtonInvest->GetEventSource());
          AddEventTarget(OnButtonBack, m_pButtonClose->GetEventSource());
          AddEventTarget(OnMouseEnterInvestButton, m_pButtonInvest->GetMouseEnterEventSource());
          AddEventTarget(OnMouseLeaveInvestButton, m_pButtonInvest->GetMouseLeaveEventSource());
          
          //m_pTabPaneTeamPurchases->ShowSelPane();
      }
      
      ~PurchasesPaneImpl()
      {
          GetWindow()->SetInvestAsteroid(0);
      }
      
      bool OnInvest()
      {
          if (trekClient.MyMissionInProgress())
          {
              TRef<ListItem> pItem = m_pListPane->GetSelItem();
              if (pItem)
              {
                  IbucketIGC* pBucket =  (IbucketIGC*)m_pListPane->GetSelItem()->GetItemData();

                  if (CanInvest(pBucket))
                  {
                      Money bucketCost = pBucket->GetPrice() - pBucket->GetMoney();

                      Money investmentAmount = min(bucketCost, trekClient.GetMoney());
                      trekClient.AddMoneyToBucket(pBucket, investmentAmount);
                  }
              }                    
          }
          return true;
      }
      
	  // TE: Added OnPartial method to handle partial-investing
      bool OnPartial()
      {
          if (trekClient.MyMissionInProgress())
          {
              TRef<ListItem> pItem = m_pListPane->GetSelItem();
              if (pItem)
              {
                  IbucketIGC* pBucket =  (IbucketIGC*)m_pListPane->GetSelItem()->GetItemData();

                  if (CanInvest(pBucket))
                  {
                      Money bucketCost = pBucket->GetPrice() - pBucket->GetMoney();	// Get required money to finish development
					  
					  // Prevent finishing the development with the right-click
					  if (bucketCost == 1)
						  return true;

                      Money TenPercent = pBucket->GetPrice() * 0.1;					// Get 10% of the total research cost

					  Money investmentAmount = min(TenPercent, trekClient.GetMoney());	// Get what's less: Your money? or the reqdevcost

					  // If we're about to finish this development, remove 1cr so we don't!!
					  while (pBucket->GetMoney() + investmentAmount >= pBucket->GetPrice())
						  investmentAmount -= 1;

					  // Perform the investment
					  trekClient.AddMoneyToBucket(pBucket, investmentAmount);
                  }
              }                    
          }
          return true;
     }

     bool OnButtonBack()
     {
         
        GetWindow()->TurnOffOverlayFlags(ofInvestment);
        return true;
     }
      
      void OnBucketChange(BucketChange bc, IbucketIGC* b)
      {
          if (bc == c_bcTerminated)
              m_pListPane->RemoveItemByData((long)b);
          else
              m_pListPane->UpdateItemByData((long)b);
          
          UpdateSpendButtons();
      }
      
      void OnModelTerminated(ImodelIGC* pmodel) 
      {
          // make sure we correct the number of existing stations on the investment list
          if ((pmodel->GetObjectType() == OT_station) && (pmodel->GetSide() != NULL)
              && (pmodel->GetSide()->GetObjectID() == trekClient.GetSideID()))
          {   
              m_pListPane->UpdateAll();
              
              UpdateSpendButtons();
          }
      }
      
      void OnStationCaptured(StationID stationID, SideID sideID) 
      {
          // this may change our station count
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      void OnDiscoveredStation(IstationIGC* pstation)
      {
          // this may change our station count
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      void OnDiscoveredAsteroid(IasteroidIGC* pasteroid)
      {
          // this may change our station status text
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      
      void UpdateTechList()
      {
          // Update the selected tech list
          // HACK: This is currently O(n^2), but it seems to work well enough.  
          int nInsertionIndex = 0;
          BuyableGroupID group = (BuyableGroupID)m_nTabSel;
          
          IsideIGC* pSide = trekClient.GetSide();
          if (pSide)
          {
              const BucketListIGC* pList = pSide->GetBuckets();
              for (BucketLinkIGC* pBucketNode = pList->first();
              pBucketNode != NULL; pBucketNode = pBucketNode->next())
              {
                  IbucketIGC*   pBucket  = pBucketNode->data();

                  if (pBucket->GetGroupID() == group)
                  {
                      if (pSide->CanBuy(pBucket))
                      {
                          IbucketIGC*   pbucketPredecessor = pBucket->GetPredecessor();
                          if ((pbucketPredecessor == NULL) || pbucketPredecessor->GetCompleteF())
                          {
                              int nExistingIndex = m_pListPane->GetItemIdxByData((long)pBucket);
                          
                              if (nExistingIndex == NA)
                                  m_pListPane->InsertItem(nInsertionIndex++, new PartListItem(pBucket));
                              else
                                  nInsertionIndex = nExistingIndex + 1;
                          }
                      }
                      else
                      {
                          m_pListPane->RemoveItemByData((long)pBucket);
                      }
                  }
              }
          }

          m_pButtonTab0->SetEnabled(BuyableTechExistsInGroup(0));
          m_pButtonTab1->SetEnabled(BuyableTechExistsInGroup(1));
          m_pButtonTab2->SetEnabled(BuyableTechExistsInGroup(2));
          m_pButtonTab3->SetEnabled(BuyableTechExistsInGroup(3));
          m_pButtonTab4->SetEnabled(BuyableTechExistsInGroup(4));
          m_pButtonTab5->SetEnabled(BuyableTechExistsInGroup(5));

          UpdateSpendButtons();
      }

      bool BuyableTechExistsInGroup(BuyableGroupID group)
      {
          IsideIGC* pSide = trekClient.GetSide();
          if (pSide)
          {
              const BucketListIGC* pList = pSide->GetBuckets();
              for (BucketLinkIGC* pBucketNode = pList->first();
              pBucketNode != NULL; pBucketNode = pBucketNode->next())
              {
                  IbucketIGC*  pBucket  = pBucketNode->data();
                  if (pBucket->GetGroupID() == group)
                  {
                      if (pSide->CanBuy(pBucket))
                      {
                          IbucketIGC*   pbucketPredecessor = pBucket->GetPredecessor();
                          if ((pbucketPredecessor == NULL) || pbucketPredecessor->GetCompleteF())
                          {
                              return true;
                          }
                      }
                  }
              }
          }

          return false;
      }
      
      void OnTechTreeChanged(SideID sid)
      {
          if (trekClient.GetSideID() == sid)
          {
              UpdateTechList();
          }
      }
      
      bool CanInvest(IbucketIGC* pBucket)
      {
          // if the game has not started yet, don't let them invest
          if (!trekClient.MyMissionInProgress())
              return false;

          if (!trekClient.GetSide())
              return false;

          // if this is a drone, make sure that we have less than 
          // 4 of that type of drone.
          if (pBucket->GetBucketType() == OT_droneType)
          {
              IdroneTypeIGC* pDronetype; 
              CastTo(pDronetype, pBucket->GetBuyable());
              if (PartListItem::CountDronesOfType(pDronetype) 
                  >= trekClient.MyMission()->GetMissionDef().misparms.nMaxMinersPerTeam)
              {
                  return false;
              }
          }
          
          // if this is a station, make sure we have the 
          // appropriate rock and don't have a builder.
          if (pBucket->GetBucketType() == OT_stationType)
          {
              IstationTypeIGC* pStationtype; 
              CastTo(pStationtype, pBucket->GetBuyable());
              if (!PartListItem::BuildSpotFoundForStationType(pStationtype)
                  || PartListItem::FindBuilderForStationType(pStationtype) != NULL)
                  return false;
          }

          // they need to have money to invest
          if (trekClient.GetMoney() <= 0)
              return false;

          // there needs to be something in which to invest
          if (pBucket->GetPercentBought() >= 100)
              return false;

          return true;
      }

      void UpdateSpendButtons()
      {
          TRef<ListItem> pItem = m_pListPane->GetSelItem();
          
          if (pItem) 
          {
              IbucketIGC* pBucket = (IbucketIGC*)(pItem->GetItemData());
              
              // if the mission has started, the player has money to invest, and the investment
              // is not at 100%...
              m_pButtonInvest->SetEnabled(CanInvest(pBucket));
          } 
          else 
          {
              // no item to buy or invest in
              m_pButtonInvest->SetEnabled(false);
          }
      }
      
      void OnMoneyChange(PlayerInfo* pPlayerInfo)
      {
          UpdateSpendButtons();
      }
      
      bool OnListHover(int nSel)
      {
          if (nSel == -1)
          {
              m_pPartInfoPane->DisplayNothing();
          }
          else
          {
              IbucketIGC* pBucket =  (IbucketIGC*)m_pListPane->GetItemByIdx(nSel)->GetItemData();
              if (m_nLastHoverIndex != nSel)
                  trekClient.PlaySoundEffect(mouseoverSound);
              m_pPartInfoPane->DisplayTeamPurchase(pBucket, false);
          }
          
          m_nLastHoverIndex = nSel;
          
          return true;
      }
      
      bool OnMouseEnterInvestButton()
      {
          OnListHover(m_pListPane->GetSelItemIdx());
          return true;
      }
      
      bool OnMouseLeaveInvestButton()
      {
          OnListHover(-1);
          return true;
      }
      
      bool OnListSelect(int nSel)
      {
          UpdateSpendButtons();
          return true;
      }
      
      void OnAddPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo)
      {
          // update the drone counts
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      void OnDelPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, QuitSideReason reason, const char* szMessageParam)
      {
          // update the drone counts
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      bool OnTabSelect0()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 0;
          m_pButtonTab0->SetChecked(true);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }
      
      bool OnTabSelect1()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 1;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(true);
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }

      bool OnTabSelect2()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 2;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(true);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }

      bool OnTabSelect3()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 3;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(true);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }

      bool OnTabSelect4()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 4;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(true);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }

      bool OnTabSelect5()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 5;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(true);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }
      
      void UpdateLayout()
      {
          DefaultUpdateLayout();
      }
      
      void SetPaneWindow(Window* pWindow)
      {
          m_pPaneWindow = pWindow;
      }
};

TRef<PurchasesPane> PurchasesPane::Create(PartInfoPane* pPartInfoPane)
{
    return new PurchasesPaneImpl(pPartInfoPane);
}

TRef<IObject> PurchasesPaneFactory::Apply(ObjectStack& stack)
{
    TRef<PartInfoPane> pinfoPane; CastTo(pinfoPane, (Pane*)(IObject*)stack.Pop());
    return (Pane*)PurchasesPane::Create(pinfoPane);
}

//////////////////////////////////////////////////////////////////////////////
//
// PartInfoPane
//
//////////////////////////////////////////////////////////////////////////////

static WinPoint s_rgPtLabels[] =
{
    WinPoint(9,25), WinPoint(9,42), WinPoint(9,59),
        WinPoint(9,76), WinPoint(9,92),
        WinPoint(120,25), WinPoint(120,42), WinPoint(120,59),
        WinPoint(120,76), WinPoint(120,92),
};

static WinPoint s_rgPtStats[] =
{
    WinPoint(67,25), WinPoint(67,42), WinPoint(67,59),
        WinPoint(67,76), WinPoint(67,92),
        WinPoint(178,25), WinPoint(178,42), WinPoint(178,59),
        WinPoint(178,76), WinPoint(178,92),
};



class PartInfoPaneImpl : public PartInfoPane
{
private:
    TRef<IbuyableIGC>   m_pBuyable;
    TRef<IhullTypeIGC>  m_pHullType;
    TRef<IpartTypeIGC>  m_pPartType;
    Money               m_moneyCost;
    const char*         m_szModel;
    TRef<Image>         m_pImagePart;
    TRef<Image>         m_pImageBkgnd;
    TRef<ConnectorPane> m_pConnectorPaneInventory;
    TRef<ConnectorPane> m_pConnectorPanePurchases;
    
public:
    
    PartInfoPaneImpl(ConnectorPane* pConnectorPanePurchases, ConnectorPane* pConnectorPaneInventory) :
      m_pConnectorPaneInventory(pConnectorPaneInventory), 
          m_pConnectorPanePurchases(pConnectorPanePurchases)
      {
          m_pImageBkgnd = GetModeler()->LoadImage(AWF_LOADOUT_INFO_PANEL, false);
          DisplayNothing();
      }
      
      void DisplayTeamPurchase(IbucketIGC* pBucket, bool bPartInInventory)
      {
          if (m_pConnectorPaneInventory)
              m_pConnectorPaneInventory->SetVisible(bPartInInventory);
          
          if (m_pConnectorPanePurchases)
              m_pConnectorPanePurchases->SetVisible(!bPartInInventory);
          
          m_pPartType = NULL;
          m_pHullType = NULL;
          m_pBuyable = pBucket;
          
          InternalSetSize(m_pImageBkgnd->GetSurface()->GetSize());
          
          NeedPaint();

          GetWindow()->SetInvestAsteroid(0);
          IbuyableIGC*    pbuyable = pBucket->GetBuyable();
          if (pbuyable->GetObjectType() == OT_stationType)
          {
              IstationTypeIGC*    pst = (IstationTypeIGC*)pbuyable;

              AsteroidAbilityBitMask  aabm = pst->GetBuildAABM();
              GetWindow()->SetInvestAsteroid(aabm);
          } 
      }
      
      void DisplayNothing()
      {
          GetWindow()->SetInvestAsteroid(0);

          if (m_pConnectorPaneInventory)
              m_pConnectorPaneInventory->SetVisible(false);
          
          if (m_pConnectorPanePurchases)
              m_pConnectorPanePurchases->SetVisible(false);
          
          m_pImagePart = NULL;
          m_pPartType = NULL;
          m_pHullType = NULL;
          m_pBuyable = NULL;
          m_szModel = NULL;
          NeedPaint();
      }
      
private:
    
    void UpdateLayout()
    {
        InternalSetSize(m_pImageBkgnd->GetSurface()->GetSize());
        Pane::UpdateLayout();
    }
    
    void DrawName(Surface* pSurface, IbuyableIGC* pBuyable) const
    {
        pSurface->DrawString(
            TrekResources::SmallBoldFont(),
            Color(64.0f / 255.0f, 70.0f / 255.0f, 1.0f),
            WinPoint(15,10), 
            pBuyable->GetName()
        );
    }
    
    void DrawDescription(Surface* pSurface, IbuyableIGC* pBuyable) const
    {
        const int nBorderWidth = 13;
        const int nLineHeight = 12;
        const int nFirstLineY = 4*nLineHeight;
        
        const int nWidth = (int)m_pImageBkgnd->GetBounds().GetRect().XSize() - nBorderWidth*2;
        int nCurrentYPos = nFirstLineY + nBorderWidth;
        
        TRef<IEngineFont> pfont = TrekResources::SmallFont();
        Color             color = MakeColorFromCOLORREF(RGB(180,180,180));

        pSurface->DrawString(pfont, color, WinPoint(nBorderWidth, 15 + nLineHeight),   ZString("Cost:           $") + ZString(pBuyable->GetPrice()));
        pSurface->DrawString(pfont, color, WinPoint(nBorderWidth, 15 + 2*nLineHeight), ZString("Time:          ") + ZString((long)pBuyable->GetTimeToBuild()) + ZString(" Sec."));

        ZString strDescription(pBuyable->GetDescription() + ZString(" "));
        
        
        while (strDescription.GetLength() != 0)
        {
            // calculate the number of characters we can display on this line
            int nLineLength = TrekResources::SmallFont()->GetMaxTextLength(strDescription, nWidth, true);
            
            // try doing word breaking
            int nWordBreakLength = nLineLength;
            while (nWordBreakLength > 0 && strDescription[nWordBreakLength] != ' ' 
                && strDescription[nWordBreakLength-1] != ' ')
                nWordBreakLength--;
            if (nWordBreakLength != 0)
                nLineLength = nWordBreakLength;
            
            if (nLineLength <= 0)
            {
                // skip the unknown character
                nLineLength = 1;
            }
            else
            {
                pSurface->DrawString(pfont, color, WinPoint(nBorderWidth,nCurrentYPos), 
                    strDescription.Left(nLineLength));
                nCurrentYPos += nLineHeight;
            }
            
            strDescription = strDescription.RightOf(nLineLength);
        }
    }
    
    void Paint(Surface* pSurface)
    {
        pSurface->BitBlt(WinPoint(0, 0), 
            m_pImageBkgnd->GetSurface(),
            WinRect(0, 0, XSize(), YSize()));
        
        if (m_pBuyable)
        {
            DrawName(pSurface, m_pBuyable);
            DrawDescription(pSurface, m_pBuyable);
        }
    }
    
    
};

TRef<PartInfoPane> PartInfoPane::Create(ConnectorPane* pConnectorPanePurchases, ConnectorPane* pConnectorPaneInventory)
{
    return new PartInfoPaneImpl(pConnectorPanePurchases, pConnectorPaneInventory);
}

TRef<IObject> PartInfoPaneFactory::Apply(ObjectStack& stack)
{
    return (Pane*)PartInfoPane::Create(NULL, NULL);
}
