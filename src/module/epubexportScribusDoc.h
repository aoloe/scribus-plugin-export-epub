/**
 * EpubExportScribusDoc contains a reference to the current scribus document and is used as
 * an interface to get information and parse the scribus document itself
 */
#ifndef EPUBEXPORTSCRIBUSDOC_H
#define EPUBEXPORTSCRIBUSDOC_H

#include <QObject>
#include <QDebug>

class ScribusDoc;

class EpubExportStructureMetadata;

class EpubExportScribusDoc : public QObject
{
    Q_OBJECT

public:
	EpubExportScribusDoc();
	~EpubExportScribusDoc();

    void set(ScribusDoc* scribusDoc) {this->scribusDoc = scribusDoc;}

    EpubExportStructureMetadata getMetadata();
private:
    ScribusDoc* scribusDoc;
};

QDebug operator<<(QDebug dbg, const EpubExportScribusDoc &scribusDoc);

#endif // EPUBEXPORTSCRIBUSDOC_H
