#ifndef MODMIDIEVENTHANDLER_H_
#define MODMIDIEVENTHANDLER_H_

#include <Base/BaseModule.h>
#include <MIDI/MidiEvent.h>
#include <Util/Logger.h>

namespace eLibV2
{
    namespace MIDI
    {
        /**
        handle incoming MIDI-Events
        */
        class MidiEventHandler : public Base::BaseName
        {
        public:
            MidiEventHandler() :
                Base::BaseName("MidiEventHandler")
            {
                Init();
            }

            /**
            insert event for given channel
            @param channel the channel to process
            @param event to insert
            */
            void insertEvent(const SInt16 channel, const MidiEvent& event);

            /**
            delete event in given channel
            @param channel the channel to process
            @param event to delete
            @return true if event was successfully deleted
            */
            bool deleteEvent(const SInt16 channel, const MidiEvent& event);

            /**
            get all events currently bound to given channel
            @param channel the channel to process
            @param events a vector filled with all found events
            @return true if events were found on given channel
            */
            bool getEvents(const SInt16 channel, MidiEventVector& events) const;

            /**
            check if channel has currently any events
            @param channel the channel to process
            @return true if any events are currently present
            */
            bool hasEventsOnChannel(const SInt16 channel) const;

            /**
            check if there are events on any channel
            @return true if any channel has events
            */
            bool hasEventsOnAnyChannel() const;

            void Init(void) {}

        private:
            bool channelInRange(const SInt16 channel) const { return ((channel >= MIDI_CHANNEL_MIN) && (channel < MIDI_CHANNEL_MAX)); }

            MidiEventVector mMidiEvents[MIDI_CHANNEL_MAX]; ///< events for all channels
        };
    }
}

#endif
