/**
 * collects the content (text, images) of the epub file
 */
#ifndef EPUBEXPORTCONTENT_H
#define EPUBEXPORTCONTENT_H

#include <QObject>
#include <QDebug>

#include <QList>

#include "plugins/scribusAPI/scribusAPIDocument.h"

class EpubExportEpubfile;
class EpubExportStructure;

class ScPage;

class EpubExportContent : public QObject
{
    Q_OBJECT

public:
	EpubExportContent();
	~EpubExportContent();

    void setDocument(ScribusAPIDocument* scribusDocument) { this->scribusDocument = scribusDocument; }

    void fillEpub(EpubExportEpubfile* epub, EpubExportStructure* structure);

private:
    ScribusAPIDocument* scribusDocument;

    void addTextXhtml(QString contentXhtml);
};

QDebug operator<<(QDebug dbg, const EpubExportContent content);

#endif // EPUBEXPORTCONTENT_H
