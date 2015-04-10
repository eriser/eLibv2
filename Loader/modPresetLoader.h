#ifndef MODPRESETLOADER_H_
#define MODPRESETLOADER_H_

#include <Base/modBaseModule.h>
#include <Util/modTypes.h>

#include <vector>
#include <fstream>

#define MAGIC_CHUNK         "CcnK"
#define MAGIC_PROGRAM       "FxCk"
#define MAGIC_PROGRAM_CHUNK "FPCh"
#define MAGIC_BANK          "FxBk"
#define MAGIC_BANK_CHUNK    "FBCh"

namespace eLibV2
{
    class PresetLoader : public BaseModule
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
            VstInt32 byteSize;      ///< size of this chunk, excl. magic + byteSize

            char fxMagic[4];        ///< 'FxCk' (regular) or 'FPCh' (opaque chunk)
            VstInt32 version;       ///< format version (currently 1)
            char fxID[4];           ///< fx unique ID
            VstInt32 fxVersion;     ///< fx version

            VstInt32 numParams;     ///< number of parameters
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
            VstInt32 byteSize;          ///< size of this chunk, excl. magic + byteSize

            char fxMagic[4];            ///< 'FxBk' (regular) or 'FBCh' (opaque chunk)
            VstInt32 version;           ///< format version (1 or 2)
            char fxID[4];               ///< fx unique ID
            VstInt32 fxVersion;         ///< fx version

            VstInt32 numPrograms;       ///< number of programs

#if VST_2_4_EXTENSIONS
            VstInt32 currentProgram;    ///< version 2: current program number
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
            int NumPrograms;
            std::vector<OutputProgram> Programs;
        } OutputBank;

    public:
        PresetLoader() : BaseName("PresetLoader") {}

        void Init(void) {}

        int Load(std::string filename);
        int Save(std::string filename);

    private:
        VstInt32 SwapBytes(VstInt32 input)
        {
            VstInt32 temp = 0;
            temp = ((input & 0xFF000000) >> 24) + (((input & 0x00FF0000) >> 16) << 8) + (((input & 0x0000FF00) >> 8) << 16) + ((input & 0x000000FF) << 24);

            return temp;
        }

        int LoadTxtFile(std::string Filename);
        int LoadFxpFile(std::string Filename);
        int LoadFxbFile(std::string Filename);
        int ReadProgram(void);

    private:
        std::ifstream InFile;
        std::ofstream OutFile;
        OutputBank Bank;
    };
}

#endif