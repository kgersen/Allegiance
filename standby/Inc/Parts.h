/*-------------------------------------------------------------------------
 * Parts.h
 * 
 * Common parts definition for client/server
 * 
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

// Many of the fed messages depend on these structures, so FEDSRVVER in 
// messages.h must be incremented for any structure change in this file 
// as well as for any change to the messages in messages.h

// in order to be flexible, we need macros to make sure all part types handle
//  the base part info the same

#ifdef NEVER
// PART_VAR_PARMS is used when creating a pfm for the var parms. Must be first var parm
#define PART_VAR_PARMS(_PartType) \
      FM_VAR_PARM(_PartType##_FileModel,   CB_ZTS) \
      FM_VAR_PARM(_PartType##_FileIcon,    CB_ZTS) \
      FM_VAR_PARM(_PartType##_FileTexture, CB_ZTS) \
      FM_VAR_PARM(_PartType##_Description, CB_ZTS) 
#endif

//NYI hack for time to build
#define SET_PARTS(partstruct, _PartType) \
    (partstruct).price             = _PartType##_Price; \
    (partstruct).timeToBuild       = _PartType##_TimeToBuild; \
    (partstruct).groupID           = _PartType##_GroupID; \
    (partstruct).partID            = _PartType##_PartID; \
    (partstruct).successorPartID   = _PartType##_SuccessorID; \
    (partstruct).mass              = _PartType##_Mass; \
    (partstruct).equipmentType     = (EquipmentType) _PartType##_PartTypeID; \
    (partstruct).signature         = float(_PartType##_Signature) / 100.f; \
    (partstruct).partMask          = _PartType##_PartMask; \
    (partstruct).ttbmRequired.ClearAll(); \
    (partstruct).ttbmEffects.ClearAll(); \
    SQLSTRCPY((partstruct).description, _PartType##_Description); \
    SQLSTRCPY((partstruct).name,        _PartType##_Name); \
    SQLSTRCPY((partstruct).modelName,   _PartType##_FileModel); \
    SQLSTRCPY((partstruct).iconName,   _PartType##_FileIcon); \
    TechsListToBits(_PartType##_TechBitsReqd, (partstruct).ttbmRequired); \
    TechsListToBits(_PartType##_TechBitsEffect, (partstruct).ttbmEffects); \
    assert ((partstruct).ttbmEffects <= (partstruct).ttbmRequired); \
    SQLSTRCPY((partstruct).inventoryLineMDL, _PartType##_InventoryLineMDL);    

//NYI hack for time to build
#define SET_EXPEND(expendstruct, EXPENDTYPE) \
    SQLSTRCPY((expendstruct).modelName,               EXPENDTYPE##_ModelInstance); \
    SQLSTRCPY((expendstruct).iconName,                EXPENDTYPE##_IconInstance); \
    SQLSTRCPY((expendstruct).textureName,             EXPENDTYPE##_TextureInstance); \
    SQLSTRCPY((expendstruct).launcherDef.modelName,   EXPENDTYPE##_Model); \
    SQLSTRCPY((expendstruct).launcherDef.iconName,    EXPENDTYPE##_Icon); \
    SQLSTRCPY((expendstruct).launcherDef.name,        EXPENDTYPE##_Name); \
    SQLSTRCPY((expendstruct).launcherDef.description, EXPENDTYPE##_Description); \
    (expendstruct).launcherDef.ttbmRequired.ClearAll(); \
    (expendstruct).launcherDef.ttbmEffects.ClearAll(); \
    TechsListToBits(EXPENDTYPE##_TechBitsReqd, (expendstruct).launcherDef.ttbmRequired); \
    TechsListToBits(EXPENDTYPE##_TechBitsEffect, (expendstruct).launcherDef.ttbmEffects); \
    assert ((expendstruct).launcherDef.ttbmEffects <= (expendstruct).launcherDef.ttbmRequired); \
    (expendstruct).radius           = EXPENDTYPE##_Radius; \
    (expendstruct).rotation         = EXPENDTYPE##_Rotation; \
    (expendstruct).color.r          = EXPENDTYPE##_PercentRed   / 100.0f; \
    (expendstruct).color.g          = EXPENDTYPE##_PercentGreen / 100.0f; \
    (expendstruct).color.b          = EXPENDTYPE##_PercentBlue  / 100.0f; \
    (expendstruct).color.a          = EXPENDTYPE##_PercentAlpha / 100.0f; \
    (expendstruct).lifespan         = EXPENDTYPE##_LifeSpan; \
    (expendstruct).loadTime         = EXPENDTYPE##_LoadTime; \
    (expendstruct).signature        = float(EXPENDTYPE##_SignatureInstance) / 100.0f; \
    (expendstruct).hitPoints        = EXPENDTYPE##_HitPoints; \
    (expendstruct).defenseType      = EXPENDTYPE##_DefenseType; \
    (expendstruct).expendabletypeID = EXPENDTYPE##_ExpendableID; \
    (expendstruct).launcherDef.mass = EXPENDTYPE##_Mass; \
    (expendstruct).launcherDef.signature = float(EXPENDTYPE##_SignatureShip) / 100.0f; \
    (expendstruct).launcherDef.price = EXPENDTYPE##_Price; \
    (expendstruct).launcherDef.timeToBuild = EXPENDTYPE##_TimeToBuild; \
    (expendstruct).launcherDef.groupID = EXPENDTYPE##_GroupID; \
    (expendstruct).launcherDef.partMask = EXPENDTYPE##_PartMask; \
    (expendstruct).launcherDef.expendableSize = EXPENDTYPE##_ExpendableSize; \
    (expendstruct).eabmCapabilities = EXPENDTYPE##_Capabilities;

// Use this to actually reference existing var-length part props
#define FM_VARPART_REF(PFM, NAME) ((PFM)->part.ib##NAME ? (char*)(PFM) + (PFM)->part.ib##NAME : NULL)

/*
struct Part // only used as base type
{
  char      szName       [c_cbName];
  char      szFileModel  [c_cbFileName];
  char      szFileTexture[c_cbFileName];
  char      szDescription[c_cbDescription];
  BitMask   maskAvailability; //Test against sideID
  Money     price;
  PartID    partID;
  EquipmentType equipmentType; // compare this with a EquipmentType enum
  short     mass;
  HitPoints hitpoints;
  Capacity  capDrain; // power, space, whatever we end up calling it
};
*/
