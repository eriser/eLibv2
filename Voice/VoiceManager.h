#ifndef MODBASEVOICE_H_
#define MODBASEVOICE_H_

#include <Base/BaseName.h>
#include <Voice/BaseVoice.h>

#include <vector>

namespace eLibV2
{
    namespace Voice
    {
        /**
        this module manages all active voices and handles incoming midi-events
        dynamic voice allocation and stealing is done here
        */
        class VoiceManager : virtual public Base::BaseName // virtual is needed for initialisation in subclasses
        {
        public:
            VoiceManager(std::vector<BaseVoice>) :
                Base::BaseName("VoiceManager")
            {
            }

            virtual ~VoiceManager() {}

        private:

        };
    }
}

#endif
