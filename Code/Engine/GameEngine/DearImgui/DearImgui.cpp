#include <PCH.h>
#include <GameEngine/DearImgui/DearImgui.h>
#include <Foundation/Configuration/Startup.h>
#include <GameApplication/GameApplication.h>
#include <RendererCore/Textures/Texture2DResource.h>
#include <Foundation/Time/Clock.h>
#include <Core/Input/InputManager.h>

EZ_IMPLEMENT_SINGLETON(ezImgui);

ezImgui::ezImgui()
  : m_SingletonRegistrar(this)
{
  Startup();
}


ezImgui::~ezImgui()
{
  Shutdown();
}

void ezImgui::Startup()
{
  ImGuiIO& cfg = ImGui::GetIO();

  cfg.DisplaySize.x = 1650;
  cfg.DisplaySize.y = 1080;

  cfg.KeyMap[ImGuiKey_Tab] = ImGuiKey_Tab;
  cfg.KeyMap[ImGuiKey_LeftArrow] = ImGuiKey_LeftArrow;
  cfg.KeyMap[ImGuiKey_RightArrow] = ImGuiKey_RightArrow;
  cfg.KeyMap[ImGuiKey_UpArrow] = ImGuiKey_UpArrow;
  cfg.KeyMap[ImGuiKey_DownArrow] = ImGuiKey_DownArrow;
  cfg.KeyMap[ImGuiKey_PageUp] = ImGuiKey_PageUp;
  cfg.KeyMap[ImGuiKey_PageDown] = ImGuiKey_PageDown;
  cfg.KeyMap[ImGuiKey_Home] = ImGuiKey_Home;
  cfg.KeyMap[ImGuiKey_End] = ImGuiKey_End;
  cfg.KeyMap[ImGuiKey_Delete] = ImGuiKey_Delete;
  cfg.KeyMap[ImGuiKey_Backspace] = ImGuiKey_Backspace;
  cfg.KeyMap[ImGuiKey_Enter] = ImGuiKey_Enter;
  cfg.KeyMap[ImGuiKey_Escape] = ImGuiKey_Escape;
  cfg.KeyMap[ImGuiKey_A] = ImGuiKey_A;
  cfg.KeyMap[ImGuiKey_C] = ImGuiKey_C;
  cfg.KeyMap[ImGuiKey_V] = ImGuiKey_V;
  cfg.KeyMap[ImGuiKey_X] = ImGuiKey_X;
  cfg.KeyMap[ImGuiKey_Y] = ImGuiKey_Y;
  cfg.KeyMap[ImGuiKey_Z] = ImGuiKey_Z;


  {
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    ezGALSystemMemoryDescription memoryDesc;
    memoryDesc.m_pData = pixels;
    memoryDesc.m_uiRowPitch = width * 4;
    memoryDesc.m_uiSlicePitch = width * height * 4;

    ezTexture2DResourceDescriptor desc;
    desc.m_DescGAL.m_uiWidth = width;
    desc.m_DescGAL.m_uiHeight = height;
    desc.m_DescGAL.m_Format = ezGALResourceFormat::RGBAUByteNormalized;
    desc.m_InitialContent = ezMakeArrayPtr(&memoryDesc, 1);

    m_hTextures.PushBack(ezResourceManager::CreateResource<ezTexture2DResource>("ImguiFont", desc));

    const size_t id = (size_t)m_hTextures.GetCount() - 1;
    io.Fonts->TexID = reinterpret_cast<void*>(id);
  }


  ImGui::NewFrame();
}

void ezImgui::Shutdown()
{
  m_hTextures.Clear();
}

void ezImgui::BeginNewFrame(ezSizeU32 windowResolution)
{
  ImGuiIO& cfg = ImGui::GetIO();

  cfg.DisplaySize.x = (float)windowResolution.width;
  cfg.DisplaySize.y = (float)windowResolution.height;
  cfg.DeltaTime = (float)ezClock::GetGlobalClock()->GetTimeDiff().GetSeconds();

  if (m_bPassInputToImgui)
  {
    char szUtf8[8] = "";
    char* pChar = szUtf8;
    ezUnicodeUtils::EncodeUtf32ToUtf8(ezInputManager::RetrieveLastCharacter(), pChar);
    cfg.AddInputCharactersUTF8(szUtf8);

    float mousex, mousey;
    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionX, &mousex);
    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionY, &mousey);
    cfg.MousePos.x = cfg.DisplaySize.x * mousex;
    cfg.MousePos.y = cfg.DisplaySize.y * mousey;
    cfg.MouseDown[0] = ezInputManager::GetInputSlotState(ezInputSlot_MouseButton0) >= ezKeyState::Pressed;
    cfg.MouseDown[1] = ezInputManager::GetInputSlotState(ezInputSlot_MouseButton1) >= ezKeyState::Pressed;
    cfg.MouseDown[2] = ezInputManager::GetInputSlotState(ezInputSlot_MouseButton2) >= ezKeyState::Pressed;

    cfg.MouseWheel = 0;
    if (ezInputManager::GetInputSlotState(ezInputSlot_MouseWheelDown) == ezKeyState::Pressed)
      cfg.MouseWheel = -1;
    if (ezInputManager::GetInputSlotState(ezInputSlot_MouseWheelUp) == ezKeyState::Pressed)
      cfg.MouseWheel = +1;

    cfg.KeyAlt = ezInputManager::GetInputSlotState(ezInputSlot_KeyLeftAlt) >= ezKeyState::Pressed || ezInputManager::GetInputSlotState(ezInputSlot_KeyRightAlt) >= ezKeyState::Pressed;
    cfg.KeyCtrl = ezInputManager::GetInputSlotState(ezInputSlot_KeyLeftCtrl) >= ezKeyState::Pressed || ezInputManager::GetInputSlotState(ezInputSlot_KeyRightCtrl) >= ezKeyState::Pressed;
    cfg.KeyShift = ezInputManager::GetInputSlotState(ezInputSlot_KeyLeftShift) >= ezKeyState::Pressed || ezInputManager::GetInputSlotState(ezInputSlot_KeyRightShift) >= ezKeyState::Pressed;
    cfg.KeySuper = ezInputManager::GetInputSlotState(ezInputSlot_KeyLeftWin) >= ezKeyState::Pressed || ezInputManager::GetInputSlotState(ezInputSlot_KeyRightWin) >= ezKeyState::Pressed;

    cfg.KeysDown[ImGuiKey_Tab] = ezInputManager::GetInputSlotState(ezInputSlot_KeyTab) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_LeftArrow] = ezInputManager::GetInputSlotState(ezInputSlot_KeyLeft) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_RightArrow] = ezInputManager::GetInputSlotState(ezInputSlot_KeyRight) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_UpArrow] = ezInputManager::GetInputSlotState(ezInputSlot_KeyUp) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_DownArrow] = ezInputManager::GetInputSlotState(ezInputSlot_KeyDown) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_PageUp] = ezInputManager::GetInputSlotState(ezInputSlot_KeyPageUp) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_PageDown] = ezInputManager::GetInputSlotState(ezInputSlot_KeyPageDown) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_Home] = ezInputManager::GetInputSlotState(ezInputSlot_KeyHome) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_End] = ezInputManager::GetInputSlotState(ezInputSlot_KeyEnd) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_Delete] = ezInputManager::GetInputSlotState(ezInputSlot_KeyDelete) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_Backspace] = ezInputManager::GetInputSlotState(ezInputSlot_KeyBackspace) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_Enter] = ezInputManager::GetInputSlotState(ezInputSlot_KeyReturn) >= ezKeyState::Pressed || ezInputManager::GetInputSlotState(ezInputSlot_KeyNumpadEnter) >= ezKeyState::Pressed;;
    cfg.KeysDown[ImGuiKey_Escape] = ezInputManager::GetInputSlotState(ezInputSlot_KeyEscape) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_A] = ezInputManager::GetInputSlotState(ezInputSlot_KeyA) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_C] = ezInputManager::GetInputSlotState(ezInputSlot_KeyC) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_V] = ezInputManager::GetInputSlotState(ezInputSlot_KeyV) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_X] = ezInputManager::GetInputSlotState(ezInputSlot_KeyX) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_Y] = ezInputManager::GetInputSlotState(ezInputSlot_KeyY) >= ezKeyState::Pressed;
    cfg.KeysDown[ImGuiKey_Z] = ezInputManager::GetInputSlotState(ezInputSlot_KeyZ) >= ezKeyState::Pressed;
  }
  else
  {
    cfg.ClearInputCharacters();

    cfg.MousePos.x = -1;
    cfg.MousePos.y = -1;

    cfg.MouseDown[0] = false;
    cfg.MouseDown[1] = false;
    cfg.MouseDown[2] = false;

    cfg.MouseWheel = 0;

    cfg.KeyAlt = false;
    cfg.KeyCtrl = false;
    cfg.KeyShift = false;
    cfg.KeySuper = false;

    for (int i = 0; i <= ImGuiKey_COUNT; ++i)
      cfg.KeysDown[i] = false;
  }

  ImGui::NewFrame();

  m_bImguiWantsInput = cfg.WantCaptureKeyboard || cfg.WantCaptureMouse;
}
