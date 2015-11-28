#ifndef MODBASEVOICE_H_
#define MODBASEVOICE_H_

// TODO: how to handle parameter-changes which will affect the modules within the voice?
namespace eLibV2
{
    namespace Instrument
    {
        /**
        this module provides a base voice which can be used to produce polyphonic output
        every note is used to create a single voice which maintains all processing itself
        each voice is designed to be processed with in its own thread
        */
        class BaseVoice
        {
        public:
            enum eVoiceState
            {
                UNDEF = 0,
                STARTED,
                STOP_REQUESTED,
                STOPPED
            };

        public:
            BaseVoice()
            {
                m_eState = STOPPED;
            }

            virtual ~BaseVoice() {}

            /**
            start the voice and rendering
            @param Note note to play
            @param Velocity velocity to use
            @param Mode additional information about how to play the note
            @return true if voice has started
            */
            virtual bool Start(UInt8 Note, UInt8 Velocity, UInt16 Mode)
            {
                bool bRes = false;

                if (m_eState == STOPPED)
                {
                    m_iNote = Note;
                    m_iVelocity = Velocity;
                    m_iMode = Mode;
                    m_eState = STARTED;
                    bRes = true;
                }
                return bRes;
            }

            /**
            request the voice to stop. this will usually trigger the release phase of an envelope
            */
            virtual void RequestStop(void) { m_eState = STOP_REQUESTED; }

            /**
            check if voice has stopped
            @return true if voice has stopped
            */
            virtual bool isStopped(void) { return (m_eState == STOPPED); }

            UInt8 GetNote(void) const { return m_iNote; }
            UInt8 GetVelocity(void) const { return m_iVelocity; }

            virtual void ParameterChanged(UInt32 Index, double Value) = 0;

            /**
            this method processes the voice and delivers the output to the main mix
            - the outputs should be accumulated
            - when the state reaches STOP_REQUESTED end the voice
            - when the voice has ended, set state to STOPPED
            @param inputs the inputs used to process external signals
            @param outputs outputs receiving the data for the main mix
            @param sampleFrames number of frames to process
            */
            virtual void Render(float** inputs, float **outputs, UInt16 sampleFrames) = 0;

        protected:
            /**
            stop the voice and signal the voicemanager it can be cleared
            */
            virtual void Stop(void)
            {
                m_iNote = 0;
                m_iVelocity = 0;
                m_iMode = 0;
                m_eState = STOP_REQUESTED;
            }

        protected:
            UInt8 m_iNote;
            UInt8 m_iVelocity;
            UInt16 m_iMode;
            eVoiceState m_eState;
        };
    }
}

#endif
