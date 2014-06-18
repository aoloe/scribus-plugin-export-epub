#include <QDebug>

EpubExportImage::EpubExportImage()
{
}

EpubExportImage::~EpubExportImage()
{
}

QDebug operator<<(QDebug dbg, const EpubExportImage &image)
{
    dbg.nospace() << "(" << "Debug not implemented" << ")";
    return dbg.space();
}

