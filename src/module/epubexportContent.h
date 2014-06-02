/**
 * collects the content (text, images) of the epub file
 */
#ifndef EPUBEXPORTCONTENT_H
#define EPUBEXPORTCONTENT_H

#include <QObject>
#include <QDebug>

#include <QList>

#include "module/epubexportScribusDocument.h"

class EpubExportEpubfile;
class EpubExportStructure;

class ScPage;

class EpubExportContent : public QObject
{
    Q_OBJECT

public:
	EpubExportContent();
	~EpubExportContent();

    void setDocument(EpubExportScribusDocument* scribusDocument) { this->scribusDocument = scribusDocument; }

    void fillEpub(EpubExportEpubfile* epub, EpubExportStructure* structure);

private:
    EpubExportScribusDocument* scribusDocument;
};

QDebug operator<<(QDebug dbg, const EpubExportContent content);

#endif // EPUBEXPORTCONTENT_H
