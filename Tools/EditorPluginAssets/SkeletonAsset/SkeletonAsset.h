#pragma once

#include <EditorFramework/Assets/SimpleAssetDocument.h>
#include <EditorFramework/Assets/AssetDocumentGenerator.h>

class ezSkeletonAssetProperties : public ezReflectedClass
{
  EZ_ADD_DYNAMIC_REFLECTION(ezSkeletonAssetProperties, ezReflectedClass);

public:
  ezSkeletonAssetProperties();
  ~ezSkeletonAssetProperties();

  ezString m_sAnimationFile;

  ezEnum<ezBasisAxis> m_ForwardDir;
  ezEnum<ezBasisAxis> m_RightDir;
  ezEnum<ezBasisAxis> m_UpDir;
};

//////////////////////////////////////////////////////////////////////////

class ezSkeletonAssetDocument : public ezSimpleAssetDocument<ezSkeletonAssetProperties>
{
  EZ_ADD_DYNAMIC_REFLECTION(ezSkeletonAssetDocument, ezSimpleAssetDocument<ezSkeletonAssetDocument>);

public:
  ezSkeletonAssetDocument(const char* szDocumentPath);
  ~ezSkeletonAssetDocument();

  virtual const char* QueryAssetType() const override { return "Skeleton"; }

protected:
  virtual ezStatus InternalTransformAsset(ezStreamWriter& stream, const char* szOutputTag, const char* szPlatform, const ezAssetFileHeader& AssetHeader, bool bTriggeredManually) override;
  virtual ezStatus InternalCreateThumbnail(const ezAssetFileHeader& AssetHeader) override;
};

//////////////////////////////////////////////////////////////////////////

class ezSkeletonAssetDocumentGenerator : public ezAssetDocumentGenerator
{
  EZ_ADD_DYNAMIC_REFLECTION(ezSkeletonAssetDocumentGenerator, ezAssetDocumentGenerator);

public:
  ezSkeletonAssetDocumentGenerator();
  ~ezSkeletonAssetDocumentGenerator();

  virtual void GetImportModes(const char* szParentDirRelativePath, ezHybridArray<ezAssetDocumentGenerator::Info, 4>& out_Modes) const override;
  virtual ezStatus Generate(const char* szDataDirRelativePath, const ezAssetDocumentGenerator::Info& info, ezDocument*& out_pGeneratedDocument) override;
  virtual const char* GetDocumentExtension() const override { return "ezSkeletonAsset"; }
  virtual const char* GetGeneratorGroup() const override { return "AnimationSkeletonGroup"; }
};
