#include <QDebug>

#include <QImage> // for the cover
#include <QByteArray> // for the cover
#include <QBuffer> // for the cover
#include <QVectorIterator> // for sorting the items on the page


#include "module/scribusAPIDocument.h"
#include "module/scribusAPIDocumentItem.h"
// TODO: i guess that the following include should be removed
#include "module/epubexportStructure.h"

#include "scribusdoc.h"
#include "scribusview.h" // for the cover
#include "scribusstructs.h" // for getPageRect() remove it, it's moved to ScPage

ScribusAPIDocument::ScribusAPIDocument()
{
}

ScribusAPIDocument::~ScribusAPIDocument()
{
}

ScribusAPIDocumentMetadata ScribusAPIDocument::getMetadata()
{
    ScribusAPIDocumentMetadata metadata = ScribusAPIDocumentMetadata();
	DocumentInformation documentMetadata = this->scribusDoc->documentInfo();

	// for mandatory fields we make later sure that they are filled
    metadata.title = documentMetadata.title();
    metadata.language = documentMetadata.langInfo();
    metadata.id = documentMetadata.ident();
    metadata.date = documentMetadata.date();
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

/**
  * add OEBPS/Styles/style.css to the current epub file
  * TODO: move to ScribusAPIDocumentStyle
  */ 
QString ScribusAPIDocument::getStylesAsCss()
{
    int n = 0;
    QString wr = QString();

    const StyleSet<ParagraphStyle>* paragraphStyles = & scribusDoc->paragraphStyles();
    n = paragraphStyles->count();
    // qDebug() << "n pstyle: " << n;
    for (int i = 0; i < n; ++i )
    {
        // for a list of properties:  grep -r "paragraphStyle()" . | vim -
        // for a list of properties:  grep -r "charStyle()" . | vim -
        // see fileloader/scribus150format/scribus150format_save.cpp
        ParagraphStyle paragraphStyle = (*paragraphStyles)[i];
        CharStyle charStyle = paragraphStyle.charStyle();
        wr += "p." + getStylenameSanitized(paragraphStyle.name()) + "{\n";
        // qDebug() << "style name: " << paragraphStyle.name();
        // qDebug() << "style alignment: " << paragraphStyle.alignment();
        // qDebug() << "style font: " << charStyle.font().scName();
        // line height: fixed (<- baseline) or auto
        // alignment
        // evt. tabs for lists
        // left right and first indents
        wr += "    padding-top:" + QString::number(paragraphStyle.gapBefore()) + "pt;\n";
        wr += "    padding-bottom:" + QString::number(paragraphStyle.gapAfter()) + "pt;\n";

        wr += "    font-size:" + QString::number(charStyle.fontSize() / 10) + "pt;\n";
        QString fontname = charStyle.font().scName();
        // as long as bold and italic are not in the features list, get the property
        // by guessing from the font name (ale/20120916)
        // rule.pattern = QRegExp("[\\\\|\\<|\\>|\\=|\\!|\\+|\\-|\\*|\\/|\\%]+");
        QRegExp regexpItalic("(\\bitalic\\b)");
        regexpItalic.setCaseSensitivity(Qt::CaseInsensitive);
        if (regexpItalic.indexIn(fontname) >= 0)
        {
            wr += "    font-variant:italic;\n";
        } QRegExp regexpBold("(\\bbold\\b)"); // TODO: add other keywords for bold
        regexpBold.setCaseSensitivity(Qt::CaseInsensitive);
        if (regexpBold.indexIn(fontname) >= 0)
        {
            wr += "    font-weight:bold;\n"; // TODO: add other keywords for italic
        }

        QStringList featureList = charStyle.features();
        QStringList::ConstIterator it;
        for (it = featureList.begin(); it != featureList.end(); ++it)
        {
            QString feature = it->trimmed();
            // qDebug() << "feature" << feature;
            if ((feature == CharStyle::UNDERLINE) || (feature == CharStyle::UNDERLINEWORDS))
                wr += "    text-decoration:underline;\n";
            else if (feature == CharStyle::STRIKETHROUGH)
                wr += "    text-decoration:line-through;\n";
        }

        // paragraphStyle.hasDropCap()
        // paragraphStyle.dcCharStyleName();
        // paragraphStyle.dropCapOffset());

        // font family + style!
        // tracking and width space
        // underline
        // stroke
        // horizontal / vertical scaling
        // color

        // charStyle.name();
        // charStyle.parent();
        // charStyle.font().scName()
        // charStyle.features().join(" ")
        // charStyle.fillColor();
        // charStyle.fillShade();
        // charStyle.strokeColor();
        // charStyle.strokeShade();
        // charStyle.shadowXOffset() / 10.0
        // charStyle.shadowYOffset() / 10.0
        // charStyle.outlineWidth() / 10.0
        // charStyle.underlineOffset() / 10.0
        // charStyle.underlineWidth() / 10.0
        // charStyle.strikethruOffset() / 10.0
        // charStyle.strikethruWidth() / 10.0
        // charStyle.scaleH() / 10.0
        // charStyle.scaleV() / 10.0
        // charStyle.baselineOffset() / 10.0
        // charStyle.language();
        // qDebug() << "charStyle font size: " << charStyle.fontSize();
        wr += "}\n";
    }

    const StyleSet<CharStyle>* charStyles = & scribusDoc->charStyles();
    n = charStyles->count();
    // qDebug() << "n cstyle: " << n;
    for (int i = 0; i < n; ++i )
    {
        CharStyle charStyle = (*charStyles)[i];
        wr += "span." + getStylenameSanitized(charStyle.name()) + "{\n";
        // qDebug() << "style name: " << charStyle.name();
        // qDebug() << "style font: " << charStyle.font().scName();
        wr += "    font-size:" + QString::number(charStyle.fontSize() / 10) + "pt;\n";
        // qDebug() << "style font size: " << charStyle.fontSize();
        wr += "}\n";
    }
    //
    // add epub exporter related styles (must be included in the imported css if one is imported
    // (or check if the exist?)
    wr += "div.picture{\n";
    // qDebug() << "style name: " << paragraphStyle.name();
    // qDebug() << "style alignment: " << paragraphStyle.alignment();
    // qDebug() << "style font: " << charStyle.font().scName();
    // line height: fixed (<- baseline) or auto
    // alignment
    // evt. tabs for lists
    // left right and first indents
    wr += "    text-align:left;\n";
    wr += "    page-break-before:always;\n";
    wr += "    page-break-inside:avoid;\n";
    wr += "}\n";

    /*
    // write the stylesheet
    epubFile->add("OEBPS/Styles/style.css", wr, true);

    struct EPUBExportContentItem contentItem;
    contentItem.id = "stylesheet";
    contentItem.href = "Styles/style.css";
    contentItem.mediaType = "text/css";
    contentItems.append(contentItem);
    */

    /*
    docu.writeAttribute("VHOCH"  , doc->typographicPrefs().valueSuperScript);
    docu.writeAttribute("VHOCHSC", doc->typographicPrefs().scalingSuperScript);
    docu.writeAttribute("VTIEF"  , doc->typographicPrefs().valueSubScript);
    docu.writeAttribute("VTIEFSC", doc->typographicPrefs().scalingSubScript);
    docu.writeAttribute("VKAPIT" , doc->typographicPrefs().valueSmallCaps);
    docu.writeAttribute("LANGUAGE", doc->hyphLanguage());
     */
     // qDebug() << "wr" << wr;
     return wr;
}

/**
 * create a cover as a png of the first page of the .sla
 * From the Sigil documentation:
 * - Image size should be 590 pixels wide x 750 pixels high
 * - Image resolution should be 72 pixels per inch (ppi) or higher
 * - Use color images, saved in RGB color space
 * - Image format can be JPEG, GIF, or PNG.
 * TODO:
 * - make sure that a cover.png image does not yet exist
 * - create an xhtml file with the cover?
 *   http://blog.threepress.org/2009/11/20/best-practices-in-epub-cover-images/
 */
QByteArray ScribusAPIDocument::getFirstPageAsCoverImage()
{
    QImage image;
    if (this->isPortrait(scribusDoc->DocPages.at(0)))
        image = scribusDoc->view()->PageToPixmap(0, 750, false);
    else
        image = scribusDoc->view()->PageToPixmap(590, 0, false);

    QByteArray bytearray;
    QBuffer buffer(&bytearray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    // qDebug() << "image.size" << image.size();
    return bytearray;
}

QString ScribusAPIDocument::getStylenameSanitized(QString stylename)
{
    return stylename.remove(QRegExp("[^a-zA-Z\\d_-]"));
}



/* =====================================================================================
 *  THE FOLLOWING METHODS SHOULD PROBABLY RATHER TO GO THE SCRIBUS MAIN CODE
 * =====================================================================================
 */

/**
 * go through the full items list in the document and add a reference of the printable ones
 * in a list sorted by page and by placement on the page.
 * TODO: correctly handle the page ranges (from .. to)
 * TODO: rename it to signify that it only reads visible items on a page (and make it a getter?)
 */
void ScribusAPIDocument::readItems()
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

        int pageNumber= itemPages.first()->pageNr();

        ScribusAPIDocumentItem* documentItem = new ScribusAPIDocumentItem();
        documentItem->setItem(docItem);
        documentItem->setPageNumber(pageNumber);
        items[pageNumber].append(documentItem);
        // itemCounter++; eventually, for the progress bar... but we should probably count the pages
    }

    for (int i = 0; i < items.count(); i++)
        qSort(items[i].begin(), items[i].end(), ScribusAPIDocumentItem::isBeforeQSort);

    qDebug() << "items:" << items;
}

void ScribusAPIDocument::readSections()
{

    bool allPages = pageRange.isEmpty();
    int n = allPages ? getPageCount() : pageRange.count();
    int nSections = getSectionCount();
    sections.resize(nSections);
    for (int i = 0; i < n; ++i)
    {
        int sectionId = scribusDoc->getSectionKeyForPageIndex(i); // TODO: use the real page that we are handling
        sections[sectionId].append(i);
    }
    qDebug() << "sections" << sections;
}

QList<ScPage*> ScribusAPIDocument::getPagesList()
{
    QList<ScPage *> result;
    bool allPages = pageRange.isEmpty();
    int n = allPages ? getPageCount() : pageRange.count();
    for (int i = 0; i < n; ++i)
    {
        result.append(scribusDoc->DocPages.at(allPages ? i : pageRange.at(i) - 1));
    }
    return result;
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
QList<ScPage *> ScribusAPIDocument::getPagesWithItem(PageItem* item)
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
QRect ScribusAPIDocument::getPageRect(const ScPage* page)
{
    MarginStruct bleeds = getPageBleeds(page);
    return QRect(
        static_cast<int>(page->xOffset() - bleeds.left()), // x
        static_cast<int>(page->yOffset() - bleeds.top()), // y
        static_cast<int>(page->width() + bleeds.left() + bleeds.right()), // w
        static_cast<int>(page->height()+ bleeds.bottom() + bleeds.top()) // h
    );
}

/**
 * TODO:
 * add it as ScPage::getBleeds(const ScPage* page) and eventually remove/deprecate all the
 * ScribusDoc::getBleeds(...) methods.
 * Warning: in ScribusDoc there are also bleeds() methods that return the values without the facing
 * pages correction!
 */
MarginStruct ScribusAPIDocument::getPageBleeds(const ScPage* page)
{
    MarginStruct result;
    scribusDoc->getBleeds(page, result);
    return result;
}

bool ScribusAPIDocument::isPortrait(const ScPage* page)
{
        return (static_cast<int>(page->width()) >= static_cast<int>(page->width()));
}

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentMetadata &metadata)
{
    dbg.nospace() << "( title:" << metadata.title << "\n"
                  << ", author:" << metadata.author << "\n"
                  << ", subject:" << metadata.subject << "\n"
                  << ", date:" << metadata.date << "\n"
                  << ", id:" << metadata.id << "\n"
                  << ", language:" << metadata.language << "\n"
                  << ", keywords:" << metadata.keywords << "\n"
                  << ", description:" << metadata.description << "\n"
                  << ", publisher:" << metadata.publisher << "\n"
                  << ", contributor:" << metadata.contributor << "\n"
                  << ", type:" << metadata.type << "\n"
                  << ", format:" << metadata.format << "\n"
                  << ", source:" << metadata.source << "\n"
                  << ", relation:" << metadata.relation << "\n"
                  << ", coverage:" << metadata.coverage << "\n"
                  << ", rights:" << metadata.rights << "\n"
                  << " )";
    return dbg.space();
}
