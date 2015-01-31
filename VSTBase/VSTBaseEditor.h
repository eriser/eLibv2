#ifndef VSTBASEEDITOR_H_
#define VSTBASEEDITOR_H_

// std headers
#include <vector>
#include <map>
#include <string>

#include <Util/modLogger.h>
#include <Data/modEditorProperties.h>

#include <VSTBase/VSTBaseBitmapManager.h>
#include <VSTBase/VSTBaseClass.h>

// vst headers
#include <audioeffectx.h>
#include <vstcontrols.h>

namespace eLibV2
{
	namespace VSTBase
	{
		enum ControlType
		{
			kOnOffButton = 1,
			kKickButton,
			kTextLabel,
			kOptionMenu,
			kAnimKnob,
			kNumControls
		};

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
			CView* addControl(ControlType type, CControlListener *listener, CPoint size, CPoint offset, VstInt32 tag, VstInt32 bitmapId, CPoint handle);
			void attachToPage(VstInt32 pageIndex, CView * control);

		protected:
			virtual bool openInvoked() = 0;

		private:
			VSTBaseClass *mEffect;
			EditorProperties mProperties;
			VSTBaseBitmapManager bitmapManager;

			// controls
			std::vector<CKickButton *> mKickButtons;

		protected:
			VstInt16 mActivePage;
			std::vector<CViewContainer*> mEditorPage;
		};
	}
}

#endif
