#include <Loader/modLPSLoader.h>

LPSLoader::~LPSLoader()
{
    Unload();
}

void LPSLoader::Init()
{
    FileLoaded = false;
    for (int i = 0; i < LPS_MAXTRACKS; i++)
    {
        TrackData[i] = 0;
        LPSTracks[i] = 0;
    }
}

void LPSLoader::Unload(void)
{
    for (int i = 0; i < LPS_MAXTRACKS; i++)
    {
        if (TrackData[i])
        {
            delete TrackData[i];
            TrackData[i] = 0;
        }

        if (LPSTracks[i])
        {
            delete LPSTracks[i];
            LPSTracks[i] = 0;
        }
    }
    FileLoaded = false;
}

int LPSLoader::Load(std::string filename)
{
    std::ifstream lpsfile;
    unsigned long numread, tracknum, NumRemain, NumPages;
    char msg[1024];
    BYTE helper[LPS_MAXREADBUF];
    
    // cleanup previously used memory
    Unload();

	lpsfile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (!lpsfile.good())
    {
        sprintf(msg, "file '%s' not found\nplease reload manually", filename.c_str());
        dbgOutput(msg);
        return LPS_ERROR_FILENOTFOUND;
    }
    tracknum = 0;
    lpsfile.read((char*)&LPSFile.Magic, sizeof(LPSFile.Magic));
    if (strncmp((char*)&LPSFile.Magic, LPS_MAGIC, 4))
    {
        dbgOutput((char*)"not a LPS-File");
        lpsfile.close();
        return LPS_ERROR_FILETYPE;
    }

    lpsfile.read((char*)&LPSFile.Version, sizeof(LPSFile.Version));
#if LPS_DEBUG_MODE
    sprintf(debug_temp, "Version: %li.%02li", (LPSFile.Version & 0xFF00) >> 8, LPSFile.Version & 0x00FF);
    dbgOutput(debug_temp);
#endif

    lpsfile.read((char*)&LPSFile.Name, sizeof(LPSFile.Name));
#if LPS_DEBUG_MODE
    sprintf(debug_temp, "Name: '%s'", (char*)&LPSFile.Name);
    dbgOutput(debug_temp);
#endif

    lpsfile.read((char*)&LPSFile.BPM, sizeof(LPSFile.BPM));
#if LPS_DEBUG_MODE
    sprintf(debug_temp, "BPM: %i", LPSFile.BPM);
    dbgOutput(debug_temp);
#endif

    lpsfile.read((char*)&LPSFile.MaxLengthTracks, sizeof(LPSFile.MaxLengthTracks));
    // divide by 2 for mono tracks
    LPSFile.MaxLengthTracks /= 2;
#if LPS_DEBUG_MODE
    sprintf(debug_temp, "Length: %li", LPSFile.MaxLengthTracks);
    dbgOutput(debug_temp);
#endif

    lpsfile.read((char*)&LPSFile.NumTracks, sizeof(LPSFile.NumTracks));
#if LPS_DEBUG_MODE
    sprintf(debug_temp, "Num: %li", LPSFile.NumTracks);
    dbgOutput(debug_temp);
#endif

    for (tracknum = 0; tracknum < LPSFile.NumTracks; tracknum++)
    {
        LPSTracks[tracknum] = new lpsData;
        if (!LPSTracks[tracknum])
        {
            dbgOutput((char*)"Error allocating memory for Track");
            return LPS_ERROR_MEMORY_TRACK;
        }
        LPSFile.TrackData[tracknum] = LPSTracks[tracknum];
        TrackData[tracknum] = new double[LPSFile.MaxLengthTracks];
        if (!TrackData[tracknum])
        {
            dbgOutput((char*)"Error allocation memory for Trackdata");
            return LPS_ERROR_MEMORY_TRACKDATA;
        }
        LPSFile.TrackData[tracknum]->Data = TrackData[tracknum];
        memset(TrackData[tracknum], 0, sizeof(double) * LPSFile.MaxLengthTracks);
        lpsfile.read((char*)&LPSTracks[tracknum]->Name, sizeof(LPSTracks[tracknum]->Name));
        lpsfile.read((char*)&LPSTracks[tracknum]->LengthCompressed, sizeof(LPSTracks[tracknum]->LengthCompressed));

        // divide by 2 for mono tracks
        if (LPSFile.Version >= 0x0101)
        {
            lpsfile.read((char*)&LPSTracks[tracknum]->LengthUnCompressed, sizeof(LPSTracks[tracknum]->LengthUnCompressed));
            LPSTracks[tracknum]->LengthUnCompressed /= 2;
        }
        else
            LPSTracks[tracknum]->LengthUnCompressed = LPSFile.MaxLengthTracks;

#if LPS_DEBUG_MODE
        sprintf(debug_temp, "Name: %s Ptr: %p %p %p Len: %li %li", LPSTracks[tracknum]->Name, LPSTracks[tracknum], TrackData[tracknum], helper, LPSTracks[tracknum]->LengthCompressed, LPSTracks[tracknum]->LengthUnCompressed);
        dbgOutput(debug_temp);
#endif
        /* allocate temp buffers */
        TrackCompressed = new BYTE[LPSTracks[tracknum]->LengthCompressed];
        LengthUncompressed = LPSTracks[tracknum]->LengthUnCompressed * 2;
        TempBuffer = new BYTE[LengthUncompressed];

        /* fast readout of file */
        NumRemain = LPSTracks[tracknum]->LengthCompressed % LPS_MAXREADBUF;
        NumPages = (LPSTracks[tracknum]->LengthCompressed - NumRemain) / LPS_MAXREADBUF;
        for (numread = 0; numread < NumPages; numread++)
        {
            lpsfile.read((char*)helper, sizeof(BYTE) * LPS_MAXREADBUF);
            memcpy(&TrackCompressed[numread * LPS_MAXREADBUF], helper, LPS_MAXREADBUF);
        }
        if (NumRemain)
        {
            lpsfile.read((char*)helper, sizeof(BYTE) * NumRemain);
            memcpy(&TrackCompressed[NumPages * LPS_MAXREADBUF], helper, NumRemain);
        }

        /* uncompress file data */
        uncompress(TempBuffer, &LengthUncompressed, TrackCompressed, LPSTracks[tracknum]->LengthCompressed);
        /* convert and copy temp buffer to destination */
        SSHORT help = 0;
        for (numread = 0; numread < LengthUncompressed / 2; numread++)
        {
            help = (TempBuffer[numread * 2 + 1] << 8) + TempBuffer[numread * 2];
            LPSFile.TrackData[tracknum]->Data[numread] = ((double)(help)) / 0x10000;
        }

        /* cleanup buffers */
        if (TrackCompressed)
            delete TrackCompressed;
        TrackCompressed = 0;
        if (TempBuffer)
            delete TempBuffer;
        TempBuffer = 0;
    }
    lpsfile.close();

#if LPS_DEBUG_MODE
    sprintf(debug_temp, "File '%s' successfully loaded", filename.c_str());
    dbgOutput(debug_temp);
#endif

    FileLoaded = true;
    return LPS_ERROR_NONE;
}

void LPSLoader::getName(char *name)
{
    if (FileLoaded)
        sprintf(name, "%s", LPSFile.Name);
    else
        strcpy(name, "");
}

USHORT LPSLoader::getBPM(void)
{
    if (FileLoaded)
        return LPSFile.BPM;
    else
        return 0;
}

ULONG LPSLoader::getMaxLengthTracks(void)
{
    if (FileLoaded)
        return LPSFile.MaxLengthTracks;
    else
        return 0;
}

USHORT LPSLoader::getNumTracks(void)
{
    if (FileLoaded)
        return LPSFile.NumTracks;
    else
        return 0;
}

void LPSLoader::getTrackName(char *name, VstInt16 TrackIndex)
{
    if (FileLoaded)
        sprintf(name, "%s", LPSFile.TrackData[TrackIndex]->Name);
    else
        strcpy(name, "");
}

ULONG LPSLoader::getTrackSize(VstInt16 TrackIndex)
{
    if (FileLoaded)
        return LPSFile.TrackData[TrackIndex]->LengthUnCompressed;
    else
        return 0;
}

double *LPSLoader::getTrackPointer(VstInt16 TrackIndex)
{
    if (FileLoaded)
        return LPSFile.TrackData[TrackIndex]->Data;
    else
        return 0;
}

double LPSLoader::getTrackData(VstInt16 TrackIndex, VstInt16 DataIndex)
{
    if (FileLoaded)
        return LPSFile.TrackData[TrackIndex]->Data[DataIndex];
    else
        return 0;
}
