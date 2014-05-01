#include <QDebug>

#include "epubexport.h"
#include "scribusdoc.h"

#include "module/epubexportScribusDoc.h"
#include "module/epubexportEpubfile.h"
#include "module/epubexportStructure.h"
#include "module/epubexportContent.h"

EpubExport::EpubExport()
{
    // progressDialog = 0;
    // itemNumber = 0;

    // qDebug() << "marksList" << this->doc->get()->marksList();
    // qDebug() << "notesList" << this->doc->get()->notesList();
}

EpubExport::~EpubExport()
{
}

void EpubExport::doExport()
{
    qDebug() << "options" << options;

	options.targetFilename = "/tmp/"+options.targetFilename;
	qDebug() << "forcing the output of the .epub file to /tmp";

    EpubExportEpubfile* epub = new EpubExportEpubfile();
    epub->setFilename(options.targetFilename);
    epub->create();


    EpubExportScribusDoc* scribusDocument = new EpubExportScribusDoc();
    scribusDocument->set(this->scribusDoc);
    scribusDocument->setPageRange(this->options.pageRange);
    scribusDocument->readItems();

    EpubExportStructure* structure = new EpubExportStructure();
    structure->setFilename(options.targetFilename);
    structure->readMetadata(scribusDocument->getMetadata());

    epub->add("OEBPS/Styles/style.css", scribusDocument->getStylesAsCss());
    structure->addToManifest("stylesheet", "Styles/style.css", "text/css");

    EpubExportContent* content = new EpubExportContent();
    content->setDocument(scribusDocument);

	epub->add("META-INF/container.xml", structure->getContainer());

	epub->add("OEBPS/toc.ncx", structure->getNCX());

	epub->add("OEBPS/content.opf", structure->getOPF());

    if (structure->hasCover())
    {
        qDebug() << "has cover";
        epub->addUncompressed("OEBPS/Images/cover.png", structure->getCover());
    } else {
        epub->addUncompressed("OEBPS/Images/cover.png", scribusDocument->getFirstPageAsCoverImage());
        /*
        struct EPUBExportContentItem contentItem;
        contentItem.id = "cover.png";
        contentItem.href = "Images/cover.png";
        contentItem.mediaType = "image/png";
        contentItems.append(contentItem);
        */
    }

    epub->close();
}

QDebug operator<<(QDebug dbg, const EpubExportOptions options)
{
    dbg.nospace() << "(targetFilename:" << options.targetFilename;
    QStringList output;
    foreach (int pageRange, options.pageRange) {
        output << QString::number(pageRange);
    }
    dbg.nospace() << ", (" << output.join(", ") << ")" << ", imageMaxWidth:" << options.imageMaxWidth  << ", imageMaxWidthThreshold:" << options.imageMaxWidthThreshold << ")";
    return dbg.space();
}
