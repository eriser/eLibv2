# source files.
SRC = \
	Data/EditorProperties.cpp \
	Data/PluginParameter.cpp \
	Data/PluginProgram.cpp \
	Effect/VCF.cpp \
	Effect/TremoloPanner.cpp \
	Effect/WaveShaper.cpp \
	Effect/RMSCalc.cpp \
	Effect/SignalAdd.cpp \
	Effect/EnvFollower.cpp \
	Effect/Overdrive.cpp \
	Effect/Clip.cpp \
	Effect/SelectorBlock.cpp \
	Effect/Delay.cpp \
	Effect/Flanger.cpp \
	Effect/SimpleComp.cpp \
	Effect/Mixer.cpp \
	Effect/EQ3Band.cpp \
	Effect/Dither.cpp \
	Effect/DynamicsProcessor.cpp \
	Effect/MultiTapDelay.cpp \
	Effect/CrossedFeedbackDelay.cpp \
	Effect/EnvelopeDetector.cpp \
	Effect/WaveWrap.cpp \
	Effect/Phaser.cpp \
	Effect/Resample.cpp \
	Effect/RoomReverb.cpp \
	Envelope/ADSR.cpp \
	Envelope/AR.cpp \
	Envelope/EnvelopeDADSR.cpp \
	Generator/NoiseGenerator.cpp \
	Generator/Wavetable.cpp \
	Generator/BaseOsc.cpp \
	Generator/Sequencer.cpp \
	Generator/BaseLFO.cpp \
	Loader/PresetLoader.cpp \
	Loader/WaveLoader.cpp \
	Loader/LPSLoader.cpp \
	MIDI/BaseMidiDevice.cpp \
	MIDI/MidiDeviceFactory.cpp \
	MIDI/MidiEventAdapter.cpp \
	MIDI/MidiEventHandler.cpp \
	MIDI/Platform/MidiDeviceLinux.cpp \
	MIDI/Platform/MidiDeviceMac.cpp \
	MIDI/Platform/MidiDeviceWin.cpp \
	Util/FrequencyTable.cpp \
	Util/DataSampler.cpp \
	Util/Logger.cpp \
	Util/ManagedBuffer.cpp \
	Util/ParamRandom.cpp

OBJ = $(SRC:.cpp=.o)
 
LIB_A = libeLibv2.a
LIB_SO = libeLibv2.so
BUILDTEST_A = buildtest_a
BUILDTEST_SO = buildtest_so
MIDITEST_A = miditest_a
MIDITEST_SO = miditest_so

# include directories
INCLUDES = -I. -I/usr/local/include

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g -Wall

# compiler
CCC = g++

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LIBS_PLATFORM = -lpthread -lasound
endif
ifeq ($(UNAME), Darwin)
LIBS_PLATFORM = -framework CoreMidi -framework CoreServices
endif

# compile flags
# c and m for standard
# z for zlib
LIBS_PATH = -L/usr/lib -L/usr/local/lib
LIBS_STANDARD = -lc -lm -lz

.SUFFIXES: .cpp

default: $(LIB_A) $(LIB_SO) $(BUILDTEST_A) $(BUILDTEST_SO) $(MIDITEST_A) $(MIDITEST_SO)

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(LIB_A): $(OBJ)
	ar rcs $(LIB_A) $(OBJ)

$(BUILDTEST_A): $(LIB_A) buildtest.cpp
	$(CCC) $(INCLUDES) $(CCFLAGS) buildtest.cpp libeLibv2.a $(LIBS_PATH) $(LIBS_STANDARD) -o $(BUILDTEST_A)

$(LIB_SO): $(SRC)
	$(CCC) $(INCLUDES) $(CCFLAGS) -shared -fPIC $(SRC) $(LIBS_PATH) $(LIBS_STANDARD) $(LIBS_PLATFORM) -o $(LIB_SO)

$(BUILDTEST_SO): $(LIB_SO) buildtest.cpp
	$(CCC) $(INCLUDES) $(CCFLAGS) buildtest.cpp -L. -leLibv2 -o $(BUILDTEST_SO)

$(MIDITEST_A): miditest.cpp $(LIB_A)
	$(CCC) $(INCLUDES) $(CCFLAGS) miditest.cpp ./libeLibv2.a $(LIBS_PLATFORM) -o $(MIDITEST_A)

$(MIDITEST_SO): miditest.cpp
	$(CCC) $(INCLUDES) $(CCFLAGS) miditest.cpp -L. -leLibv2 $(LIBS_PLATFORM) -o $(MIDITEST_SO)

#depend: dep
# 
#	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f $(OBJ) $(LIB_A) $(LIB_SO) $(BUILDTEST_A) $(BUILDTEST_SO) $(MIDITEST_A) $(MIDITEST_SO) Makefile.bak
