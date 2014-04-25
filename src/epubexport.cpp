#include <QDebug>

#include "epubexport.h"
#include "scribusdoc.h"

#include "module/epubexportScribusDoc.h"

EpubExport::EpubExport()
{
    // progressDialog = 0;
    // itemNumber = 0;

    // qDebug() << "marksList" << this->doc->get()->marksList();
    // qDebug() << "notesList" << this->doc->get()->notesList();
}

EpubExport::~EpubExport()
{
    this->doc = new EpubExportScribusDoc();
}

void EpubExport::addScribusDocument(ScribusDoc* doc)
{
    this->doc->add(doc);

}

void EpubExport::doExport()
{
}
