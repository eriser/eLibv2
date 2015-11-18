# source files.
SRC =  	Generator/NoiseGenerator.cpp \
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
 
OUTSTATIC = libeLibv2.a
OUTDYNAMIC = libeLibv2.so
BUILDTEST = buildtest

# include directories
INCLUDES = -I. -I../include/ -I/usr/local/include
 
# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g

# compiler
CCC = g++

# compile flags
LDFLAGS = -L/usr/lib -L/usr/local/lib -lc -lm -lz

.SUFFIXES: .cpp
 
default: $(OUTSTATIC) $(OUTDYNAMIC) $(BUILDTEST)
 
.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@
 
$(OUTSTATIC): $(OBJ)
	ar rcs $(OUTSTATIC) $(OBJ)

$(OUTDYNAMIC): $(SRC)
	g++ $(INCLUDES) -shared -fPIC -o $(OUTDYNAMIC) $(SRC) $(LDFLAGS)

$(BUILDTEST): $(OUTDYNAMIC)
	g++ $(INCLUDES) buildtest.cpp -o buildtest -L. -leLibv2

#depend: dep
# 
#	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)
 
clean:
	rm -f $(OBJ) $(OUTSTATIC) $(OUTDYNAMIC) Makefile.bak
