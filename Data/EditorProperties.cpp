#include <Data/EditorProperties.h>

using namespace eLibV2::Data;

UInt8 EditorProperties::mNumPages = 0;
std::map<UInt8, UInt16> EditorProperties::mBackgroundBitmaps;
std::map<std::string, UInt16> EditorProperties::mControlBitmaps;
