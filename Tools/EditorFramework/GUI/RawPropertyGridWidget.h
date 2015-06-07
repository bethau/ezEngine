#pragma once

#include <EditorFramework/Plugin.h>
#include <Foundation/Containers/HybridArray.h>
#include <ToolsFoundation/Object/DocumentObjectBase.h>
#include <ToolsFoundation/Selection/SelectionManager.h>
#include <EditorFramework/GUI/PropertyEditorBaseWidget.moc.h>
#include <EditorFramework/GUI/RawPropertyWidget.h>
#include <QWidget>
#include <QLayout>

class ezCollapsibleGroupBox;
class ezDocumentBase;
struct ezDocumentObjectPropertyEvent;

class EZ_EDITORFRAMEWORK_DLL ezRawPropertyGridWidget : public QWidget
{
public:
  ezRawPropertyGridWidget(ezDocumentBase* pDocument, QWidget* pParent);
  ~ezRawPropertyGridWidget();

  void ClearSelection();
  void SetSelection(const ezDeque<const ezDocumentObjectBase*>&);

private:
  void EditorPropertyChangedHandler(const ezPropertyEditorBaseWidget::Event& ed);
  void ObjectPropertyChangedHandler(const ezPropertyEditorBaseWidget::Event& ed);
  void PropertyChangedHandler(const ezPropertyEditorBaseWidget::Event& ed, bool bEditor);
  void SelectionEventHandler(const ezSelectionManager::Event& e);
  void PropertyEventHandler(const ezDocumentObjectPropertyEvent& e);

  ezDeque<const ezDocumentObjectBase*> m_Selection;
  QVBoxLayout* m_pLayout;
  QWidget* m_pMainContent;
  ezCollapsibleGroupBox* m_pGroups[2];
  ezRawPropertyWidget* m_pRawPropertyWidget[2];
  QSpacerItem* m_pSpacer;
  ezDocumentBase* m_pDocument;

  /// \todo Broken delegate stuff
  ezDelegate<void(const ezSelectionManager::Event&)> m_DelegateSelectionEvents;
  ezDelegate<void(const ezDocumentObjectPropertyEvent&)> m_DelegatePropertyEvents;
};


