#pragma once

#include <Foundation/Basics.h>
#include <GuiFoundation/DocumentWindow/DocumentWindow.moc.h>
#include <ToolsFoundation/Object/DocumentObjectManager.h>
#include <GuiFoundation/Widgets/CurveEditData.h>

class ezQtPropertyAnimModel;
class QTreeView;
class QItemSelection;
class QItemSelectionModel;
class ezQtCurve1DEditorWidget;
struct ezDocumentObjectPropertyEvent;
struct ezDocumentObjectStructureEvent;
class ezQtColorGradientEditorWidget;
class ezColorGradientAssetData;

class ezQtPropertyAnimAssetDocumentWindow : public ezQtDocumentWindow
{
  Q_OBJECT

public:
  ezQtPropertyAnimAssetDocumentWindow(ezDocument* pDocument);
  ~ezQtPropertyAnimAssetDocumentWindow();

  virtual const char* GetWindowLayoutGroupName() const { return "PropertyAnimAsset"; }

private slots:
  void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

  //////////////////////////////////////////////////////////////////////////
  // Curve editor events
  
  void onCurveInsertCpAt(ezUInt32 uiCurveIdx, ezInt64 tickX, double newPosY);
  void onCurveCpMoved(ezUInt32 curveIdx, ezUInt32 cpIdx, ezInt64 iTickX, double newPosY);
  void onCurveCpDeleted(ezUInt32 curveIdx, ezUInt32 cpIdx);
  void onCurveTangentMoved(ezUInt32 curveIdx, ezUInt32 cpIdx, float newPosX, float newPosY, bool rightTangent);
  void onLinkCurveTangents(ezUInt32 curveIdx, ezUInt32 cpIdx, bool bLink);
  void onCurveTangentModeChanged(ezUInt32 curveIdx, ezUInt32 cpIdx, bool rightTangent, int mode);

  void onCurveBeginOperation(QString name);
  void onCurveEndOperation(bool commit);
  void onCurveBeginCpChanges(QString name);
  void onCurveEndCpChanges();

  //////////////////////////////////////////////////////////////////////////
  // Color gradient editor events
  
  void onGradientColorCpAdded(float posX, const ezColorGammaUB& color);
  void onGradientAlphaCpAdded(float posX, ezUInt8 alpha);
  void onGradientIntensityCpAdded(float posX, float intensity);
  void MoveGradientCP(ezInt32 idx, float newPosX, const char* szArrayName);
  void onGradientColorCpMoved(ezInt32 idx, float newPosX);
  void onGradientAlphaCpMoved(ezInt32 idx, float newPosX);
  void onGradientIntensityCpMoved(ezInt32 idx, float newPosX);
  void RemoveGradientCP(ezInt32 idx, const char* szArrayName);
  void onGradientColorCpDeleted(ezInt32 idx);
  void onGradientAlphaCpDeleted(ezInt32 idx);
  void onGradientIntensityCpDeleted(ezInt32 idx);
  void onGradientColorCpChanged(ezInt32 idx, const ezColorGammaUB& color);
  void onGradientAlphaCpChanged(ezInt32 idx, ezUInt8 alpha);
  void onGradientIntensityCpChanged(ezInt32 idx, float intensity);
  void onGradientBeginOperation();
  void onGradientEndOperation(bool commit);
  //void onGradientNormalizeRange();

  //////////////////////////////////////////////////////////////////////////

private:
  void PropertyEventHandler(const ezDocumentObjectPropertyEvent& e);
  void StructureEventHandler(const ezDocumentObjectStructureEvent& e);
  void UpdateCurveEditor();
  void UpdateGradientEditor();

  ezCurveGroupData m_CurvesToDisplay;
  ezColorGradientAssetData* m_pGradientToDisplay = nullptr;
  ezInt32 m_iMapGradientToTrack = -1;
  ezDynamicArray<ezInt32> m_MapSelectionToTrack;
  QTreeView* m_pPropertyTreeView = nullptr;
  ezQtPropertyAnimModel* m_pPropertiesModel;
  QItemSelectionModel* m_pSelectionModel = nullptr;
  ezQtCurve1DEditorWidget* m_pCurveEditor = nullptr;
  ezQtColorGradientEditorWidget* m_pGradientEditor = nullptr;
};
