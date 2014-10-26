/*
 * modePresetLoader.cpp
 *
 *  Created on: 28.11.2011
 *      Author: dedokter
 */

#include <Loader/modPresetLoader.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace eLibV2;
using namespace std;

PresetLoader::PresetLoader()
{
    Init();
}

void PresetLoader::Init()
{
    setModuleName("PresetLoader");
}

int PresetLoader::Load(string filename)
{
    size_t found;
    string Filetype;
    int ret;

    if (!filename.size())
        return ERR_PRESET_NO_FILENAME;

    found = filename.rfind('.');
    if (found == string::npos)
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

int PresetLoader::Save(string filename)
{
    char tempstr[12];

    OutFile.open(filename.c_str(), ifstream::out | ifstream::binary);
    if (!OutFile.good())
        return ERR_PRESET_FILE_OPEN;

    if (Bank.NumPrograms)
    {
        OutFile << "#define PRESET_NUM " << Bank.NumPrograms << endl;
        OutFile << "#define PARAM_NUM " << Bank.Programs.at(0).Params.size() << endl << endl;

        // output programnames
        OutFile << "static char preset_names[PRESET_NUM][kVstMaxProgNameLen] = {";
        for (int ProgramIndex = 0; ProgramIndex < Bank.NumPrograms; ProgramIndex++)
        {
            // new line and indent
            if ((ProgramIndex % MAX_NAMES_PER_LINE) == 0)
                OutFile << endl << "    ";

            OutFile << "{\"" << Bank.Programs.at(ProgramIndex).ProgramName << "\"}";
            if (ProgramIndex < (Bank.NumPrograms - 1))
                OutFile << ",";
        }
        OutFile << endl << "};" << endl << endl;

        // output program values
        OutFile << "double preset_data[PRESET_NUM][PARAM_NUM] = {" << endl;
        for (int ProgramIndex = 0; ProgramIndex < Bank.NumPrograms; ProgramIndex++)
        {
            OutFile << "{";
            for (unsigned int ParamIndex = 0; ParamIndex < Bank.Programs.at(ProgramIndex).Params.size(); ParamIndex++)
            {
                if ((ParamIndex % MAX_PARAMS_PER_LINE) == 0)
                {
                    sprintf(tempstr, "%02i", ParamIndex);
                    OutFile << endl << "/* " << tempstr << " */";
                }

                sprintf(tempstr, "%f", Bank.Programs.at(ProgramIndex).Params.at(ParamIndex));
                OutFile << " " << tempstr;
                if (ParamIndex < (Bank.Programs.at(ProgramIndex).Params.size() - 1))
                    OutFile << ", ";
            }
            OutFile << endl << "}";
            if (ProgramIndex < (Bank.NumPrograms - 1))
                OutFile << ",";
            OutFile << endl;
        }
        OutFile << endl << "};" << endl;

    }

    OutFile.close();
    return ERR_PRESET_NO_ERROR;
}

int PresetLoader::LoadTxtFile(string Filename)
{
    InFile.open(Filename.c_str(), ifstream::in);
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

int PresetLoader::ReadProgram(void)
{
    fxProgram fxpProgram;
    OutputProgram Program;
    VstInt32 tempval;
    flint tempdata;

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
    cout << getModuleName() << ": converting for Plugin: [ID: " << fxpProgram.fxID[0] << fxpProgram.fxID[1] << fxpProgram.fxID[2] << fxpProgram.fxID[3] << " Version: " << fxpProgram.fxVersion << "]" << endl;

    InFile.read((char*)&tempval, sizeof(tempval));
    fxpProgram.numParams = SwapBytes(tempval);
    InFile.read((char*)&fxpProgram.prgName, sizeof(fxpProgram.prgName));

    if ((fxpProgram.byteSize) && (fxpProgram.byteSize != (fxpProgram.numParams * 4 + 48)))
    {
        cout << "expected " << fxpProgram.numParams * 4 + 48 << " bytes, got " << fxpProgram.byteSize << endl;
        return ERR_PRESET_FXP_CORRUPTED;
    }

    // begin output format
    Program.ProgramName.assign(fxpProgram.prgName);
    cout << getModuleName() << ": Reading " << fxpProgram.numParams <<  " parameters for program '" <<  fxpProgram.prgName << "'" << endl;

    for (int ii = 0; ii < fxpProgram.numParams; ii++)
    {
        InFile.read((char*)&tempval, sizeof(tempdata));
        tempdata.i = SwapBytes(tempval);
        Program.Params.push_back(tempdata.f);
    }
    Bank.Programs.push_back(Program);
    return ERR_PRESET_NO_ERROR;
}

int PresetLoader::LoadFxpFile(string Filename)
{
    int ret = 0;

    InFile.open(Filename.c_str(), ifstream::in | ifstream::binary);
    if (!InFile.good())
        return ERR_PRESET_FILE_OPEN;

    ret = ReadProgram();
    InFile.close();

    Bank.NumPrograms = Bank.Programs.size();
    return ret;
}

int PresetLoader::LoadFxbFile(string Filename)
{
    int ret = 0;
    VstInt32 tempval;
    fxBank fxbBank;

    InFile.open(Filename.c_str(), ifstream::in | ifstream::binary);
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
    cout << getModuleName() << ": converting for Plugin: [ID: " << fxbBank.fxID[0] << fxbBank.fxID[1] << fxbBank.fxID[2] << fxbBank.fxID[3] << " Version: " << fxbBank.fxVersion << "]" << endl;

    InFile.read((char*)&tempval, sizeof(tempval));
    fxbBank.numPrograms = SwapBytes(tempval);
    InFile.read((char*)&fxbBank.future, sizeof(char) * 64);
    InFile.read((char*)&fxbBank.future, sizeof(char) * 64);

    cout << getModuleName() << ": Reading " << fxbBank.numPrograms <<  " programs" << endl;
    for (VstInt32 ProgramIndex = 0; ProgramIndex < fxbBank.numPrograms; ProgramIndex++)
    {
        ret = ReadProgram();
        if (ret)
            break;
    }
    InFile.close();

    Bank.NumPrograms = Bank.Programs.size();
    return ret;
}
