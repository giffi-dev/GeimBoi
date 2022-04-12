#ifdef _WIN32
    #include <direct.h>
    #define get_current_dir_name() _getcwd(NULL,0)
#elif __unix__
    #include <unistd.h>
#endif
#include <utils/Benchmark.hpp>
#include "appSettings.hpp"

using namespace Giffi;

void appSettings::SetLastRomPath(const std::string& path)
{
    Get().m_IniFile.SetValue("General", "LastRomPath", path);
}

std::string appSettings::GetLastRomPath()
{
    if (!Get().m_IniFile.HasValue("General", "LastRomPath"))
    {
        return get_current_dir_name();
    }
    return Get().m_IniFile.GetValue<std::string>("General", "LastRomPath");
}

appSettings& appSettings::Get()
{
    static appSettings p;
    return p;
}

appSettings::appSettings()
    : m_IniFile(iniFilePath) {}

appSettings::~appSettings()
{
    m_IniFile.SaveFile(iniFilePath);
}
