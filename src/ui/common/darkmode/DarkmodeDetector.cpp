#include "DarkmodeDetector.hpp"

#include "base/PlumbumBase.hpp"

#include <QApplication>
#include <QStyle>

namespace Plumbum::components::darkmode
{
    // Referenced from github.com/keepassxreboot/keepassxc. Licensed under GPL2/3.
    // Copyright (C) 2020 KeePassXC Team <team@keepassxc.org>
    bool isDarkMode()
    {
        if (!qApp || !qApp->style())
        {
            return false;
        }
        return qApp->style()->standardPalette().color(QPalette::Window).toHsl().lightness() < 110;
    }

} // namespace Plumbum::components::darkmode
