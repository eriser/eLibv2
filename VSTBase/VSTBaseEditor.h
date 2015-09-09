#ifndef VSTBASEEDITOR_H_
#define VSTBASEEDITOR_H_

// std headers
#include <vector>
#include <map>
#include <string>

#include <Util/Logger.h>
#include <Data/EditorProperties.h>
#include <Data/EditorParameter.h>

#include <VSTBase/VSTBaseBitmapManager.h>
#include <VSTBase/VSTBaseClass.h>

// vst headers
#include <audioeffectx.h>
#include <vstcontrols.h>

using namespace eLibV2::Data;

namespace eLibV2
{
    namespace VSTBase
    {
        class VSTBaseEditor : public AEffGUIEditor
        {
        public:
            VSTBaseEditor(VSTBaseClass* effect);

            virtual VSTBaseClass* getEffect();

            virtual void setupEditor(const EditorProperties properties);

            virtual void setParameter(VstInt32 index, float value) = 0;
            virtual void valueChanged(CDrawContext* context, CControl* control) = 0;

            virtual bool open(void *ptr);
            virtual void close();

            virtual void idle();

            CBitmap* getBitmap(const VstInt32 id);

            /**
            add a gui-control to internal manager
            @param type the type control @see ControlType
            @param listener the listener for control-events. usually the editor itself
            @param position desired position of control
            @param tag internal id used within valueChanged or setParameter
            @param numBitmaps number of sub-bitmaps present in bitmap for control (has to be loaded into BitmapManager)
            @param bitmapId id which was used to load bitmap into BitmapManager
            @param handle ???
            */
            CView* addControl(EditorParameter::ControlType type, CControlListener* listener, CPoint position, VstInt32 tag, VstInt32 numBitmaps, VstInt32 bitmapId, CPoint handle);

            /**
            set value of specified control
            @param type type of control @see ControlType
            @param tag internal id used to specify the gui-control
            @param value value the control should be set to
            */
            void setControlValue(EditorParameter::ControlType type, VstInt32 tag, double value);

            /**
            attach gui-control to the ViewContainer with the given index
            @param pageIndex index of ViewContainer
            @param control gui-control to attach
            */
            void attachToPage(VstInt32 pageIndex, CView* control);

        protected:
            /**
            abstract method which is called when editor is opened
            */
            virtual bool openInvoked() = 0;

        private:
            VSTBaseClass *mEffect;
            EditorProperties mProperties;
            VSTBaseBitmapManager bitmapManager;

            // controls
            std::map<VstInt32, COnOffButton*> mOnOffButtons;
            std::map<VstInt32, CKickButton*> mKickButtons;
            std::map<VstInt32, CTextLabel*> mTextLabels;
            std::map<VstInt32, COptionMenu*> mOptionMenus;
            std::map<VstInt32, CAnimKnob*> mAnimKnobs;
            std::map<VstInt32, CMovieBitmap*> mMovieBitmap;

        protected:
            VstInt16 mActivePage;
            std::vector<CViewContainer*> mEditorPage;
        };
    }
}

#endif
