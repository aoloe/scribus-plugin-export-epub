/**
 * EpubExportScribusDocument is a proxy to the scribus' PageItem and is thought as
 * an interface to get information about the scribus items
 */
#ifndef EPUBEXPORTSCRIBUSDOCUMENTITEM_H
#define EPUBEXPORTSCRIBUSDOCUMENTITEM_H

#include <QDebug>

#include <QObject>

class EpubExportScribusDocumentItem : public QObject
{
    Q_OBJECT

public:
	EpubExportScribusDocumentItem();
	~EpubExportScribusDocumentItem();
};

QDebug operator<<(QDebug dbg, const EpubExportScribusDocumentItem &scribusDocumentItem);

#endif // EPUBEXPORTSCRIBUSDOCUMENTITEM_H
