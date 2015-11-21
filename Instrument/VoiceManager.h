#ifndef MODVOICEMANAGER_H_
#define MODVOICEMANAGER_H_

#include <Base/BaseName.h>
#include <Instrument/BaseVoice.h>

#include <memory.h>
#include <time.h>

namespace eLibV2
{
    namespace Instrument
    {
        /**
        this module manages all active voices and handles incoming midi-events
        dynamic voice allocation and stealing is done here
        */
        class VoiceManager : public Base::BaseName
        {
        public:
            enum StealingStrategy
            {
                NONE = 0,
                OLDEST,
                QUIETEST
            };

        public:
            VoiceManager() :
                Base::BaseName("VoiceManager")
            {
                m_iNumVoices = 0;
                m_ppFreeVoices = NULL;
                m_pUsedVoices = NULL;
                m_eStealingStrategy = NONE;
            }
            ~VoiceManager() {}

            /**
            initialize with an array of available voices, ready allocated
            @param Voices the pointer to the array of available voices
            @param NumVoices number of reserved voices
            */
            void SetVoices(BaseVoice** Voices, const UInt8 NumVoices)
            {
                m_iNumVoices = NumVoices;
                m_ppFreeVoices = new BaseVoice*[m_iNumVoices];

                for (UInt8 VoiceIndex = 0; VoiceIndex < m_iNumVoices; ++VoiceIndex)
                    m_ppFreeVoices[VoiceIndex] = Voices[VoiceIndex];

                m_pUsedVoices = new sUsedVoice[m_iNumVoices];
                memset(m_pUsedVoices, 0, sizeof(sUsedVoice) * m_iNumVoices);
            }

            /**
            get a free voice and start a new note
            the following strategy is used to find a suitable voice:
            - free voices are preferred
            - when there are no free voices
              - take oldest triggered note
              - take note with least velocity
            @param Note the note to be played
            @param Velocity velocity of the note
            @param Mode special modes
            @return true if free voice was found
            */
            bool StartNote(UInt8 Note, UInt8 Velocity, UInt16 Mode)
            {
                bool bRes = false;

                if (m_iNumVoices)
                {
                    BaseVoice* FreeVoice = GetFreeVoice();

                    if (!FreeVoice)
                        FreeVoice = StealVoice();

                    // found voice -> start it and store into used-voices
                    if (FreeVoice)
                    {
                        bRes = FreeVoice->Start(Note, Velocity, Mode);
                        UseVoice(FreeVoice);
                    }
                }
                return bRes;
            }

            /**
            stop the voice with the previously started note
            @param Note the note to be played
            @return true if the voice was found
            */
            bool StopNote(UInt8 Note)
            {
                bool bRes = false;

                if (m_iNumVoices)
                {
                    // search for corresponding note
                    // TODO this might not be the right way to do it
                    for (UInt8 VoiceIndex = 0; VoiceIndex < m_iNumVoices; ++VoiceIndex)
                    {
                        if (m_pUsedVoices[VoiceIndex].Voice == NULL)
                            continue;

                        if (m_pUsedVoices[VoiceIndex].Voice->GetNote() == Note)
                        {
                            m_pUsedVoices[VoiceIndex].Voice->RequestStop();
                            bRes = true;
                            break;
                        }
                    }
                }
                return bRes;
            }

            void SetStealingStrategy(StealingStrategy Strategy) { m_eStealingStrategy = Strategy; }

            // TODO: this should be split over multiple threads
            void Render(float** inputs, float** outputs, UInt16 sampleFrames)
            {
                if (m_iNumVoices)
                {
                    for (UInt8 VoiceIndex = 0; VoiceIndex < m_iNumVoices; ++VoiceIndex)
                    {
                        if (m_pUsedVoices[VoiceIndex].Voice == NULL)
                            continue;

                        // first check if voice has stopped
                        if (m_pUsedVoices[VoiceIndex].Voice->isStopped())
                        {
                            // move back to free-voices-array
                            FreeVoice(VoiceIndex);
                        }
                        // every voice should add its outputs
                        // TODO this won't work if working with threads
                        else
                            m_pUsedVoices[VoiceIndex].Voice->Render(inputs, outputs, sampleFrames);
                    }
                }
            }

        private:
            /**
            get a free voice or steal it when none is available
            @return the free/stolen voice or NULL
            */
            BaseVoice* GetFreeVoice(void)
            {
                BaseVoice* FoundVoice = NULL;
                for (UInt8 VoiceIndex = 0; VoiceIndex < m_iNumVoices; ++VoiceIndex)
                {
                    if (m_ppFreeVoices[VoiceIndex] != NULL)
                    {
                        // get voice and erase from array
                        FoundVoice = m_ppFreeVoices[VoiceIndex];
                        m_ppFreeVoices[VoiceIndex] = NULL;
                        break;
                    }
                }
                return FoundVoice;
            }

            /**
            steal voice using set strategy
            @return stolen voice or NULL
            */
            BaseVoice* StealVoice(void)
            {
                BaseVoice* StolenVoice = NULL;
                time_t SuitableTimeStamp = 0;
                UInt8 SuitableVelocity = 0;

                // only steal if applicable
                if (m_eStealingStrategy != NONE)
                {
                    for (UInt8 VoiceIndex = 0; VoiceIndex < m_iNumVoices; ++VoiceIndex)
                    {
                        // leave out any empty voices
                        if (m_pUsedVoices[VoiceIndex].Voice == NULL)
                            continue;

                        switch (m_eStealingStrategy)
                        {
                            case OLDEST:
                                // get timestamp of first voice and compare to others
                                if (SuitableTimeStamp == 0)
                                {
                                    SuitableTimeStamp = m_pUsedVoices[VoiceIndex].StartTimeStamp;
                                    StolenVoice = m_pUsedVoices[VoiceIndex].Voice;
                                }
                                else
                                {
                                    if (SuitableTimeStamp > m_pUsedVoices[VoiceIndex].StartTimeStamp)
                                    {
                                        SuitableTimeStamp = m_pUsedVoices[VoiceIndex].StartTimeStamp;
                                        StolenVoice = m_pUsedVoices[VoiceIndex].Voice;
                                    }
                                }
                                break;

                            case QUIETEST:
                                // get velocity of first voice and compare to others
                                if (SuitableVelocity == 0)
                                {
                                    SuitableVelocity = m_pUsedVoices[VoiceIndex].Voice->GetVelocity();
                                    StolenVoice = m_pUsedVoices[VoiceIndex].Voice;
                                }
                                else
                                {
                                    if (SuitableVelocity > m_pUsedVoices[VoiceIndex].Voice->GetVelocity())
                                    {
                                        SuitableVelocity = m_pUsedVoices[VoiceIndex].Voice->GetVelocity();
                                        StolenVoice = m_pUsedVoices[VoiceIndex].Voice;
                                    }
                                }
                                break;
                        }
                    }
                }
                return StolenVoice;
            }

            void UseVoice(BaseVoice* Voice)
            {
                for (UInt8 VoiceIndex = 0; VoiceIndex < m_iNumVoices; ++VoiceIndex)
                {
                    if (m_pUsedVoices[VoiceIndex].Voice == NULL)
                    {
                        m_pUsedVoices[VoiceIndex].Voice = Voice;
                        time(&m_pUsedVoices[VoiceIndex].StartTimeStamp);
                        break;
                    }
                }
            }

            /**
            put back voice into free-voice-array
            */
            void FreeVoice(UInt8 StoppedIndex)
            {
                for (UInt8 VoiceIndex = 0; VoiceIndex < m_iNumVoices; ++VoiceIndex)
                {
                    if (m_ppFreeVoices[VoiceIndex] == NULL)
                    {
                        m_ppFreeVoices[VoiceIndex] = m_pUsedVoices[StoppedIndex].Voice;
                        m_pUsedVoices[StoppedIndex].Voice = NULL;
                        m_pUsedVoices[StoppedIndex].StartTimeStamp = 0;
                        break;
                    }
                }
            }

        private:
            typedef struct
            {
                time_t StartTimeStamp;
                BaseVoice* Voice;
            } sUsedVoice;

            UInt8 m_iNumVoices;
            BaseVoice** m_ppFreeVoices;
            sUsedVoice* m_pUsedVoices;
            StealingStrategy m_eStealingStrategy;
        };
    }
}

#endif