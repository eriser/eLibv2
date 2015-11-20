#ifndef MODPRESETLOADER_H_
#define MODPRESETLOADER_H_

#include <Base/BaseModule.h>

#include <vector>
#include <fstream>

#define MAGIC_CHUNK         "CcnK"
#define MAGIC_PROGRAM       "FxCk"
#define MAGIC_PROGRAM_CHUNK "FPCh"
#define MAGIC_BANK          "FxBk"
#define MAGIC_BANK_CHUNK    "FBCh"

namespace eLibV2
{
    namespace Loader
    {
        class PresetLoader : public Base::BaseName
        {
        public:
            enum
            {
                MAX_NAMES_PER_LINE = 8,
                MAX_PARAMS_PER_LINE = 12
            };

            enum
            {
                ERR_PRESET_NO_ERROR = 0,
                ERR_PRESET_NO_FILENAME,
                ERR_PRESET_INVALID_FILENAME,
                ERR_PRESET_UNKNOWN_TYPE,
                ERR_PRESET_FILE_OPEN,
                ERR_PRESET_INVALID_FXP_FILE,
                ERR_PRESET_FXP_CORRUPTED,
                ERR_PRESET_FXP_CHUNK,
                ERR_PRESET_FXP_RECORD
            };

            //-------------------------------------------------------------------------------------------------------
            /** Program (fxp) structure. */
            //-------------------------------------------------------------------------------------------------------
            struct fxProgram
            {
                //-------------------------------------------------------------------------------------------------------
                char chunkMagic[4];     ///< 'CcnK'
                SInt32 byteSize;      ///< size of this chunk, excl. magic + byteSize

                char fxMagic[4];        ///< 'FxCk' (regular) or 'FPCh' (opaque chunk)
                SInt32 version;       ///< format version (currently 1)
                char fxID[4];           ///< fx unique ID
                SInt32 fxVersion;     ///< fx version

                SInt32 numParams;     ///< number of parameters
                char prgName[28];       ///< program name (null-terminated ASCII string)
                //-------------------------------------------------------------------------------------------------------
            };

            //-------------------------------------------------------------------------------------------------------
            /** Bank (fxb) structure. */
            //-------------------------------------------------------------------------------------------------------
            struct fxBank
            {
                //-------------------------------------------------------------------------------------------------------
                char chunkMagic[4];         ///< 'CcnK'
                SInt32 byteSize;          ///< size of this chunk, excl. magic + byteSize

                char fxMagic[4];            ///< 'FxBk' (regular) or 'FBCh' (opaque chunk)
                SInt32 version;           ///< format version (1 or 2)
                char fxID[4];               ///< fx unique ID
                SInt32 fxVersion;         ///< fx version

                SInt32 numPrograms;       ///< number of programs

#if VST_2_4_EXTENSIONS
                SInt32 currentProgram;    ///< version 2: current program number
                char future[124];           ///< reserved, should be zero
#else
                char future[128];           ///< reserved, should be zero
#endif
                //-------------------------------------------------------------------------------------------------------
            };

            typedef struct
            {
                std::string ProgramName;
                std::vector<float> Params;
            } OutputProgram;

            typedef struct
            {
                SInt16 NumPrograms;
                std::vector<OutputProgram> Programs;
            } OutputBank;

        public:
            PresetLoader() :
                Base::BaseName("PresetLoader")
            {
            }

            SInt16 Load(const std::string& filename);
            SInt16 Save(const std::string& filename);

        private:
            SInt32 SwapBytes(SInt32 input)
            {
                SInt32 temp = 0;
                temp = ((input & 0xFF000000) >> 24) + (((input & 0x00FF0000) >> 16) << 8) + (((input & 0x0000FF00) >> 8) << 16) + ((input & 0x000000FF) << 24);

                return temp;
            }

            SInt16 LoadTxtFile(const std::string& Filename);
            SInt16 LoadFxpFile(const std::string& Filename);
            SInt16 LoadFxbFile(const std::string& Filename);
            SInt16 ReadProgram(void);

        private:
            std::ifstream InFile;
            std::ofstream OutFile;
            OutputBank Bank;
        };
    }
}

#endif