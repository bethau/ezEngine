
static double g_fInvQpcFrequency;

void ezTime::Initialize()
{
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);

  g_fInvQpcFrequency = 1.0 / double(frequency.QuadPart);
}

//#if EZ_ENABLED(EZ_COMPILE_FOR_DEVELOPMENT)
//  static LARGE_INTEGER s_LastTime = { 0 };
//#endif

ezTime ezTime::Now()
{
//#if EZ_ENABLED(EZ_COMPILE_FOR_DEVELOPMENT)
//  volatile LARGE_INTEGER LastTime = s_LastTime;
//#endif

  LARGE_INTEGER temp;
  QueryPerformanceCounter(&temp);

  /// \todo QueryPerformanceCounter can jump 'back' in time on Windows.
//#if EZ_ENABLED(EZ_COMPILE_FOR_DEVELOPMENT)
//  EZ_ASSERT_DEV(LastTime.QuadPart <= temp.QuadPart, "Serious problem, Steve. This is like \"Houston, forget that other thing\".\n\n\n"
//    "When this happens the PC timer is unreliable. It was probably called from different threads and the clocks on different CPU cores seem to return different results.\n"
//    "Under these conditions the engine cannot run reliably, it might crash or act weird.");
//  s_LastTime = temp;
//#endif

  return ezTime::Seconds(double(temp.QuadPart) * g_fInvQpcFrequency);
}

