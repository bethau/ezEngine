#pragma once

#include <Foundation/Basics.h>

class ezIBinaryStreamReader;
class ezIBinaryStreamWriter;
class ezImage;

class ezImageFileFormat
{
public:
  virtual ezResult readImage(ezIBinaryStreamReader& stream, ezImage& image) const = 0;
  virtual ezResult writeImage(ezIBinaryStreamWriter& stream, const ezImage& image) const = 0;
};
