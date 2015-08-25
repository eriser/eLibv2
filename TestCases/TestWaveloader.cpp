#include "gtest/gtest.h"
#include <Loader/WaveLoader.h>

#include "TestGlobals.h"

using namespace eLibV2::Loader;

TEST(TestWaveLoader, testwave_8bit_stereo)
{
    WaveLoader loader;
    WaveLoader::fmtChunk format;
    ASSERT_EQ(loader.Load(TestDataFolder + "testwave_8bit_stereo.wav"), WaveLoader::WAVE_ERROR_NO_ERROR);
    format = loader.getWaveFormat();
    ASSERT_EQ(format.BitsPerSample, 8);
    ASSERT_EQ(format.Compression, WaveLoader::WAVE_COMPR_PCM);
    ASSERT_EQ(format.NumChannels, 2);
    ASSERT_EQ(format.SampleRate, 44100);
}

TEST(TestWaveLoader, testwave_16bit_stereo)
{
    WaveLoader loader;
    WaveLoader::fmtChunk format;
    ASSERT_EQ(loader.Load(TestDataFolder + "testwave_16bit_stereo.wav"), WaveLoader::WAVE_ERROR_NO_ERROR);
    format = loader.getWaveFormat();
    ASSERT_EQ(format.BitsPerSample, 16);
    ASSERT_EQ(format.Compression, WaveLoader::WAVE_COMPR_PCM);
    ASSERT_EQ(format.NumChannels, 2);
    ASSERT_EQ(format.SampleRate, 44100);
}

TEST(TestWaveLoader, testwave_24bit_mono)
{
    WaveLoader loader;
    WaveLoader::fmtChunk format;
    ASSERT_EQ(loader.Load(TestDataFolder + "testwave_24bit_mono.wav"), WaveLoader::WAVE_ERROR_NO_ERROR);
    format = loader.getWaveFormat();
    ASSERT_EQ(format.BitsPerSample, 24);
    ASSERT_EQ(format.Compression, WaveLoader::WAVE_COMPR_PCM);
    ASSERT_EQ(format.NumChannels, 1);
    ASSERT_EQ(format.SampleRate, 44100);
}

TEST(TestWaveLoader, testwave_24bit_stereo)
{
    WaveLoader loader;
    WaveLoader::fmtChunk format;
    ASSERT_EQ(loader.Load(TestDataFolder + "testwave_24bit_stereo.wav"), WaveLoader::WAVE_ERROR_NO_ERROR);
    format = loader.getWaveFormat();
    ASSERT_EQ(format.BitsPerSample, 24);
    ASSERT_EQ(format.Compression, WaveLoader::WAVE_COMPR_PCM);
    ASSERT_EQ(format.NumChannels, 2);
    ASSERT_EQ(format.SampleRate, 44100);
}

TEST(TestWaveLoader, testwave_32bit_stereo)
{
    WaveLoader loader;
    WaveLoader::fmtChunk format;
    ASSERT_EQ(loader.Load(TestDataFolder + "testwave_32bit_stereo.wav"), WaveLoader::WAVE_ERROR_NO_ERROR);
    format = loader.getWaveFormat();
    ASSERT_EQ(format.BitsPerSample, 32);
    ASSERT_EQ(format.Compression, WaveLoader::WAVE_COMPR_PCM);
    ASSERT_EQ(format.NumChannels, 2);
    ASSERT_EQ(format.SampleRate, 44100);
}

TEST(TestWaveLoader, testwave_float_stereo)
{
    WaveLoader loader;
    WaveLoader::fmtChunk format;
    ASSERT_EQ(loader.Load(TestDataFolder + "testwave_float_stereo.wav"), WaveLoader::WAVE_ERROR_NO_ERROR);
    format = loader.getWaveFormat();
    ASSERT_EQ(format.BitsPerSample, 32);
    ASSERT_EQ(format.Compression, WaveLoader::WAVE_COMPR_IEEE);
    ASSERT_EQ(format.NumChannels, 2);
    ASSERT_EQ(format.SampleRate, 44100);
}

TEST(TestWaveLoader, testwave_double_stereo)
{
    WaveLoader loader;
    WaveLoader::fmtChunk format;
    ASSERT_EQ(loader.Load(TestDataFolder + "testwave_double_stereo.wav"), WaveLoader::WAVE_ERROR_NO_ERROR);
    format = loader.getWaveFormat();
    ASSERT_EQ(format.BitsPerSample, 64);
    ASSERT_EQ(format.Compression, WaveLoader::WAVE_COMPR_IEEE);
    ASSERT_EQ(format.NumChannels, 2);
    ASSERT_EQ(format.SampleRate, 44100);
}
