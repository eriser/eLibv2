/*
 * VSTBaseEditor
 */

#include <VSTBase/VSTBaseEditor.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

VSTBaseEditor::VSTBaseEditor(AudioEffect *effect) : AEffGUIEditor(effect)
{
    ModuleLogger::print("editor create");
}
