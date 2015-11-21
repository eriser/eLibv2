#ifndef MODBASEINSTRUMENT_H_
#define MODBASEINSTRUMENT_H_

#include <Base/BaseName.h>
#include <Instrument/VoiceManager.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Instrument
    {
        /**
        this class implements the basic interface for an instrument
        it uses a voice-manager to play multiple notes simultaneously
        */
        class BaseInstrument : public Base::BaseName
        {
        public:
            BaseInstrument() :
                Base::BaseName("BaseInstrument")
            {
                voiceManager = new VoiceManager();
            }

            virtual ~BaseInstrument()
            {
                if (voiceManager)
                    delete voiceManager;
                voiceManager = NULL;
            }

            virtual bool StartNote(UInt8 Note, UInt8 Velocity, UInt16 Mode)
            {
                if (voiceManager)
                    voiceManager->StartNote(Note, Velocity, Mode);
                return true;
            }

            virtual bool StopNote(UInt8 Note)
            {
                if (voiceManager)
                    voiceManager->StopNote(Note);
                return true;
            }

            virtual void Render(float** inputs, float **outputs, UInt16 sampleFrames)
            {
                if (voiceManager)
                    voiceManager->Render(inputs, outputs, sampleFrames);
            }

            void SetVoices(BaseVoice** Voices, UInt8 NumVoices)
            {
                voiceManager->SetVoices(Voices, NumVoices);
            }

        protected:
            VoiceManager* voiceManager;
        };
    }
}

#endif
