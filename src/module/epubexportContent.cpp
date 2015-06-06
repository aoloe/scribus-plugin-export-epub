/**
 *
 */
#include <QDebug>

#include "module/epubexportContent.h"
// #include "module/epubexportImage.h"
#include "module/epubexportEpubfile.h"
#include "module/epubexportStructure.h"

#include "plugins/scribusAPI/scribusAPIDocumentItem.h"

#include "module/epubexportXhtml.h"


EpubExportContent::EpubExportContent()
{
}

EpubExportContent::~EpubExportContent()
{
}

/**
 * for each section create an xhtml file that gets added to the epub file.
 * the content is retrieved by looping through the pages and adding each item sequentially to the xhtml structure
 */
void EpubExportContent::fillEpub(EpubExportEpubfile* epub, EpubExportStructure* structure)
{
    // TODO: currently we are creating an xhtml file per section. as soon as a real TOC is available
    // give a choice to use it, instead.
    int sectionI = 0;
    foreach(QList<int> section, scribusDocument->getSections())
    {
        sectionI++;
        EpubExportXhtml xmlFile;
        // TODO: get the information below from structure or anywhere else more appropriate...
        xmlFile.initialize("en", "this document", "../Styles/style.css");

        foreach (int page, section)
        {
            qDebug() << "page" << page;
            // TODO: iterate throug the items and add each of them to the xhtml file
            foreach (ScribusAPIDocumentItem* item, scribusDocument->getPageItems(page))
            {
                qDebug() << "item" << item;
                if (item->isTextFrame())
                {
                    // TODO: how to get the xml back from item? as QDOM... or as QString?
                    // addTextXhtml(item->getXhtmlContent(xmlFile.getQDomDocument()));
                    xmlFile.addContentText(item->getTextDom(xmlFile.getQDomDocument()));
                }
                else if (item->isImageFrame())
                {
                    // item->setPageSize();
                    // xmlFile.addContent(item->getDomContentImage(xmlFile.getQDomDocument()));
                    // EpubExportImage image;
                    ScribusAPIDocumentItemImageWeb image = item->getImageWeb();
                    // TODO: make sure that the filename is unique (and that images are not inserted at double
                    // by looking at the path?)
                    xmlFile.addContentImage(image);
                    // TODO: add the image to the package!
                    epub->addFileUncompressed("OEBPS/Images/" + image.filename, image.path);
                    // epub->add("OEBPS/Text/" + filename, xmlFile.getString());
                    structure->addToManifest(image.filename, "Images/" + image.filename, image.mediatype);
                }
                foreach (ScribusAPIDocumentItemResourceFile resourceFile, item->getResourceFiles())
                {
                    // TODO: add the file to the epub container
                    // TODO: add the file to the toc + ncx
                }
            }
        }
        QString filename = QString("Section%1.xhtml").arg(sectionI, 4, 10, QChar('0'));
        // TODO: as soon as we have a TOC, take the title from the text
        QString title = QString("Section %1").arg(sectionI, 4, 10, QChar('0'));
        // passing somethign else then -1 to toString() adds indenting line breaks. we prefer to manually add
        // some breaks with getFixedXhtml()
        // epubFile->add("OEBPS/Text/" + file.filename, getFixedXhtml(xhtmlDocument.toString(-1)), true);
        epub->add("OEBPS/Text/" + filename, xmlFile.getString());
        structure->addToManifest(filename, "Text/" + filename, "application/xhtml+xml");
        structure->addToToc(filename, "Text/" + filename, title);
    }
}
