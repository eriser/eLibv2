#include "gtest/gtest.h"
#include <MIDI/modMidiEventHandler.h>

using namespace eLibV2::MIDI;

TEST(TestMidiHandler, hasEventsOnChannel)
{
    MidiEventHandler handler;
    handler.insertEvent(0, MidiEvent(65, 100));

    ASSERT_TRUE(handler.hasEventsOnChannel(0));
}

TEST(TestMidiHandler, hasNoEventsOnChannel)
{
    MidiEventHandler handler;
    handler.insertEvent(0, MidiEvent(65, 100));

    ASSERT_FALSE(handler.hasEventsOnChannel(1));
}

TEST(TestMidiHandler, hasEventsOnAnyChannel)
{
    MidiEventHandler handler;
    handler.insertEvent(0, MidiEvent(65, 100));

    ASSERT_TRUE(handler.hasEventsOnAnyChannel());
}

TEST(TestMidiHandler, insertEvent)
{
    MidiEventHandler handler;
    handler.insertEvent(0, MidiEvent(65, 100));

    MidiEventVector events = handler.getEvents(0);
    ASSERT_EQ(1, events.size());
    ASSERT_EQ(65, events[0].getNote());
    ASSERT_EQ(100, events[0].getVelocity());
}

TEST(TestMidiHandler, deleteEvent)
{
    MidiEventHandler handler;
    handler.insertEvent(0, MidiEvent(65, 100));

    ASSERT_TRUE(handler.hasEventsOnChannel(0));
    handler.deleteEvent(0, MidiEvent(65, 100));
    ASSERT_FALSE(handler.hasEventsOnChannel(0));
}

TEST(TestMidiHandler, deleteEventSuccessful)
{
    MidiEventHandler handler;
    handler.insertEvent(0, MidiEvent(65, 100));

    ASSERT_TRUE(handler.deleteEvent(0, MidiEvent(65, 100)));
}

TEST(TestMidiHandler, deleteEventFailure)
{
    MidiEventHandler handler;
    handler.insertEvent(0, MidiEvent(65, 100));

    ASSERT_FALSE(handler.deleteEvent(0, MidiEvent(66, 100)));
}
