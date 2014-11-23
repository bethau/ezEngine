#pragma once

#include <EditorFramework/Plugin.h>
#include <EditorFramework/GUI/PropertyEditorBaseWidget.moc.h>
#include <ToolsFoundation/Reflection/IReflectedTypeAccessor.h>
#include <QWidget>
#include <QLayout>

class QGroupBox;

class EZ_EDITORFRAMEWORK_DLL ezRawPropertyWidget : public QWidget
{
public:
  ezRawPropertyWidget(QWidget* pParent, const ezIReflectedTypeAccessor& accessor);

  ezEvent<const ezPropertyEditorBaseWidget::Event&> m_PropertyChanged;

  void ChangePropertyValue(const ezString& sPropertyPath, const ezVariant& value);

private:
  void BuildUI(const ezIReflectedTypeAccessor& et, const ezReflectedType* pType, ezPropertyPath& ParentPath, QLayout* pLayout);

  void PropertyChangedHandler(const ezPropertyEditorBaseWidget::Event& ed);

  QVBoxLayout* m_pLayout;

  ezMap<ezString, ezPropertyEditorBaseWidget*> m_PropertyWidgets;
};


