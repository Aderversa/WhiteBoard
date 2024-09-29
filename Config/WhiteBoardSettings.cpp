#include "Config/WhiteBoardSettings.h"

#include <QFile>
#include <QCoreApplication>

namespace ADEV
{

WhiteBoardSettings::WhiteBoardSettings()
    : QSettings(QString(QCoreApplication::applicationDirPath() + "WhiteBoard_Config.ini"), QSettings::IniFormat)
{
}

WhiteBoardSettings& WhiteBoardSettings::getInstance()
{
    static WhiteBoardSettings instance;
    return instance;
}

} // end of ADEV
