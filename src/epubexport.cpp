#include <QDebug>

#include "epubexport.h"
#include "scribusdoc.h"

EpubExport::EpubExport()
{
    // progressDialog = 0;
    // itemNumber = 0;

    // qDebug() << "marksList" << this->doc->get()->marksList();
    // qDebug() << "notesList" << this->doc->get()->notesList();
}

EpubExport::~EpubExport()
{
    // this->doc = new EpubExportScribusDoc();
}

void EpubExport::setScribusDocument(ScribusDoc* doc)
{
    // this->doc->add(doc);

}

void EpubExport::doExport()
{
}
