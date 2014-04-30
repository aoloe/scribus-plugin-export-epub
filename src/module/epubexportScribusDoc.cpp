#include <QDebug>

#include "module/epubexportScribusDoc.h"
#include "module/epubexportStructure.h"

#include "scribusdoc.h"

EpubExportScribusDoc::EpubExportScribusDoc()
{
}

EpubExportScribusDoc::~EpubExportScribusDoc()
{
}

EpubExportStructureMetadata EpubExportScribusDoc::getMetadata()
{
    EpubExportStructureMetadata metadata = EpubExportStructureMetadata();
	DocumentInformation documentMetadata = this->scribusDoc->documentInfo();

	// for mandatory fields we make later sure that they are filled
    metadata.title = documentMetadata.title();
    metadata.language = documentMetadata.langInfo();
    metadata.id = documentMetadata.ident();
    metadata.date = documentMetadata.date();
    metadata.language = documentMetadata.langInfo();
    metadata.title = documentMetadata.title();
    metadata.author = documentMetadata.author();
	// non mandatory fields from the main screen
    metadata.subject = documentMetadata.subject();
    metadata.keywords = documentMetadata.keywords();
	metadata.description = documentMetadata.comments();
	metadata.publisher = documentMetadata.publisher();
	// non mandatory fields from the further information screen
	metadata.contributor = documentMetadata.contrib();
	metadata.type = documentMetadata.type();
    metadata.format = documentMetadata.format();
	metadata.source = documentMetadata.source();
	metadata.relation = documentMetadata.relation();
	metadata.coverage = documentMetadata.cover();
	metadata.rights = documentMetadata.rights();

    return metadata;
}
