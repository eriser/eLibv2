#include <Loader/PresetLoader.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace eLibV2::Loader;

SInt16 PresetLoader::Load(const std::string& filename)
{
    size_t found;
    std::string Filetype;
    SInt16 ret;

    if (!filename.size())
        return ERR_PRESET_NO_FILENAME;

    found = filename.rfind('.');
    if (found == std::string::npos)
        return ERR_PRESET_INVALID_FILENAME;

    Filetype = filename.substr(found);

    Bank.Programs.clear();

    if ((Filetype == ".txt") || (Filetype == ".TXT"))
        ret = LoadTxtFile(filename);
    else if ((Filetype == ".fxp") || (Filetype == ".FXP"))
        ret = LoadFxpFile(filename);
    else if ((Filetype == ".fxb") || (Filetype == ".FXB"))
        ret = LoadFxbFile(filename);
    else
        ret = ERR_PRESET_UNKNOWN_TYPE;
    return ret;
}

SInt16 PresetLoader::Save(const std::string& filename)
{
    char tempstr[12];

    OutFile.open(filename.c_str(), std::ifstream::out | std::ifstream::binary);
    if (!OutFile.good())
        return ERR_PRESET_FILE_OPEN;

    if (Bank.NumPrograms)
    {
        OutFile << "#define PRESET_NUM " << Bank.NumPrograms << std::endl;
        OutFile << "#define PARAM_NUM " << Bank.Programs.at(0).Params.size() << std::endl << std::endl;

        // output programnames
        OutFile << "static char preset_names[PRESET_NUM][kVstMaxProgNameLen] = {";
        for (SInt16 ProgramIndex = 0; ProgramIndex < Bank.NumPrograms; ProgramIndex++)
        {
            // new line and indent
            if ((ProgramIndex % MAX_NAMES_PER_LINE) == 0)
                OutFile << std::endl << "    ";

            OutFile << "{\"" << Bank.Programs.at(ProgramIndex).ProgramName << "\"}";
            if (ProgramIndex < (Bank.NumPrograms - 1))
                OutFile << ",";
        }
        OutFile << std::endl << "};" << std::endl << std::endl;

        // output program values
        OutFile << "double preset_data[PRESET_NUM][PARAM_NUM] = {" << std::endl;
        for (SInt16 ProgramIndex = 0; ProgramIndex < Bank.NumPrograms; ProgramIndex++)
        {
            OutFile << "{";
            for (UInt16 ParamIndex = 0; ParamIndex < Bank.Programs.at(ProgramIndex).Params.size(); ParamIndex++)
            {
                if ((ParamIndex % MAX_PARAMS_PER_LINE) == 0)
                {
                    sprintf(tempstr, "%02i", ParamIndex);
                    OutFile << std::endl << "/* " << tempstr << " */";
                }

                sprintf(tempstr, "%f", Bank.Programs.at(ProgramIndex).Params.at(ParamIndex));
                OutFile << " " << tempstr;
                if (ParamIndex < (Bank.Programs.at(ProgramIndex).Params.size() - 1))
                    OutFile << ", ";
            }
            OutFile << std::endl << "}";
            if (ProgramIndex < (Bank.NumPrograms - 1))
                OutFile << ",";
            OutFile << std::endl;
        }
        OutFile << std::endl << "};" << std::endl;
    }
    OutFile.close();
    return ERR_PRESET_NO_ERROR;
}

SInt16 PresetLoader::LoadTxtFile(const std::string& Filename)
{
    InFile.open(Filename.c_str(), std::ifstream::in);
    if (!InFile.good())
        return ERR_PRESET_FILE_OPEN;

#if 0

input:

    PARAMS=100
    SECTION BEGIN
    VALUE=1
    VALUE=1
    VALUE=1
    VALUE=1
    SECTION END
    SECTION BEGIN
    VALUE=1
    VALUE=1
    VALUE=1
    VALUE=1
    SECTION END


    while (!feof(Infile))
    {
        fgets(buffer, 128, Infile);
        name = strtok(buffer, "=\n");
        value = strtok(NULL, "=\n");
        if ((name != NULL) && (value != NULL))
        {
            if (strcmp(name, "PARAMS") == 0)
                paramCount = atoi(value);
            else if ((strcmp(name, "SECTION") == 0) && (strcmp(value, "BEGIN") == 0))
            {
                paramNum = 0;
                if (presetIndex != 0)
                    fprintf(Outfile, ",");
                fprintf(Outfile, "\n/* Preset %02i */\n{", presetIndex);
            }
            else if ((strcmp(name, "SECTION") == 0) && (strcmp(value, "END") == 0))
            {
                fprintf(Outfile, "\n}");
                presetIndex++;
            }
            else if (strcmp(name, "VALUE") == 0)
            {
                if ((paramNum % MAX_PARAMS_LINE) == 0)
                {
                    cr = '\n';
                    sprintf(index, "/* %02i */ ", paramNum);
                }
                else
                {
                    cr = ' ';
                    sprintf(index, "");
                }

                if (paramNum < paramCount - 1)
                    strcpy(comma, ", ");
                else
                    strcpy(comma, "");

                fprintf(Outfile, "%c%s%s%s", cr, index, value, comma);
                paramNum++;
            }
        }
    }
    fclose(Outfile);
#endif

    InFile.close();
    return 0;
}

SInt16 PresetLoader::ReadProgram(void)
{
    fxProgram fxpProgram;
    OutputProgram Program;
    SInt32 tempval;
    flint tempdata;
    std::stringstream ss;

    InFile.read((char*)&fxpProgram.chunkMagic, sizeof(fxpProgram.chunkMagic));
    if (strncmp((char*)&fxpProgram.chunkMagic, MAGIC_CHUNK, 4))
        return ERR_PRESET_INVALID_FXP_FILE;

    InFile.read((char*)&tempval, sizeof(tempval));
    fxpProgram.byteSize = SwapBytes(tempval);
    InFile.read((char*)&fxpProgram.fxMagic, sizeof(fxpProgram.fxMagic));

    if (strncmp((char*)&fxpProgram.fxMagic, MAGIC_PROGRAM_CHUNK, 4) == 0)
        return ERR_PRESET_FXP_CHUNK;
    else if (strncmp((char*)&fxpProgram.fxMagic, MAGIC_PROGRAM, 4))
        return ERR_PRESET_FXP_RECORD;

    InFile.read((char*)&tempval, sizeof(tempval));
    fxpProgram.version = SwapBytes(tempval);
    InFile.read((char*)&fxpProgram.fxID, sizeof(fxpProgram.fxID));
    InFile.read((char*)&tempval, sizeof(tempval));
    fxpProgram.fxVersion = SwapBytes(tempval);

    ss << getModuleName() << ": converting for Plugin: [ID: " << fxpProgram.fxID[0] << fxpProgram.fxID[1] << fxpProgram.fxID[2] << fxpProgram.fxID[3];
    ss << " Version: " << fxpProgram.fxVersion << "]" << std::endl;
    ModuleLogger::print(LOG_CLASS_LOADER, ss.str().c_str());

    InFile.read((char*)&tempval, sizeof(tempval));
    fxpProgram.numParams = SwapBytes(tempval);
    InFile.read((char*)&fxpProgram.prgName, sizeof(fxpProgram.prgName));

    if ((fxpProgram.byteSize) && (fxpProgram.byteSize != (fxpProgram.numParams * 4 + 48)))
    {
        ss.clear();
        ss << "expected " << fxpProgram.numParams * 4 + 48 << " bytes, got " << fxpProgram.byteSize << std::endl;
        ModuleLogger::print(LOG_CLASS_LOADER, ss.str().c_str());

        return ERR_PRESET_FXP_CORRUPTED;
    }

    // begin output format
    Program.ProgramName.assign(fxpProgram.prgName);
    ss.clear();
    ss << getModuleName() << ": Reading " << fxpProgram.numParams <<  " parameters for program '" <<  fxpProgram.prgName << "'" << std::endl;
    ModuleLogger::print(LOG_CLASS_LOADER, ss.str().c_str());

    for (SInt16 ii = 0; ii < fxpProgram.numParams; ii++)
    {
        InFile.read((char*)&tempval, sizeof(tempdata));
        tempdata.i = SwapBytes(tempval);
        Program.Params.push_back(tempdata.f);
    }
    Bank.Programs.push_back(Program);
    return ERR_PRESET_NO_ERROR;
}

SInt16 PresetLoader::LoadFxpFile(const std::string& Filename)
{
    SInt16 ret = 0;

    InFile.open(Filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (!InFile.good())
        return ERR_PRESET_FILE_OPEN;

    ret = ReadProgram();
    InFile.close();

    Bank.NumPrograms = Bank.Programs.size();
    return ret;
}

SInt16 PresetLoader::LoadFxbFile(const std::string& Filename)
{
    SInt16 ret = 0;
    SInt32 tempval;
    fxBank fxbBank;
    std::stringstream ss;

    InFile.open(Filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (!InFile.good())
        return ERR_PRESET_FILE_OPEN;

    InFile.read((char*)&fxbBank.chunkMagic, sizeof(fxbBank.chunkMagic));
    if (strncmp((char*)&fxbBank.chunkMagic, MAGIC_CHUNK, 4))
        return ERR_PRESET_INVALID_FXP_FILE;

    InFile.read((char*)&tempval, sizeof(tempval));
    fxbBank.byteSize = SwapBytes(tempval);
    InFile.read((char*)&fxbBank.fxMagic, sizeof(fxbBank.fxMagic));
    if (strncmp((char*)&fxbBank.fxMagic, MAGIC_BANK_CHUNK, 4) == 0)
        return ERR_PRESET_FXP_CHUNK;
    else if (strncmp((char*)&fxbBank.fxMagic, MAGIC_BANK, 4))
        return ERR_PRESET_FXP_RECORD;

    InFile.read((char*)&tempval, sizeof(tempval));
    fxbBank.version = SwapBytes(tempval);
    InFile.read((char*)&fxbBank.fxID, sizeof(fxbBank.fxID));
    InFile.read((char*)&tempval, sizeof(tempval));
    fxbBank.fxVersion = SwapBytes(tempval);

    ss << getModuleName() << ": converting for Plugin: [ID: " << fxbBank.fxID[0] << fxbBank.fxID[1] << fxbBank.fxID[2] << fxbBank.fxID[3] << " Version: " << fxbBank.fxVersion << "]" << std::endl;
    ModuleLogger::print(LOG_CLASS_LOADER, ss.str().c_str());

    InFile.read((char*)&tempval, sizeof(tempval));
    fxbBank.numPrograms = SwapBytes(tempval);
    InFile.read((char*)&fxbBank.future, sizeof(char) * 64);
    InFile.read((char*)&fxbBank.future, sizeof(char) * 64);

    ss.clear();
    ss << getModuleName() << ": Reading " << fxbBank.numPrograms <<  " programs" << std::endl;
    ModuleLogger::print(LOG_CLASS_LOADER, ss.str().c_str());

    for (SInt32 ProgramIndex = 0; ProgramIndex < fxbBank.numPrograms; ProgramIndex++)
    {
        ret = ReadProgram();
        if (ret != ERR_PRESET_NO_ERROR)
            break;
    }
    InFile.close();

    Bank.NumPrograms = Bank.Programs.size();
    return ret;
}
