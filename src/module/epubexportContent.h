// Collects the content (text, images) of the epub file.

#ifndef EPUBEXPORTCONTENT_H
#define EPUBEXPORTCONTENT_H

#include <QObject>
#include <QDebug>

#include <QList>

#include "plugins/scribusAPI/scribusAPIDocument.h"

class EpubExportEpubfile;
class EpubExportStructure;

class ScPage;

/**
 * @brief Collects the content (text, images) of the epub file.
 *
 */
class EpubExportContent : public QObject
{
    Q_OBJECT

public:
	EpubExportContent();
	~EpubExportContent();

    /**
     * @brief Pass the current document to the content class.
     */
    void setDocument(ScribusAPIDocument* scribusDocument) { this->scribusDocument = scribusDocument; }
    /**
     * @brief Get the Epub file and structure and fill them with the content.
     */
    void fillEpub(EpubExportEpubfile* epub, EpubExportStructure* structure);

private:
    ScribusAPIDocument* scribusDocument;

    void addTextXhtml(QString contentXhtml);
};

QDebug operator<<(QDebug dbg, const EpubExportContent content);

#endif // EPUBEXPORTCONTENT_H
