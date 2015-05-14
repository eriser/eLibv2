#include "AsioDevice.h"

using namespace eLibV2::ASIO;

// asio.cpp uses this explicitly as external reference (bad design)
AsioDrivers *asioDrivers;

extern AsioLoader::DriverInfo asioDriverInfo;

AsioDevice::AsioDevice()
    : m_uiNumAsioDevices(0),
    m_AsioLoader(NULL)
{
    asioDrivers = new AsioDrivers();
    m_AsioLoader = new AsioLoader(asioDrivers);

    EnumerateDevices();
}

AsioDevice::~AsioDevice()
{
    if (asioDrivers)
    {
        delete asioDrivers;
        asioDrivers = NULL;
    }

    if (m_AsioLoader)
    {
        delete m_AsioLoader;
        m_AsioLoader = NULL;
    }
}

void AsioDevice::EnumerateDevices()
{
    char *drivernames[MAX_ASIO_DEVICE_NUM];
    for (int i = 0; i < MAX_ASIO_DEVICE_NUM; i++)
    {
        drivernames[i] = (char*)malloc(MAX_ASIO_DEVICE_NAME);
        memset(drivernames[i], 0, MAX_ASIO_DEVICE_NAME);
    }
    m_uiNumAsioDevices = asioDrivers->getDriverNames((char**)&drivernames, MAX_ASIO_DEVICE_NUM);

    std::cout << "found " << m_uiNumAsioDevices << " ASIO drivers in your system. Please select one for output." << std::endl;
    for (int i = 0; i < m_uiNumAsioDevices; i++)
    {
        m_DeviceNames.push_back(drivernames[i]);
        free(drivernames[i]);
    }
}

std::string AsioDevice::GetDeviceName(unsigned int deviceIndex)
{
    if (deviceIndex < m_DeviceNames.size())
        return m_DeviceNames[deviceIndex];
    else
        return std::string("not found");
}

bool AsioDevice::OpenDevice(int driverIndex)
{
    ModuleLogger::enable();
    ModuleLogger::setConsole();

    bool bRes = false;
    // load the driver, this will setup all the necessary internal data structures
    // since asio low-level-methods can't handle const char arguments
    if (m_AsioLoader->LoadDriver(const_cast<char*>(m_DeviceNames[driverIndex].c_str())))
    {
        // initialize the driver
        if (ASIOInit(&asioDriverInfo.driverInfo) == ASE_OK)
        {
            ModuleLogger::print(
                "asioVersion:   %d\n"
                "driverVersion: %d\n"
                "Name:          %s\n"
                "ErrorMessage:  %s\n",
                asioDriverInfo.driverInfo.asioVersion,
                asioDriverInfo.driverInfo.driverVersion,
                asioDriverInfo.driverInfo.name,
                asioDriverInfo.driverInfo.errorMessage);

            if (m_AsioLoader->Init(&asioDriverInfo) == 0)
            {
                // ASIOControlPanel(); you might want to check wether the ASIOControlPanel() can open
                //ASIOControlPanel();

                if (m_AsioLoader->CreateBuffers(&asioDriverInfo) == ASE_OK)
                {
                    if (ASIOStart() == ASE_OK)
                        bRes = true;
                    else
                        ModuleLogger::print("ASIOStart failed");
                }
                else
                    ModuleLogger::print("CreateBuffers failed");
            }
            else
                ModuleLogger::print("Init failed");
        }
        else
            ModuleLogger::print("ASIOInit failed");
    }
    else
        ModuleLogger::print("LoadDriver failed");
    return bRes;
}

void AsioDevice::CloseDevice()
{
    ASIOStop();
    ASIODisposeBuffers();
    ASIOExit();
    asioDrivers->removeCurrentDriver();
}

