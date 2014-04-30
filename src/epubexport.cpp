#include <QDebug>

#include "epubexport.h"
#include "scribusdoc.h"

#include "module/epubexportScribusDoc.h"
#include "module/epubexportEpubfile.h"
#include "module/epubexportStructure.h"

EpubExport::EpubExport()
{
    // progressDialog = 0;
    // itemNumber = 0;

    this->doc = new EpubExportScribusDoc();

    // qDebug() << "marksList" << this->doc->get()->marksList();
    // qDebug() << "notesList" << this->doc->get()->notesList();
}

EpubExport::~EpubExport()
{
}

void EpubExport::addScribusDocument(ScribusDoc* scribusDoc)
{
    this->doc->set(scribusDoc);

}

void EpubExport::doExport()
{
    qDebug() << "options" << options;

	options.targetFilename = "/tmp/"+options.targetFilename;
	qDebug() << "forcing the output of the .epub file to /tmp";

    EpubExportEpubfile* epub = new EpubExportEpubfile();
    epub->setFilename(options.targetFilename);
    epub->create();

    /*
    EpubExportStructure* structure = new EpubExportStructure();
    structure->setFilename(options.targetFilename);
    structure->readMetadata(doc->getMetadata());
    */
    EpubExportStructure structure;
    structure.setFilename(options.targetFilename);
    structure.readMetadata(doc->getMetadata());

	epub->add("META-INF/container.xml", structure.getContainer());

	epub->add("OEBPS/toc.ncx", structure.getNCX());

	epub->add("OEBPS/content.opf", structure.getOPF());

    if (structure.hasCover())
    {
        epub->addUncompressed("OEBPS/Images/cover.png", structure.getCover());
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
