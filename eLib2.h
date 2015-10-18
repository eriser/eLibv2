#ifndef __ELIB2_H__
#define __ELBI2_H__

// block modules
#include <Connection/AdderConnection.h>
#include <Connection/ConnectionPool.h>
#include <Connection/ConstantConnection.h>
#include <Connection/InputConnection.h>
#include <Connection/OutputConnection.h>
#include <Connection/MultiplierConection.h>
#include <Connection/ScalerConnection.h>

// data types
#include <Data/PluginParameter.h>
#include <Data/PluginProgram.h>

// loader modules
#include <Loader/EditorParameterLoader.h>
#include <Loader/LPSLoader.h>
#include <Loader/PluginParameterLoader.h>
#include <Loader/PresetLoader.h>
#include <Loader/WaveLoader.h>

// generator modules
#include <Generator/BaseLFO.h>
#include <Generator/BaseOsc.h>
#include <Generator/NoiseGenerator.h>
#include <Generator/Sequencer.h>
#include <Generator/Wavetable.h>

// envelope modules
#include <Envelope/EnvelopeDADSR.h>

// effect modules
#include <Effect/Clip.h>
#include <Effect/CrossedFeedbackDelay.h>
#include <Effect/Delay.h>
#include <Effect/Dither.h>
#include <Effect/DynamicsProcessor.h>
#include <Effect/EnvelopeDetector.h>
#include <Effect/EnvFollower.h>
#include <Effect/EQ3Band.h>
#include <Effect/Flanger.h>
#include <Effect/Mixer.h>
#include <Effect/MultiTapDelay.h>
#include <Effect/Overdrive.h>
#include <Effect/Phaser.h>
#include <Effect/Resample.h>
#include <Effect/RMSCalc.h>
#include <Effect/SelectorBlock.h>
#include <Effect/SignalAdd.h>
#include <Effect/SimpleComp.h>
#include <Effect/VCF.h>
#include <Effect/WaveWrap.h>

// filter modules
#include <Filter/ButterworthBPF.h>
#include <Filter/ButterworthBSF.h>
#include <Filter/ButterworthHPF.h>
#include <Filter/ButterworthLPF.h>
#include <Filter/CascadedEQConstantQ.h>
#include <Filter/HiShelving.h>
#include <Filter/LoShelving.h>
#include <Filter/LinkwitzRileyHPF.h>
#include <Filter/LinkwitzRileyLPF.h>
#include <Filter/ParametricConstantQ.h>
#include <Filter/ResonantBPF.h>
#include <Filter/ResonantBSF.h>
#include <Filter/ResonantHPF.h>
#include <Filter/ResonantLPF.h>
#include <Filter/SwitchableAPF.h>

// midi event handling
#include <MIDI/MidiEvent.h>
#include <MIDI/MidiEventHandler.h>
#include <MIDI/MidiEventAdapter.h>

// helper modules
#include <Util/DataSampler.h>
#include <Util/Logger.h>
#include <Util/ParamRandom.h>
#include <Util/Types.h>

// VSTBase
#include <VSTBase/VSTBaseBitmapManager.h>
#include <VSTBase/VSTBaseClass.h>
#include <VSTBase/VSTBaseEditor.h>

#endif
