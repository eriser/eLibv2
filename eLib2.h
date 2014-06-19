/*
 * eLib2.h
 *
 *  Created on: 25.01.2012
 *      Author: dedokter
 */

#ifndef __ELIB2_H__
#define __ELBI2_H__

// loader modules
#include "modLPSLoader.h"
#include "modPresetLoader.h"
#include "modWaveLoader.h"

// generator modules
#include "modBaseLFO.h"
#include "modBaseOsc.h"
#include "modNoiseGenerator.h"
#include "modSequencer.h"
#include "modWavetable.h"

// envelope modules
#include "modEnvelopeDADSR.h"

// effect modules
#include "mod3BandEQ.h"
#include "modClip.h"
#include "modDither.h"
#include "modEnvFollower.h"
#include "modMixer.h"
#include "modOverdrive.h"
#include "modSignalAdd.h"
#include "modSimpleComp.h"
#include "modVCF.h"
#include "modWaveWrap.h"

// midi event handling
#include "modMidiEvent.h"
#include "modMidiEventHandler.h"

// helper modules
#include "modLogger.h"
#include "modParamRandom.h"
#include "modTypes.h"

// VSTBase
#include "VSTBase/VSTBaseClass.h"
#include "VSTBase/VSTBaseProgram.h"
#include "VSTBase/VSTBaseProperties.h"

#endif /* __ELIB2_H__ */
