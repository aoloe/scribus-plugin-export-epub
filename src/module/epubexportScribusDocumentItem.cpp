#include <QDebug>

#include "module/epubexportScribusDocumentItem.h"

EpubExportScribusDocumentItem::EpubExportScribusDocumentItem()
{
}

EpubExportScribusDocumentItem::~EpubExportScribusDocumentItem()
{
}

/**
 * TODO: as soon as other write directions are to be considered the order has to be made more flexible
 */
bool EpubExportScribusDocumentItem::isBefore(EpubExportScribusDocumentItem item)
{
    return (getX() < item.getX()) ||
           ((getX() == item.getX()) && (getY() < item.getY()));
}
