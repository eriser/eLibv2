# source files.
SRC = \
	Generator/NoiseGenerator.cpp \
	Generator/Wavetable.cpp \
	Generator/BaseOsc.cpp \
	Generator/Sequencer.cpp \
	Generator/BaseLFO.cpp \
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
	Util/FrequencyTable.cpp \
	Util/DataSampler.cpp \
	Util/Logger.cpp \
	Util/ManagedBuffer.cpp \
	Util/ParamRandom.cpp \
	MIDI/MidiDevice.cpp \
	MIDI/MidiEventAdapter.cpp \
	MIDI/MidiEventHandler.cpp \
	Envelope/EnvelopeDADSR.cpp \
	Data/EditorProperties.cpp \
	Data/PluginParameter.cpp \
	Data/PluginProgram.cpp \
	Loader/PresetLoader.cpp \
	Loader/WaveLoader.cpp \
	Loader/LPSLoader.cpp

OBJ = $(SRC:.cpp=.o)
 
LIB_A = libeLibv2.a
LIB_SO = libeLibv2.so
BUILDTEST_A = buildtest_a
BUILDTEST_SO = buildtest_so
MIDITEST_LINUX = miditest_linux
MIDITEST_MACOS = miditest_macos

# include directories
INCLUDES = -I. -I/usr/local/include

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g -Wall

# compiler
CCC = g++

# compile flags
# c and m for standard
# z for zlib
LDFLAGS = -L/usr/lib -L/usr/local/lib -lc -lm -lz

.SUFFIXES: .cpp

default: $(LIB_A) $(LIB_SO) $(BUILDTEST_A) $(BUILDTEST_SO) $(MIDITEST_LINUX) $(MIDITEST_MACOS)

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(LIB_A): $(OBJ)
	ar rcs $(LIB_A) $(OBJ)

$(BUILDTEST_A): $(LIB_A) buildtest.cpp
	$(CCC) $(INCLUDES) $(CCFLAGS) buildtest.cpp libeLibv2.a $(LDFLAGS) -o $(BUILDTEST_A)

$(LIB_SO): $(SRC)
	$(CCC) $(INCLUDES) $(CCFLAGS) -shared -fPIC $(SRC) $(LDFLAGS) -o $(LIB_SO)

$(BUILDTEST_SO): $(LIB_SO) buildtest.cpp
	$(CCC) $(INCLUDES) $(CCFLAGS) buildtest.cpp -L. -leLibv2 -o $(BUILDTEST_SO)

$(MIDITEST_LINUX): miditest_linux.cpp
	$(CCC) $(INCLUDES) $(CCFLAGS) miditest_linux.cpp -o $(MIDITEST_LINUX)

$(MIDITEST_MACOS): miditest_macos.cpp
	$(CCC) $(INCLUDES) $(CCFLAGS) miditest_macos.cpp -o $(MIDITEST_MACOS) -framework CoreMIDI -framework CoreServices

#depend: dep
# 
#	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f $(OBJ) $(LIB_A) $(LIB_SO) $(BUILDTEST_A) $(BUILDTEST_SO) $(MITEST_LINUX) $(MIDITEST_MACOS) Makefile.bak
