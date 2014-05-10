/**
 * collects and exports to xhtml the text content of the epub file
 */
#ifndef EPUBEXPORTCONTENT_H
#define EPUBEXPORTCONTENT_H

#include <QObject>
#include <QDebug>

#include "module/epubexportScribusDoc.h"

class EpubExportEpubfile;
class EpubExportStructure;

class EpubExportContent : public QObject
{
    Q_OBJECT

public:
	EpubExportContent();
	~EpubExportContent();

    void setDocument(EpubExportScribusDoc* scribusDocument) { this->scribusDocument = scribusDocument; }

    void fillEpub(EpubExportEpubfile* epub, EpubExportStructure* structure);

private:
    EpubExportScribusDoc* scribusDocument;
};

QDebug operator<<(QDebug dbg, const EpubExportContent content);

#endif // EPUBEXPORTCONTENT_H
