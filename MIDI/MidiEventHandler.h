#ifndef MODMIDIEVENTHANDLER_H_
#define MODMIDIEVENTHANDLER_H_

#include <BASE/BaseModule.h>
#include <MIDI/MidiEvent.h>
#include <Util/Logger.h>

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

            /**
            insert event for given channel
            @param channel the channel to process
            @param event to insert
            */
            void insertEvent(const int channel, const MidiEvent& event);

            /**
            delete event in given channel
            @param channel the channel to process
            @param event to delete
            @return true if event was successfully deleted
            */
            bool deleteEvent(const int channel, const MidiEvent& event);

            /**
            get all events currently bound to given channel
            @param channel the channel to process
            @return vector of all events for given channel
            */
            MidiEventVector getEvents(const int channel) const;

            /**
            check if channel has currently any events
            @param channel the channel to process
            @return true if any events are currently present
            */
            bool hasEventsOnChannel(const int channel) const;

            /**
            check if there are events on any channel
            @return true if any channel has events
            */
            bool hasEventsOnAnyChannel() const;

            void Init() {}

        private:
            bool channelInRange(const int channel) const { return ((channel >= 0) && (channel < MAX_MIDI_CHANNELS)); }

            MidiEventVector mMidiEvents[MAX_MIDI_CHANNELS]; ///< events for all channels
        };
    }
}

#endif
