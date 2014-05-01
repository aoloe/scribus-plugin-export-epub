#include <QDebug>

#include "module/epubexportScribusDoc.h"
#include "module/epubexportStructure.h"

#include "scribusdoc.h"
#include "scribusstructs.h" // for getPageRect() remove it, it's moved to ScPage

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


/* =====================================================================================
 *  THE FOLLOWING METHODS SHOULD PROBABLY RATHER TO GO THE SCRIBUS MAIN CODE
 * =====================================================================================
 */

/**
 * go through the full items list in the document and add a reference of the printable ones
 * in a list sorted by page
 */
void EpubExportScribusDoc::readItems()
{
    QList<int> layerNotPrintable;
    foreach(ScLayer layer, scribusDoc->Layers)
        if (!layer.isPrintable)
            layerNotPrintable.append(layer.ID);

    int n = this->getPageCount();
    // qDebug() << "readItems n: " << n;
    items.resize(n);
	foreach(PageItem* docItem, scribusDoc->DocItems)
    {
		if (!docItem->printEnabled())
			continue;
        if (layerNotPrintable.contains(docItem->LayerID))
            continue;

        const QList<ScPage*> itemPages = getPagesWithItem(docItem);
		// if the item is not on a page, ignore it
        if (itemPages.empty())
			continue;
        items[itemPages.first()->pageNr()].append(docItem);
        // itemCounter++; eventually, for the progress bar... but we should probably count the pages
    }
    qDebug() << "items:" << items;
}

/**
 * Returns a list of ScPage where the item appears. If the item is fully ion the scratch space,
 * an empty vector is returned.
 * TODO:
 * - This (or a similar) method should replace the (very) similar calculations in
 *   ScribusDoc::fixItemPageOwner, ScribusDoc::OnPage and PDFLibCore::PDF_ProcessItem
 *   It should go to PageItem (or ScPage) and it should be cached in memory + eventually in the .SLA
 *   --> PageItem::getPages()
 *   (According to jghali OwnPage should only be used make sense of the coordinates of an item,
 *   which are stored in relation to its own page)
 */
QList<ScPage *> EpubExportScribusDoc::getPagesWithItem(PageItem* item)
{
    QList<ScPage *> result;

    // some woodoo adjustings
	if (item->isGroup())
		item->asGroupFrame()->adjustXYPosition();
	item->setRedrawBounding();

	double itemLineWidth = item->lineWidth();
    QRect pageRect;
    QRect itemRect = QRect(
        static_cast<int>(item->BoundingX - itemLineWidth / 2.0), // x
        static_cast<int>(item->BoundingY - itemLineWidth / 2.0), // y
        static_cast<int>(qMax(item->BoundingW + itemLineWidth, 1.0)), // w
        static_cast<int>(qMax(item->BoundingH + itemLineWidth, 1.0)) // h
    );

    bool fullyOnOwnPage = false;
    // First check if the element is fully on its OwnPage
    // OwnPage is an indicator of where the item could be, but it's not reliable.
    if (item->OwnPage > -1)
    {
        ScPage* page = scribusDoc->DocPages.at(item->OwnPage); // TODO: use the real page that we are handling
        if (getPageRect(page).contains(itemRect)) {
            result.append(page);
            fullyOnOwnPage = true;
        }
    }

    // If the item is not fully on the OwnPage, check on all pages (in the range)
    if (!fullyOnOwnPage)
    {
        // TODO: if creating the QRect is expensive, we can create a list of pages' QRects
        // before cycling through the items
        bool allPages = pageRange.isEmpty();
        int n = allPages ? getPageCount() : pageRange.count();
        for (int i = 0; i < n; ++i)
        {
            ScPage* page = scribusDoc->DocPages.at(allPages ? i : pageRange.at(i) - 1);
            if (getPageRect(page).intersects(itemRect))
                result.append(page);
            // TODO: we can use rect.intersected() to get a rectangle and calculate the area of the page
            // that has the biggest intersection and use it as the "main page";
            // or we can use the first page where the intersection occurs (two different uses)
        }
        // TODO: if OwnPage == -1 and a page has been found, fix OwnPage in the item
    }

    return result;
}

/**
 * TODO:
 * Add it as ScPage::getRect(const ScPage* page)
 * Eventually, rename to signify that it does not return xOffset, yOffset, ... but it adds the bleeds
 */
QRect EpubExportScribusDoc::getPageRect(const ScPage* page)
{
    MarginStruct bleeds = getPageBleeds(page);
    return QRect(
        static_cast<int>(page->xOffset() - bleeds.Left), // x
        static_cast<int>(page->yOffset() - bleeds.Top), // y
        static_cast<int>(page->width() + bleeds.Left + bleeds.Right), // w
        static_cast<int>(page->height()+ bleeds.Bottom + bleeds.Top) // h
    );
}

/**
 * TODO:
 * add it as ScPage::getBleeds(const ScPage* page) and eventually remove/deprecate all the
 * ScribusDoc::getBleeds(...) methods.
 * Warning: in ScribusDoc there are also bleeds() methods that return the values without the facing
 * pages correction!
 */
MarginStruct EpubExportScribusDoc::getPageBleeds(const ScPage* page)
{
    MarginStruct result;
    scribusDoc->getBleeds(page, result);
    return result;
}
