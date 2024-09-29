#ifndef WHITEBOARDSETTINGS_H
#define WHITEBOARDSETTINGS_H

#include <QSettings>

namespace ADEV
{

// 单例模式
class WhiteBoardSettings : public QSettings
{
    Q_OBJECT
private:
    WhiteBoardSettings();

public:
    static WhiteBoardSettings& getInstance();
};

} // end of ADEV

#endif // WHITEBOARDSETTINGS_H
