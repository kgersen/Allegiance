/////////////////////////////////////////////////////////////////////////////
// RangeValueSlider.cpp | Implementation of the TCRangeValueSlider class.

#include "RangeValueSlider.h"


/////////////////////////////////////////////////////////////////////////////
// TCRangeValueSlider


/////////////////////////////////////////////////////////////////////////////
// Operations

bool TCRangeValueSlider::Update(ULONG nObjects, ITCRangeValue** ppObjects)
{
  // Enable/disable the controls if no objects or more than 1 object
  bool bEnable = (1 == nObjects);
  if (bEnable)
  {
    __try
    {
      // Get the object
      ITCRangeValue* pobj = ppObjects[0];

      // Save the default value
      m_nValueDefault = pobj->DefaultValue;

      // Get the range of the value
      long nMin = pobj->MinValue;
      long nMax = pobj->MaxValue;
      long nSteps = nMax + 1 - nMin;

      // Set the tick frequency of the slider
      long nTickMarks = pobj->TickMarks;
      if (nTickMarks)
      {
        long nTickFreq = nSteps / pobj->TickMarks;
        SetTicFreq(nTickFreq);

        // Set the page size of the slider to be the same as the tick freq
        SetPageSize(nTickFreq);
      }
      else
      {
        ClearTics();
      }

      // Set the line size of the slider
      long nGranularity = pobj->Granularity;
      SetLineSize(nGranularity);

      // Set the range of the slider
      SetRange(nMin, nMax, true);

      // Set the position of the slider
      long nValue = pobj->RangeValue;
      SetPos(nValue);
    }
    __except(1)
    {
      bEnable = false;
    }
  }

  // Enable/disable the window if no objects or more than 1 object
  EnableWindow(bEnable);

  // Returnt the enabled flag
  return bEnable;
}

HRESULT TCRangeValueSlider::Apply(ULONG nObjects, ITCRangeValue** ppObjects)
{
  // Do nothing if no objects or more than 1 object
  if (1 != nObjects)
    return E_UNEXPECTED;

  // Get the object
  ITCRangeValue* pobj = ppObjects[0];

  // Apply the slider setting to the object
  long nValue = GetPos();
  return pobj->put_RangeValue(nValue);
}

