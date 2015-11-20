#ifndef MODBASEVOICE_H_
#define MODBASEVOICE_H_

#include <Base/BaseName.h>
#include <Util/Logger.h>
#include <Util/Defines.h>

#include <map>
#include <sstream>
#include <memory.h>

using namespace eLibV2::Util;

namespace eLibV2
{
    namespace Base
    {
        /**
        this module provides a base voice which can be used to produce polyphonic output
        every note is used to create a single voice which maintains all processing itself
        each voice is designed to be processed with in its own thread
        */
        class BaseVoice : virtual public BaseName // virtual is needed for initialisation in subclasses
        {
        public:
            BaseVoice() :
                BaseName("BaseVoice")
            {
            }

            virtual ~BaseVoice() {}

            virtual void PlayNote(UInt8 Note, UInt16 Mode)
            {
                m_iNote = Note;
                m_iMode = Mode;
            }

            /**
            this method processes the voice and delivers the output to the main mix
            @param inputs the inputs used to process external signals
            @param outputs outputs receiving the data for the main mix
            @param sampleFrames number of frames to process
            */
            virtual void Render(double** inputs, double **outputs, UInt16 sampleFrames) = 0;

        private:
            UInt8 m_iNote;
            UInt16 m_iMode;
        };
    }
}

#endif
