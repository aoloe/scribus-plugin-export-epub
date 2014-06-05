/**
 * EpubExportScribusDocument is a proxy to the scribus' PageItem and is thought as
 * an interface to get information about the scribus items
 */
#ifndef EPUBEXPORTSCRIBUSDOCUMENTITEM_H
#define EPUBEXPORTSCRIBUSDOCUMENTITEM_H

#include <QDebug>

#include <QObject>

#include "scribusdoc.h"

class EpubExportScribusDocumentItem : public QObject
{
    Q_OBJECT

public:
	EpubExportScribusDocumentItem();
	~EpubExportScribusDocumentItem();
    void setItem(PageItem* item) { this->item = item; }
    bool isBefore(EpubExportScribusDocumentItem* item);
    /**
     * used by qSort to sort the items by their place on the page
     */
    static bool isBeforeQSort(const EpubExportScribusDocumentItem* item1, const EpubExportScribusDocumentItem* item2) { return true; } // item1.isBefore(item2); }
    int getX() { return this->item->gXpos; }
    int getY() { return this->item->gYpos; }

protected:
    PageItem* item;
};

QDebug operator<<(QDebug dbg, const EpubExportScribusDocumentItem &scribusDocumentItem);

#endif // EPUBEXPORTSCRIBUSDOCUMENTITEM_H
