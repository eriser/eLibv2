#ifndef VSTBASEEDITOR_H_
#define VSTBASEEDITOR_H_

// std headers
#include <vector>
#include <map>
#include <string>

#include "Util/modLogger.h"
#include "VSTBaseBitmapManager.h"

// vst headers
#include <audioeffectx.h>
#include <vstcontrols.h>

namespace eLibV2
{
    namespace VSTBase
    {
        class VSTBaseEditor : public AEffGUIEditor
        {
        public:
            VSTBaseEditor(AudioEffect* effect);
            virtual ~VSTBaseEditor() {};

            virtual void setParameter(VstInt32 index, float value) = 0;
            virtual void valueChanged(CDrawContext* context, CControl* control) = 0;

        private:
            VSTBaseBitmapManager bitmapManager;
        };
    }
}

#endif /* VSTBASEEDITOR_H_ */