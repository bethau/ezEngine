#pragma once

#include <Core/ResourceManager/ResourceHandle.h>
#include <Core/World/Declarations.h>
#include <Foundation/SimdMath/SimdTransform.h>
#include <Foundation/Strings/HashedString.h>
#include <Foundation/Types/SharedPtr.h>
#include <ProcGenPlugin/ProcGenPluginDLL.h>

class ezExpressionByteCode;
typedef ezTypedResourceHandle<class ezColorGradientResource> ezColorGradientResourceHandle;
typedef ezTypedResourceHandle<class ezPrefabResource> ezPrefabResourceHandle;
typedef ezTypedResourceHandle<class ezSurfaceResource> ezSurfaceResourceHandle;

namespace ezProcGenInternal
{
  class PlacementTile;
  class FindPlacementTilesTask;
  class PreparePlacementTask;
  class PlacementTask;

  struct Pattern
  {
    struct Point
    {
      ezVec2 m_Coordinates;
      float m_fThreshold;
    };

    ezArrayPtr<Point> m_Points;
    float m_fSize;
  };

  struct PlacementOutput : public ezRefCounted
  {
    PlacementOutput()
    {
      m_pPattern = nullptr;
      m_fFootprint = 1.0f;
      m_vMinOffset.SetZero();
      m_vMaxOffset.SetZero();
      m_fAlignToNormal = 1.0f;
      m_vMinScale.Set(1.0f);
      m_vMaxScale.Set(1.0f);
      m_fCullDistance = 100.0f;
      m_uiCollisionLayer = 0;

      m_pByteCode = nullptr;
    }

    float GetTileSize() const { return m_pPattern->m_fSize * m_fFootprint; }

    bool IsValid() const
    {
      return !m_ObjectsToPlace.IsEmpty() && m_pPattern != nullptr && m_fFootprint > 0.0f && m_fCullDistance > 0.0f &&
             m_pByteCode != nullptr;
    }

    ezHashedString m_sName;

    ezHybridArray<ezPrefabResourceHandle, 4> m_ObjectsToPlace;

    const Pattern* m_pPattern;
    float m_fFootprint;

    ezVec3 m_vMinOffset;
    ezVec3 m_vMaxOffset;

    float m_fAlignToNormal;

    ezVec3 m_vMinScale;
    ezVec3 m_vMaxScale;

    float m_fCullDistance;

    ezUInt32 m_uiCollisionLayer;

    ezColorGradientResourceHandle m_hColorGradient;

    ezSurfaceResourceHandle m_hSurface;

    ezExpressionByteCode* m_pByteCode;
  };

  struct EZ_PROCGENPLUGIN_DLL ExpressionInputs
  {
    static ezHashedString s_sPositionX;
    static ezHashedString s_sPositionY;
    static ezHashedString s_sPositionZ;
    static ezHashedString s_sNormalX;
    static ezHashedString s_sNormalY;
    static ezHashedString s_sNormalZ;
    static ezHashedString s_sPointIndex;
  };

  struct EZ_PROCGENPLUGIN_DLL ExpressionOutputs
  {
    static ezHashedString s_sDensity;
    static ezHashedString s_sScale;
    static ezHashedString s_sColorIndex;
    static ezHashedString s_sObjectIndex;
  };

  struct PlacementPoint
  {
    EZ_DECLARE_POD_TYPE();

    ezVec3 m_vPosition;
    float m_fScale;
    ezVec3 m_vNormal;
    ezUInt8 m_uiColorIndex;
    ezUInt8 m_uiObjectIndex;
    ezUInt16 m_uiPointIndex;
  };

  struct PlacementTransform
  {
    EZ_DECLARE_POD_TYPE();

    ezSimdTransform m_Transform;
    ezColorGammaUB m_Color;
    ezUInt8 m_uiObjectIndex;
    ezUInt16 m_uiPointIndex;
  };

  struct PlacementTileDesc
  {
    ezComponentHandle m_hComponent;
    ezUInt32 m_uiOutputIndex;
    ezInt32 m_iPosX;
    ezInt32 m_iPosY;
    float m_fMinZ;
    float m_fMaxZ;
    float m_fPatternSize;
    float m_fDistanceToCamera;

    ezHybridArray<ezSimdTransform, 8, ezAlignedAllocatorWrapper> m_GlobalToLocalBoxTransforms;
  };
} // namespace ezPPInternal