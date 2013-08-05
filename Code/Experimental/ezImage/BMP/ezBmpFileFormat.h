#pragma once
#include "ezImageFileFormat.h"

class ezBmpFormat : public ezImageFileFormat
{
public:
  virtual ezResult readImage(ezIBinaryStreamReader& stream, ezImage& image) const EZ_OVERRIDE;

  virtual ezResult writeImage(ezIBinaryStreamWriter& stream, const ezImage& image) const EZ_OVERRIDE;
};
