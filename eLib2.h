#ifndef __ELIB2_H__
#define __ELBI2_H__

// block modules
#include <Blocks/modConstantConnector.h>
#include <Blocks/modInputConnector.h>
#include <Blocks/modOutputConnector.h>
#include <Blocks/modScalingConnector.h>

// loader modules
#include <Loader/modLPSLoader.h>
#include <Loader/modPresetLoader.h>
#include <Loader/modWaveLoader.h>

// generator modules
#include <Generator/modBaseLFO.h>
#include <Generator/modBaseOsc.h>
#include <Generator/modNoiseGenerator.h>
#include <Generator/modSequencer.h>
#include <Generator/modWavetable.h>

// envelope modules
#include <Envelope/modEnvelopeDADSR.h>

// effect modules
#include <Effect/mod3BandEQ.h>
#include <Effect/modClip.h>
#include <Effect/modDither.h>
#include <Effect/modEnvFollower.h>
#include <Effect/modMixer.h>
#include <Effect/modOverdrive.h>
#include <Effect/modSignalAdd.h>
#include <Effect/modSimpleComp.h>
#include <Effect/modVCF.h>
#include <Effect/modWaveWrap.h>

// midi event handling
#include <MIDI/modMidiEvent.h>
#include <MIDI/modMidiEventHandler.h>
#include <MIDI/modMidiEventAdapter.h>

// helper modules
#include <Util/modLogger.h>
#include <Util/modParamRandom.h>
#include <Util/modTypes.h>
#include <Util/modTestCase.h>

// VSTBase
#include <VSTBase/VSTBaseBitmapManager.h>
#include <VSTBase/VSTBaseClass.h>
#include <VSTBase/VSTBaseEditor.h>
#include <VSTBase/VSTBaseParameter.h>
#include <VSTBase/VSTBaseProgram.h>
#include <VSTBase/VSTBaseProperties.h>

#endif