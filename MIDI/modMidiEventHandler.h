#ifndef MODMIDIEVENTHANDLER_H_
#define MODMIDIEVENTHANDLER_H_

#include <BASE/modBaseModule.h>
#include <MIDI/modMidiEvent.h>
#include <Util/modLogger.h>

namespace eLibV2
{
    namespace MIDI
    {
        /**
        handle incoming MIDI-Events
        */
        class MidiEventHandler : public Base::BaseModule
        {
        public:
            enum { MAX_MIDI_CHANNELS = 16 };

        public:
            MidiEventHandler() : Base::BaseName("MidiEventHandler") { Init(); }
            ~MidiEventHandler();

            /**
            insert event for given channel
            @param channel the channel to process
            @param event to insert
            */
            void insertEvent(int channel, MidiEvent event);

            /**
            delete event in given channel
            @param channel the channel to process
            @param event to delete
            @return true if event was successfully deleted
            */
            bool deleteEvent(int channel, MidiEvent event);

            /**
            get all events currently bound to given channel
            @param channel the channel to process
            @return vector of all events for given channel
            */
            MidiEventVector getEvents(int channel);

            /**
            check if channel has currently any events
            @param channel the channel to process
            @return true if any events are currently present
            */
            bool hasEventsOnChannel(int channel);

            /**
            check if there are events on any channel
            @return true if any channel has events
            */
            bool hasEventsOnAnyChannel();

            void Init();

        private:
            MidiEventVector *mMidiEvents[MAX_MIDI_CHANNELS]; ///< events for all channels
        };
    }
}

#endif
