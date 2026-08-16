#pragma once

class QImage;
class QSize;
class QString;

namespace Plumbum::ui
{
    QString DecodeQRCode(const QImage &img);
    QImage EncodeQRCode(const QString content, int size);
} // namespace Plumbum::ui
using namespace Plumbum::ui;
