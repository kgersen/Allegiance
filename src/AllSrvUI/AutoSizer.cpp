/////////////////////////////////////////////////////////////////////////////
// AutoSizer.cpp | Implementation of the CAutoSizer class.

#include "stdafx.h"
#include "AutoSizer.h"


/////////////////////////////////////////////////////////////////////////////
// CAutoSizer


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CAutoSizer::CAutoSizer() :
  m_hwnd(NULL),
  m_bRecomputeMinWindow(false)
{
  ::SetRectEmpty(&m_rcMinWindow);
  ::SetRectEmpty(&m_rcMinClient);
  ::SetRectEmpty(&m_rcClient);
  ::SetRectEmpty(&m_rcClientOriginal);
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

bool CAutoSizer::SetWindowAndRules(HWND hwnd, const AutoSizerRule* pRules)
{
  // This should only be called once or reinitialized in WM_DESTROY
  _ASSERTE(!m_hwnd);

  // Save the specified HWND
  _ASSERTE(::IsWindow(hwnd));
  m_hwnd = hwnd;

  // Save the initial client rectangle
  ::GetClientRect(m_hwnd, &m_rcClient);
  ::GetClientRect(m_hwnd, &m_rcClientOriginal);

  // Set current client rectangle as minimum client rect
  ::GetClientRect(m_hwnd, &m_rcMinClient);

  // Set current window rectangle as minimum window rect
  ::GetWindowRect(m_hwnd, &m_rcMinWindow);
  ::MapWindowPoints(NULL, m_hwnd, (POINT*)&m_rcMinWindow, 2);

  // Allow NULL as pRules parameter
  if (pRules)
  {
    // Count the number of rules in the specified array
    const AutoSizerRule* pRulesIt = pRules;
	int cRules;	// mdvalley: define it here
    for (cRules = 0; !pRulesIt->IsEnd(); ++pRulesIt)
      ++cRules;

    // Copy the rules
    m_Rules.resize(cRules);
    for (int i = 0; i < m_Rules.size(); ++i)
    {
      m_Rules[i] = pRules + i;
      m_Rules[i].ResolveIDs(this);
      m_Rules[i].SaveInitialOffsets(this);
    }

    // Update the positions of the controls according to the array of rules
    UpdatePositions();
  }

  // Indicate success
  return true;
}

bool CAutoSizer::SetMinSize(int cx, int cy, bool bDlgUnits)
{
  // Ensure that this is only called when attached to a window
  if (!m_hwnd)
  {
    _ASSERTE(m_hwnd);
    return false;
  }

  // Compute the difference between current minimum client and window rects
  m_rcMinWindow.left   -= m_rcMinClient.left;
  m_rcMinWindow.top    -= m_rcMinClient.top;
  m_rcMinWindow.right  -= m_rcMinClient.right;
  m_rcMinWindow.bottom -= m_rcMinClient.bottom;

  // Save the new minimum client rect
  m_rcMinClient.left   = 0;
  m_rcMinClient.top    = 0;
  m_rcMinClient.right  = cx;
  m_rcMinClient.bottom = cy;

  // Convert dialog units to pixels, if specified
  if (bDlgUnits)
    ::MapDialogRect(m_hwnd, &m_rcMinClient);

  // Compute the new minimum window rect
  m_rcMinWindow.left   = m_rcMinClient.left   + m_rcMinWindow.left;
  m_rcMinWindow.top    = m_rcMinClient.top    + m_rcMinWindow.top;
  m_rcMinWindow.right  = m_rcMinClient.right  + m_rcMinWindow.right;
  m_rcMinWindow.bottom = m_rcMinClient.bottom + m_rcMinWindow.bottom;

  // Adjust the window if new minimum is bigger than current size
  if (m_rcMinClient.right > m_rcClient.right
    || m_rcMinClient.bottom > m_rcClient.bottom)
  {
    // Get the current window rect
    RECT rcWindow;
    ::GetWindowRect(m_hwnd, &rcWindow);

    // Compute the new extents of the window
    int cxWindow = max(rcWindow.right - rcWindow.left,
      m_rcMinWindow.right - m_rcMinWindow.left);
    int cyWindow = max(rcWindow.bottom - rcWindow.top,
      m_rcMinWindow.bottom - m_rcMinWindow.top);

    // Set the new window size
    ::SetWindowPos(m_hwnd, NULL, 0, 0, cxWindow, cyWindow,
      SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION);
  }

  // Indicate success
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// Operations

bool CAutoSizer::ProcessMessage(MSG* pMsg)
{
  LRESULT lr = 0;
  return ProcessMessage(pMsg->message, pMsg->wParam, pMsg->lParam, &lr);
}

bool CAutoSizer::ProcessMessage(UINT uMsg, WPARAM wp, LPARAM lp, LRESULT* plr)
{
  // Do nothing if the window has not been set
  if (!m_hwnd)
    return false;

  // Delegate messages to handler functions
  switch (uMsg)
  {
    case WM_DESTROY:
      OnDestroy();
      break;
    case WM_GETMINMAXINFO:
      OnGetMinMaxInfo((MINMAXINFO*)lp);
      if (plr)
        *plr = 0;
      break;
    case WM_SIZE:
      OnSize(wp, LOWORD(lp), HIWORD(lp));
      if (plr)
        *plr = 0;
      break;
    case WM_STYLECHANGED:
      OnStyleChanged();
      if (plr)
        *plr = 0;
      break;
    default:
      return false;
  }

  // Currently always returning false
  return false;
}

bool CAutoSizer::AddRule(HWND hwndFollower, AutoSizer_Follower edgeFollower,
  HWND hwndLeader, AutoSizer_Leader edgeLeader, AutoSizer_Refresh refresh)
{
  if (!::IsWindow(hwndFollower))
    return false;
  if (!hwndLeader)
    hwndLeader = GetWindow();

  // Create a rule structure with the new values
  AutoSizerRule rule;
  rule.m_idFollower = reinterpret_cast<UINT>(hwndFollower);
  rule.m_idLeader   = reinterpret_cast<UINT>(hwndLeader);
  rule.m_eFollower  = edgeFollower;
  rule.m_eLeader    = edgeLeader;
  rule.m_eRefresh   = refresh;

  XRuleIt it;	// mdvalley: define it here
  // Ensure that the specified rule does not already exist in the array
  for (it = m_Rules.begin(); it != m_Rules.end(); ++it)
    if (*it == &rule)
      return false;

  // Create a new rule item
  XRule xRule(&rule);
  xRule.SaveInitialOffsets(this);

  // Find the first rule with the same follower
  for (it = m_Rules.begin(); it != m_Rules.end(); ++it)
    if (it->GetFollower() == hwndFollower)
      break;

  // Find the first rule NOT with the same follower
  for (it; it != m_Rules.end(); ++it)
    if (it->GetFollower() != hwndFollower)
      break;

  // Insert the new rule into the array
  m_Rules.insert(it, xRule);

  // Update the positions of the controls according to the new array of rules
  UpdatePositions();

  // Indicate success
  return true;
}

bool CAutoSizer::RemoveRules(HWND hwndFollower)
{
  // Get the current size of the rules array
  int cRules = m_Rules.size();

  // Remove each rule for the specified follower window
  for (XRuleIt it = m_Rules.begin(); it != m_Rules.end();)
  {
    if (it->GetFollower() == hwndFollower)
      it = m_Rules.erase(it);
    else
      ++it;
  }

  // Indicate whether or not any rules were removed
  return cRules != m_Rules.size();
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void CAutoSizer::ComputeMinRects()
{
  _ASSERTE(m_hwnd);

}

void CAutoSizer::UpdatePositions()
{
  // Update the positions of the controls according to the array of rules
  HDWP hdwp = ::BeginDeferWindowPos(m_Rules.size());
  HWND hwndPrevFollower = NULL;
  HWND hwndFollower;
  RECT rcFollower, rcNew;
  XRuleIt it = m_Rules.begin();
  for (int i = 0; i <= m_Rules.size(); ++it, ++i)
  {
    // Get the window rectangle of the current follower control
    if (i == m_Rules.size() || it->GetFollower() != hwndPrevFollower)
    {
      // Apply the previous control's changes, if any
      if (hwndPrevFollower)
      {
        // Compute the old and new width and height
        int cxOld = rcFollower.right - rcFollower.left;
        int cyOld = rcFollower.bottom - rcFollower.top;
        int cxNew = rcNew.right - rcNew.left;
        int cyNew = rcNew.bottom - rcNew.top;

        // Determine the DeferWindowPos flags
        UINT nFlags = 0;
        if (cxOld == cxNew && cyOld == cyNew)
          nFlags |= SWP_NOSIZE;
        if (rcNew.left == rcFollower.left && rcNew.top == rcFollower.top)
          nFlags |= SWP_NOMOVE;

        // If the rectangle of the control has changed, reposition it
        if ((SWP_NOSIZE | SWP_NOMOVE) != nFlags)
        {
          // Move and/or size the control
          nFlags |= SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION;
          hdwp = ::DeferWindowPos(hdwp, hwndFollower, NULL,
            rcNew.left, rcNew.top, cxNew, cyNew, nFlags);
        }
      }

      // Exit loop now if beyond last rule in array
      if (i == m_Rules.size())
        break;

      // Save the new follower HWND
      hwndPrevFollower = it->GetFollower();

      // Get the window handle of the current follower control
      hwndFollower = hwndPrevFollower;

      // Get the window rectangle of the current follower control
      ::GetWindowRect(hwndFollower, &rcFollower);
      ::MapWindowPoints(NULL, m_hwnd, (POINT*)&rcFollower, 2);
      ::CopyRect(&rcNew, &rcFollower);
    }

    // Update the control's rectangle
    it->UpdateRectangle(this, rcNew);
  }
  ::EndDeferWindowPos(hdwp);
  ::UpdateWindow(m_hwnd);
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

void CAutoSizer::OnDestroy()
{
  // Release the array
  m_Rules.clear();

  // Reinitialize the object state
  m_hwnd = NULL;
  m_bRecomputeMinWindow = false;
  ::SetRectEmpty(&m_rcMinWindow);
  ::SetRectEmpty(&m_rcMinClient);
  ::SetRectEmpty(&m_rcClient);
  ::SetRectEmpty(&m_rcClientOriginal);
}

void CAutoSizer::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
  pMMI->ptMinTrackSize.x = m_rcMinWindow.right - m_rcMinWindow.left;
  pMMI->ptMinTrackSize.y = m_rcMinWindow.bottom - m_rcMinWindow.top;
}

void CAutoSizer::OnSize(UINT nType, int cx, int cy)
{
  if (!cx && !cy)
  {
    // Get the new client rectangle
    RECT rcClient;
    ::GetClientRect(GetWindow(), &rcClient);
    cx = rcClient.right - rcClient.left;
    cy = rcClient.bottom - rcClient.top;
  }

  // Recompute the minimum window rectangle, if flagged
  if (m_bRecomputeMinWindow)
  {
    // Reset the flag
    m_bRecomputeMinWindow = false;

    // Get the difference between the new client and window rects
    RECT rcWindow, rcClient;
    ::GetClientRect(m_hwnd, &rcClient);
    ::GetWindowRect(m_hwnd, &rcWindow);
    ::MapWindowPoints(NULL, m_hwnd, (POINT*)&rcWindow, 2);

    // Compute the difference between the new client and window rects
    int cxWindow = rcWindow.right - rcWindow.left;
    int cyWindow = rcWindow.bottom - rcWindow.top;
    rcWindow.left   -= rcClient.left;
    rcWindow.top    -= rcClient.top;
    rcWindow.right  -= rcClient.right;
    rcWindow.bottom -= rcClient.bottom;

    // Compute the new minimum window rect from the minimum client rect
    m_rcMinWindow.left   = m_rcMinClient.left   + rcWindow.left;
    m_rcMinWindow.top    = m_rcMinClient.top    + rcWindow.top;
    m_rcMinWindow.right  = m_rcMinClient.right  + rcWindow.right;
    m_rcMinWindow.bottom = m_rcMinClient.bottom + rcWindow.bottom;

    // Determine if the window needs to be resized if smaller than minimums
    if (cx < m_rcMinClient.right || cy < m_rcMinClient.bottom)
    {
      ::SetWindowPos(m_hwnd, NULL, 0, 0,
        max(0, m_rcMinWindow.right - m_rcMinWindow.left),
          max(0, m_rcMinWindow.bottom - m_rcMinWindow.top),
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION);
      return;
    }
  }

  // Save the new client rectangle
  m_rcClient.right = cx;
  m_rcClient.bottom = cy;

  // Update the positions of the controls according to the array of rules
  UpdatePositions();
}

void CAutoSizer::OnStyleChanged()
{
  // Indicate that next WM_SIZE will recompute minimum window rect
  m_bRecomputeMinWindow = true;
}


/////////////////////////////////////////////////////////////////////////////
// CAutoSizer::XRule


/////////////////////////////////////////////////////////////////////////////
// Operations

bool CAutoSizer::XRule::SaveInitialOffsets(CAutoSizer* pSizer)
{
  _ASSERTE(::IsWindow(GetFollower()));

  // Get the HWND of the follower control
  if (!GetFollower() || !::IsWindow(GetFollower()))
    return false;

  // Get the window rectangles of each control
  RECT rcFollower, rcLeader;
  const RECT* prcLeader;
  ::GetWindowRect(GetFollower(), &rcFollower);
  ::MapWindowPoints(NULL, pSizer->GetWindow(), (POINT*)&rcFollower, 2);
  if (GetLeader() == pSizer->GetWindow())
    prcLeader = pSizer->GetOriginalClientRect();
  else
  {
    prcLeader = &rcLeader;
    if (!GetLeader() || !::IsWindow(GetLeader()))
      return false;
    ::GetWindowRect(GetLeader(), &rcLeader);
    ::MapWindowPoints(NULL, pSizer->GetWindow(), (POINT*)&rcLeader, 2);
  }

  // Determine the leader coordinate to use for the offset calculation
  int nLeader = 0;
  switch (m_Rule.m_eLeader)
  {
    case AutoSizer_Lead_Left:
      nLeader = prcLeader->left;
      break;
    case AutoSizer_Lead_Right:
      nLeader = prcLeader->right;
      break;
    case AutoSizer_Lead_Top:
      nLeader = prcLeader->top;
      break;
    case AutoSizer_Lead_Bottom:
      nLeader = prcLeader->bottom;
      break;
    case AutoSizer_Lead_HCenter:
    case AutoSizer_Lead_VCenter:
      // There are no initial offsets to compute for centering rules
      break;
    default:
      _ASSERTE(false);
      return false;
  }

  // Determine the follower coordinate(s) to use for the offset calculation
  int cOffsets = 0;
  int nFollower[2] = {0, 0};
  switch (m_Rule.m_eFollower)
  {
    case AutoSizer_Follow_Left:
      cOffsets = 1;
      nFollower[0] = rcFollower.left;
      break;
    case AutoSizer_Follow_Right:
      cOffsets = 1;
      nFollower[0] = rcFollower.right;
      break;
    case AutoSizer_Follow_LeftRight:
      cOffsets = 2;
      nFollower[0] = rcFollower.left;
      nFollower[1] = rcFollower.right;
      break;
    case AutoSizer_Follow_Top:
      cOffsets = 1;
      nFollower[0] = rcFollower.top;
      break;
    case AutoSizer_Follow_Bottom:
      cOffsets = 1;
      nFollower[0] = rcFollower.bottom;
      break;
    case AutoSizer_Follow_TopBottom:
      cOffsets = 2;
      nFollower[0] = rcFollower.top;
      nFollower[1] = rcFollower.bottom;
      break;
    // There are no initial offsets to compute for centering rules
    case AutoSizer_Follow_HCenter:
    case AutoSizer_Follow_VCenter:
      break;
    default:
      _ASSERTE(false);
      return false;
  }

  // Loop thru and compute each follower offset
  for (int i = 0; i < cOffsets; ++i)
    m_Offsets[i] = nFollower[i] - nLeader;

  // Indicate success
  return true;
}

bool CAutoSizer::XRule::UpdateRectangle(CAutoSizer* pSizer, RECT& rcNew) const
{
  // Get the window rectangle of the leader control
  RECT rcLeader;
  const RECT* prcLeader;
  if (GetLeader() == pSizer->GetWindow())
    prcLeader = pSizer->GetClientRect();
  else
  {
    prcLeader = &rcLeader;
    _ASSERTE(::IsWindow(GetLeader()));
    if (!::IsWindow(GetLeader()))
      return false;
    ::GetWindowRect(GetLeader(), &rcLeader);
    ::MapWindowPoints(NULL, pSizer->GetWindow(), (POINT*)&rcLeader, 2);
  }

  // Determine the leader coordinate to use for the offset calculation
  int nLeader = 0;
  switch (m_Rule.m_eLeader)
  {
    case AutoSizer_Lead_Left:
      nLeader = prcLeader->left;
      break;
    case AutoSizer_Lead_Right:
      nLeader = prcLeader->right;
      break;
    case AutoSizer_Lead_Top:
      nLeader = prcLeader->top;
      break;
    case AutoSizer_Lead_Bottom:
      nLeader = prcLeader->bottom;
      break;
    case AutoSizer_Lead_HCenter:
      nLeader = prcLeader->left + (prcLeader->right - prcLeader->left) / 2;
      break;
    case AutoSizer_Lead_VCenter:
      nLeader = prcLeader->top + (prcLeader->bottom - prcLeader->top) / 2;
      break;
    default:
      _ASSERTE(false);
      return false;
  }

  // Adjust the rectangle of the follower control
  RECT rcOld;
  ::CopyRect(&rcOld, &rcNew);
  switch (m_Rule.m_eFollower)
  {
    case AutoSizer_Follow_Left:
      rcNew.left = nLeader + m_Offsets[0];
      break;
    case AutoSizer_Follow_Right:
      rcNew.right = nLeader + m_Offsets[0];
      break;
    case AutoSizer_Follow_LeftRight:
      rcNew.left = nLeader + m_Offsets[0];
      rcNew.right = nLeader + m_Offsets[1];
      break;
    case AutoSizer_Follow_Top:
      rcNew.top = nLeader + m_Offsets[0];
      break;
    case AutoSizer_Follow_Bottom:
      rcNew.bottom = nLeader + m_Offsets[0];
      break;
    case AutoSizer_Follow_TopBottom:
      rcNew.top = nLeader + m_Offsets[0];
      rcNew.bottom = nLeader + m_Offsets[1];
      break;
    case AutoSizer_Follow_HCenter:
    {
      int cxFollower = (rcNew.right - rcNew.left);
      rcNew.left = nLeader - (cxFollower / 2);
      rcNew.right = rcNew.left + cxFollower;
      break;
    } 
    case AutoSizer_Follow_VCenter:
    {
      int cyFollower = (rcNew.bottom - rcNew.top);
      rcNew.top = nLeader - (cyFollower / 2);
      rcNew.bottom = rcNew.top + cyFollower;
      break;
    } 
    default:
      _ASSERTE(false);
      return false;
  }

  // Compute the old and new width and height
  int cxOld = rcOld.right - rcOld.left;
  int cyOld = rcOld.bottom - rcOld.top;
  int cxNew = rcNew.right - rcNew.left;
  int cyNew = rcNew.bottom - rcNew.top;

  // Refresh the dialog window as specified, if needed
  if (m_Rule.m_eRefresh > AutoSizer_Refresh_NoRefresh)
  {
    if (cxOld != cxNew || cyOld != cyNew ||
      rcNew.left != rcOld.left || rcNew.top != rcOld.top)
    {
      bool bEraseBk = AutoSizer_Refresh_BkRefresh == m_Rule.m_eRefresh;
      ::InvalidateRect(pSizer->GetWindow(), &rcNew, bEraseBk);
    }
  }

  // Indicate success
  return true;
}


