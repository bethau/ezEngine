#include <RendererCore/PCH.h>
#include <RendererCore/Pipeline/RenderPipeline.h>
#include <RendererCore/RenderContext/RenderContext.h>
#include <RendererCore/RenderLoop/RenderLoop.h>

#include <Foundation/Configuration/CVar.h>

#include <Core/World/World.h>

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezRenderData, ezReflectedClass, 1, ezRTTINoAllocator);
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezRenderer, ezReflectedClass, 1, ezRTTINoAllocator);
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_IMPLEMENT_MESSAGE_TYPE(ezExtractRenderDataMessage);

extern ezCVarBool CVarMultithreadedRendering;

ezRenderPassType ezRenderPipeline::s_uiNextPassType = 0;
ezRenderPipeline::PassTypeData ezRenderPipeline::s_PassTypeData[MAX_PASS_TYPES];

void ezRenderPipeline::PassData::SortRenderData()
{
  struct RenderDataComparer
  {
    EZ_FORCE_INLINE bool Less(const ezRenderData* a, const ezRenderData* b) const
    {
      return a->GetSortingKey() < b->GetSortingKey();
    }
  };

  m_RenderData.Sort(RenderDataComparer());
}

ezRenderPipeline::ezRenderPipeline()
{
  m_uiLastExtractionFrame = -1;
  m_uiLastRenderFrame = -1;
}

ezRenderPipeline::~ezRenderPipeline()
{
  ClearPipelineData(&m_Data[0]);
  ClearPipelineData(&m_Data[1]);

  /// \todo: this will crash if passes are not allocated with the default allocator. We need a better mechanism here.
  for (auto pPass : m_Passes)
  {
    EZ_DEFAULT_DELETE(pPass);
  }
  m_Passes.Clear();
}

void ezRenderPipeline::ExtractData(const ezView& view)
{
  // Is this view already extracted?
  if (m_uiLastExtractionFrame == ezRenderLoop::GetFrameCounter())
    return;

  m_uiLastExtractionFrame = ezRenderLoop::GetFrameCounter();

  PipelineData* pPipelineData = GetPipelineDataForExtraction();

  // Usually clear is not needed, only if the multithreading flag is switched during runtime.
  ClearPipelineData(pPipelineData);

  ezExtractRenderDataMessage msg;
  msg.m_pRenderPipeline = this;
  msg.m_pView = &view;

  /// \todo use spatial data to do visibility culling etc.
  for (auto it = view.GetWorld()->GetObjects(); it.IsValid(); ++it)
  {
    const ezGameObject* pObject = it;
    pObject->SendMessage(msg);
  }

  for (ezUInt32 uiPassIndex = 0; uiPassIndex < pPipelineData->m_PassData.GetCount(); ++uiPassIndex)
  {
    PassData& data = pPipelineData->m_PassData[uiPassIndex];
    data.SortRenderData();
  }
}

void ezRenderPipeline::Render(const ezView& view, ezRenderContext* pRendererContext)
{
  EZ_ASSERT_DEV(m_uiLastRenderFrame != ezRenderLoop::GetFrameCounter(), "Render must not be called multiple times per frame.");
  m_uiLastRenderFrame = ezRenderLoop::GetFrameCounter();

  // calculate camera matrices
  const ezRectFloat& viewPortRect = view.GetViewport();
  pRendererContext->GetGALContext()->SetViewport(viewPortRect.x, viewPortRect.y, viewPortRect.width, viewPortRect.height, 0.0f, 1.0f);

  ezRenderViewContext renderViewContext;
  renderViewContext.m_pView = &view;
  renderViewContext.m_pRenderContext = pRendererContext;

  for (ezUInt32 i = 0; i < m_Passes.GetCount(); ++i)
  {
    m_Passes[i]->Run(renderViewContext);
  }

  ClearPipelineData(GetPipelineDataForRendering());
}

void ezRenderPipeline::AddPass(ezRenderPipelinePass* pPass)
{
  pPass->m_pPipeline = this;

  m_Passes.PushBack(pPass);
}

void ezRenderPipeline::RemovePass(ezRenderPipelinePass* pPass)
{
  m_Passes.Remove(pPass);

  pPass->m_pPipeline = nullptr;
}

ezRenderPipeline::PipelineData* ezRenderPipeline::GetPipelineDataForExtraction()
{
  return &m_Data[ezRenderLoop::GetFrameCounter() & 1];
}

ezRenderPipeline::PipelineData* ezRenderPipeline::GetPipelineDataForRendering()
{
  const ezUInt32 uiFrameCounter = ezRenderLoop::GetFrameCounter() + (CVarMultithreadedRendering ? 1 : 0);
  return &m_Data[uiFrameCounter & 1];
}


// static
void ezRenderPipeline::ClearPipelineData(PipelineData* pPipeLineData)
{
  for (ezUInt32 uiPassIndex = 0; uiPassIndex < pPipeLineData->m_PassData.GetCount(); ++uiPassIndex)
  {
    PassData& data = pPipeLineData->m_PassData[uiPassIndex];

    /// \todo not needed once we use a proper allocator
    for (ezUInt32 i = 0; i < data.m_RenderData.GetCount(); ++i)
    {
      ezRenderData* pRenderData = const_cast<ezRenderData*>(data.m_RenderData[i]);
      EZ_DEFAULT_DELETE(pRenderData);
    }

    data.m_RenderData.Clear();
  }
}

// static 
bool ezRenderPipeline::IsPipelineDataEmpty(PipelineData* pPipeLineData)
{
  for (ezUInt32 uiPassIndex = 0; uiPassIndex < pPipeLineData->m_PassData.GetCount(); ++uiPassIndex)
  {
    if (!pPipeLineData->m_PassData[uiPassIndex].m_RenderData.IsEmpty())
      return false;
  }

  return true;
}

//static 
ezRenderPassType ezRenderPipeline::FindOrRegisterPassType(const char* szPassTypeName)
{
  EZ_ASSERT_RELEASE(MAX_PASS_TYPES > s_uiNextPassType, "Reached the maximum of %d pass types.", MAX_PASS_TYPES);
  
  ezTempHashedString passTypeName(szPassTypeName);

  for (ezRenderPassType type = 0; type < MAX_PASS_TYPES; ++type)
  {
    if (s_PassTypeData[type].m_sName == passTypeName)
      return type;
  }

  ezRenderPassType newType = s_uiNextPassType;
  s_PassTypeData[newType].m_sName.Assign(szPassTypeName);
  s_PassTypeData[newType].m_ProfilingID = ezProfilingSystem::CreateId(szPassTypeName);

  ++s_uiNextPassType;
  return newType;
}

ezRenderPassType ezDefaultPassTypes::Opaque = ezRenderPipeline::FindOrRegisterPassType("Opaque");
ezRenderPassType ezDefaultPassTypes::Masked = ezRenderPipeline::FindOrRegisterPassType("Masked");
ezRenderPassType ezDefaultPassTypes::Transparent = ezRenderPipeline::FindOrRegisterPassType("Transparent");



EZ_STATICLINK_FILE(RendererCore, RendererCore_Pipeline_Implementation_RenderPipeline);

