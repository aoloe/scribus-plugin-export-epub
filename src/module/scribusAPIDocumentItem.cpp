#include <QDebug>

#include <QString>
#include <QDomElement>
#include <QSize>

#include "util_formats.h" // for checking file extension

#include "module/scribusAPIDocumentItem.h"
#include "module/scribusAPIDocumentItemFormatting.h"

ScribusAPIDocumentItem::ScribusAPIDocumentItem()
{
}

ScribusAPIDocumentItem::~ScribusAPIDocumentItem()
{
}

/**
 * TODO: as soon as other write directions are to be considered the order has to be made more flexible
 */
bool ScribusAPIDocumentItem::isBefore(ScribusAPIDocumentItem* const item) const
{
    return (getY() < item->getY()) ||
           ((getY() == item->getY()) && (getX() <= item->getX()));
}

/**
 * example of charStyle() use in svgexplugin.cpp 
 * cf. short-words/parse.cpp
 * cf. Scribus150Format::writeITEXTs() for getting the local formatting
 * in plugins/fileloader/scribus150format/scribus150format_save.cpp
 *
 * - get the list of runs (chunks different formatting) in the text frame
 * - foreach run:
 *   - if it's a p
 *     - create a p element with it's style name
 *     - set this element as the latest inserted paragraph
 *     - set the the dom element as the current one
 *   - get the character style
 *   - get the local formatting of the first char in the run
 *     - font name
 *       - if the the font name suggests an italic or bold add italic or bold to the list of the formatting
 *   - foreach formatting (feature)
 *     - if it's bold, italic, superscript, subscript create an element with the specfic tag
 *     - if it's underline, strikethrough add the formatting to the span's style attribute
 *   - if there is a character style of a local formatting:
 *     - if not specific tag has been createdi create a span element
 *     - add the class and style attributes to the element
 *     - add the span to the paragraph element and set it as the current element
 *   - insert the text line by line (with a br at the end of each but the last line)
 *   - add the p to the dom
 *   - set the current element as the latest element paragraph created
 */
QList<QDomElement> ScribusAPIDocumentItem::getTextDom(QDomDocument xhtmlDocument)
{
    QList<QDomElement> result;
    // initialize the local variables
    QDomElement elementCurrent;
    QDomElement elementParagraph;
    QDomElement element;
    QString paragraphStyleName;
    QString characterStyleName;
    QString run_text;

    // qDebug() << "item name" << item->itemName();
    ScribusAPIDocumentItemFormatting formatting;
    formatting.setItem(item);
    // qDebug() << "isTextFirstInChain" << isTextFirstInChain(); 
    // qDebug() << "isTextEmpty" << isTextEmpty(); 

    if (isTextFirstInChain() && (!isTextEmpty()))
    {
        foreach (ScribusAPIDocumentItemTextRuns run, getTextRuns())
        {
            formatting.readAtPosition(run.position);

            // qDebug() << "run:" << run;
            if (run.type == 'p')
            {
                if (!elementParagraph.isNull())
                    result.append(elementParagraph);

                elementParagraph = xhtmlDocument.createElement("p");

                paragraphStyleName = formatting.getParagraphStyleName();
                paragraphStyleName = getStylenameSanitized(paragraphStyleName);
                // qDebug() << "paragraphStyle:" << paragraphStyleName;
                if (paragraphStyleName != "")
                    elementParagraph.setAttribute("class", paragraphStyleName);

                elementCurrent = elementParagraph;
            }

            QStringList characterFormatting;

            if (formatting.isBold())
            {
                qDebug() << "paragraphStyleName" << paragraphStyleName;
                // TODO: only set b (and i & co) if b is not set in the paragraph or character style
                // TODO: find a way to remove the bold if it's in the style but not in the local formatting
                element = xhtmlDocument.createElement("b");
                elementCurrent.appendChild(element);
                elementCurrent = element;
            }
            if (formatting.isItalic())
            {
                element = xhtmlDocument.createElement("i");
                elementCurrent.appendChild(element);
                elementCurrent = element;
            }
            if (formatting.isSuperscript())
            {
                element = xhtmlDocument.createElement("sup");
                elementCurrent.appendChild(element);
                elementCurrent = element;
            }
            if (formatting.isSubscript())
            {
                element = xhtmlDocument.createElement("sub");
                elementCurrent.appendChild(element);
                elementCurrent = element;
            }
            if (formatting.isUnderline() || formatting.isUnderlineWords())
            {
                characterFormatting << "text-decoration:underline";
            }
            if (formatting.isStrikethrough())
            {
                characterFormatting << "text-decoration:line-through";
            }
            /*
            else if (feature != CharStyle::INHERIT) // what is this good for?
                qDebug() << "else feature" << feature;
            */
            /*
             * The following other character formats are supported by Scribus but not retained
             * but not (yet) exported to epub:
             * OUTLINE: text-outline: 1px 1px #ccc;
             * SHADOWED: text-shadow: 2px 2px #ff0000;
             * ALLCAPS: text-transform:uppercase;
             * SMALLCAPS: font-variant:small-caps;
             * SHYPHEN: hyphenation is possible... how to use it?
             */
            // qDebug() << "tag name: " << element.tagName();

            characterStyleName = formatting.getCharacterStyleName();
            characterStyleName = getStylenameSanitized(characterStyleName);
            // qDebug() << "character style: " << characterStyleName;

            if (!characterFormatting.isEmpty() || (characterStyleName != ""))
            {
                element = xhtmlDocument.createElement("span");
                // qDebug() << "tag name: " << element.tagName();

                if (characterStyleName != "")
                    element.setAttribute("class", characterStyleName);

                if (!characterFormatting.isEmpty())
                    element.setAttribute("style", characterFormatting.join("; ") + ";");

                elementCurrent.appendChild(element);
                elementCurrent = element;
            }

            foreach (QVector<QString> content, run.content)
            {
                QVectorIterator<QString> list(content);
                while (list.hasNext()) {
                    QDomText elementText = xhtmlDocument.createTextNode(list.next());
                    elementCurrent.appendChild(elementText);
                    if (list.hasNext())
                        elementCurrent.appendChild(xhtmlDocument.createElement("br"));
                }
            }
            /*
            if (elementParagraph.text() != "")
                xhtmlBody.appendChild(elementParagraph);
            */

            elementCurrent = elementParagraph;
        }
        if (!elementParagraph.isNull())
            result.append(elementParagraph);
    }
    // qDebug() << "result" << result;
    return result;
}

/**
 * @return the visible part of the image in PNG or JPG format
 */
ScribusAPIDocumentItemImageWeb ScribusAPIDocumentItem::getImageWeb()
{
    ScribusAPIDocumentItemImageWeb result;

    QString filename(item->Pfile);
    qDebug() << "filename" << filename;
    if (filename == "")
        return result;
    result.path = filename;

    QFileInfo fileinfo = QFileInfo(filename);
    QString ext = fileinfo.suffix().toLower();

    // TODO: sanitize the filename
    // zippedFilename.remove(QRegExp("[^a-zA-Z\\d\\s_.-]"));
    result.filename = fileinfo.fileName();

    int mediaType = 0;
    if (ext == "png")
        mediaType = FormatsManager::PNG;
    else if (extensionIndicatesJPEG(ext))
        mediaType = FormatsManager::JPEG;
    // qDebug() << "mediaType" << mediaType;

    if (mediaType == 0)
    {
        // TODO: convert the other "acceptable" image types to png or jpeg (how to choose?)
        qDebug() << "image format not yet supported: " << filename;
        return result;
    }

    result.mediatype = FormatsManager::instance()->mimetypeOfFormat(mediaType).first();

    QPixmap image; // empty if the image has not been cropped nor scaled
    bool usingLoadedImage = false;

    // qDebug() << "image file" << filename;
    // qDebug() << "imageXScale" << item->imageXScale();
    // qDebug() << "imageYScale" << item->imageYScale();
    // qDebug() << "imageXOffset" << item->imageXOffset();
    // qDebug() << "imageYOffset" << item->imageYOffset();
    // qDebug() << "item width" << item->width();
    // qDebug() << "item height" << item->height();
    // qDebug() << "image BBoxX" << item->pixm.imgInfo.BBoxX;
    // qDebug() << "image BBoxH" << item->pixm.imgInfo.BBoxH;
    // qDebug() << "image xres" << item->pixm.imgInfo.xres;
    // qDebug() << "image yres" << item->pixm.imgInfo.yres;
    // qDebug() << "image width" << item->pixm.width();
    // qDebug() << "image height" << item->pixm.height();

    double cropX = item->imageXOffset();
    double cropY =  item->imageYOffset();

    // calculate the frame's width and height in "image pixels"
    double frameWidth = item->width() / item->imageXScale();
    double frameHeight = item->height() / item->imageYScale();

    result.imageSize = QSize(static_cast<int>(frameWidth), static_cast<int>(frameHeight));

     // TODO: if the image's width and height are already stored, only load the image when it has to be cropped or scaled
    if (!image.load(filename))
        return result;

    QRect frameRect = QRect(- cropX, -cropY, frameWidth, frameHeight);
    QRect imageRect = QRect(0, 0, image.width(), image.height());
    QRect cropRect;
    // qDebug() << "frameRect" << frameRect;
    // qDebug() << "imageRect" << imageRect;

    if (frameRect != imageRect)
    {
        usingLoadedImage = true;
        cropRect = frameRect.intersected(imageRect);
        // qDebug() << "cropRect" << cropRect;
        if (!cropRect.isEmpty())
        {
            QPixmap imageTmp = image.copy(cropRect);
            image = imageTmp;
        }
    }
    /*

    qDebug() << "frameWidth" << frameWidth;
    qDebug() << "imageMaxWidth" << imageMaxWidth;
    int scaling = 100;

    ScPage* page = doc->DocPages.at(this->pageNumber);
    qDebug() << "item width" << item->width();
    double proportion = item->width() / (page->width() - page->rightMargin() - page->leftMargin());
    qDebug() << "proportion" << proportion;
    qDebug() << "imageMaxWidthThreshold" << imageMaxWidthThreshold;

    int scaledWidth;
    if (proportion > static_cast<double>(imageMaxWidthThreshold) / 100)
        scaledWidth = imageMaxWidth;
    else
        scaledWidth = static_cast<int>(proportion * imageMaxWidth);
    qDebug() << "scaledWidth" << scaledWidth;

    qDebug() << "image width" << image.width();
    if (scaledWidth < frameWidth)
        scaling = static_cast<int>(static_cast<double>(scaledWidth) / image.width() * 100);
    qDebug() << "scaling" << scaling;

    QString zippedFilename = fileinfo.fileName();
    if (usingLoadedImage)
    {
        zippedFilename = fileinfo.completeBaseName()+"_c-%1-%2-%3-%4-s-%5."+fileinfo.suffix();
        zippedFilename = zippedFilename.arg(cropRect.x()).arg(cropRect.y()).arg(cropRect.width()).arg(cropRect.height()).arg(scaling);
    }
    zippedFilename.remove(QRegExp("[^a-zA-Z\\d\\s_.-]"));
    qDebug() << "zippedFilename" << zippedFilename;

    QString filepath = "Images/" + zippedFilename;

    // add the image to the dom
    QDomElement div = xhtmlDocument.createElement("div");
    // TODO: only set class picture if the images is maximized (or use different names: the goal is not to only set the page break before, if the picture is maximized)
    div.setAttribute("class", "picture");
    QDomElement element = xhtmlDocument.createElement("img");
    // <image height="800" width="600" xlink:href="../Images/cover.jpeg"></image>
    element.setAttribute("height", (int) item->height()); // TODO: use the real width of the visible part of the image (as a rectangle)
    element.setAttribute("width", (int) item->width());
    element.setAttribute("alt", ""); // TODO do we have a way to define the metadata? eventually from the exif? epubcheck says it's mandatory... and it's not nice to leave it empty...
    element.setAttribute("src", "../"+filepath); // TODO: make sure that the name is unique in the target! (if it already exists prefix the frame name?)
    // TODO: set the width and height? from the item?
    div.appendChild(element);

    result.append(div);
    */


    /*
    void EpubExport::addImage(PageItem* docItem)
    {


        // TODO: add also the path to the original picture before the zippedFilename in order to catch files with
        // the same name but from different paths... and then? how to set the name?
        if (!imageFileNames.contains(zippedFilename))
        {
            imageFileNames << zippedFilename;
            qDebug() << "zippedFilename" << zippedFilename;
            qDebug() << "imageFileNames" << imageFileNames;
            if (scaling > 0 && scaling < 100)
            {
                usingLoadedImage = true;
                QPixmap imageTmp = image.scaledToWidth(scaledWidth, Qt::SmoothTransformation);
                image = imageTmp;
            }

            // copy the image into the zip
            if (!usingLoadedImage)
            {
                qDebug() << "standard file add";
                QFile file(fileinfo.filePath()); // TODO: if we already have a scimage we may have to change this
                epubFile->add("OEBPS/"+filepath, &file, true);
            }
            else
            {
                QByteArray imageBytes;
                QBuffer buffer(&imageBytes);
                buffer.open(QIODevice::WriteOnly);
                image.save(&buffer, mediaType == FormatsManager::JPEG ? "jpg" : "png");
                epubFile->add("OEBPS/"+filepath, imageBytes, false);


            }

            struct EPUBExportContentItem contentItem;
            contentItem.id = zippedFilename;
            contentItem.href = filepath;
            contentItem.mediaType = FormatsManager::instance()->mimetypeOfFormat(mediaType).first();
            contentItems.append(contentItem);
        } // if (imageFileNames.contains(zippedFilename))
    }
*/
        /*
        // TODO: some leftovers if we want ever do a color managed conversion of the pictures
        ScImage img;
        ScImage item->pixm;
        ImageInfoRecord imgInfo;
            ImageTypeEnum type -> 0 = jpg, 1 = tiff, 2 = psd, 3 = eps/ps, 4 = pdf, 5 = jpg2000, 6 = other
            ColorSpaceEnum colorspace -> 0 = RGB  1 = CMYK  2 = Grayscale 3 = Duotone
        CMSettings cms(c->doc(), Profil, Intent);
        cms.setUseEmbeddedProfile(Embedded);
        usingLoadedImage = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, 72, &realCMYK);
        bool loadPicture(const QString & fn, int page, const CMSettings& cmSettings, RequestType requestType, int gsRes, bool *realCMYK = 0, bool showMsg = false);
         */

    return result;
}

QList<ScribusAPIDocumentItemResourceFile> ScribusAPIDocumentItem::getResourceFiles()
{
    QList<ScribusAPIDocumentItemResourceFile> result;
    return result;
}

/**
 * parse the text and define the runs list
 * this method is based on pierre's work for the mitical OIF branch
 * ... also inspired by Scribus150Format::writeITEXTs
 * TODO: use the text/storytext methods as soon as they are implemented
 */

QVector<ScribusAPIDocumentItemTextRuns> ScribusAPIDocumentItem::getTextRuns()
{
    QVector<ScribusAPIDocumentItemTextRuns> runs; // the indexes where the runs start
    struct ScribusAPIDocumentItemTextRuns runsItem;
    runs.clear();

    CharStyle lastStyle;
    int lastPos = 0;
    runsItem.position = 0;
    runsItem.type = 'p';
    runsItem.content.clear();

    StoryText itemText = this->item->itemText;
    int n = itemText.length();

    QVector<QString> lines;
    QString line = "";

    if (n == 0) {
        runsItem.length = 0;
        runs.append(runsItem);
    } else {
        lines.clear();
        QString content = itemText.text(0, itemText.length());

        for(int i = 0; i < n; ++i)
        {
            bool output = true;

            const CharStyle& style1(itemText.charStyle(i));
            const QChar ch = itemText.text(i);
            /*
            // TODO: we have to correctly handle the marks
            ScText* chProperties = itemText.item(i);
            if (chProperties->mark) {
                Mark* footnoteCall = chProperties->mark;
                if (footnoteCall->getType() == MARKNoteMasterType) {
                    TextNote* footnote = chProperties->mark->getData().notePtr;
                    qDebug() << "calling mark:" << footnoteCall->getString();
                    if (!footnote->saxedText().isEmpty()) {
                        StoryText footnoteText = desaxeString(doc, footnote->saxedText());
                        qDebug() << "note text:" << footnoteText.text(0, footnoteText.length());
                        // TODO: refactor to be able to recursively call the html formatting!
                    }
                    qDebug() << "note text:" << footnote->saxedText();
                    qDebug() << "note mark:" << footnote->num();
                    qDebug() << "note style:" << footnote->notesStyle()->name();

                    // apptend 
                } else if (footnoteCall->getType() == MARKNoteFrameType) {
                }
                continue; // don't insert the marks as such!
            }
            */

            /*
            // store formatting struct for:
            // - current paragraph style
            // - current character style
            // - sum of current paragraph and char style
            // - currentformatting
            // store last insterted formatting string (<strong><em><span style="font-size:12px");
            // --> struct EPUBExportCurrentFormatting
            */
            
            // if there is a paragraph change or the formatting has changed
            if (
                ch == SpecialChars::PARSEP ||
                ch == SpecialChars::COLBREAK ||
                ch == SpecialChars::FRAMEBREAK
            )
            {
                // qDebug() << "ch" << ch.unicode();
                runsItem.length = i - runsItem.position;
                if (line != "")
                {
                    lines.append(line);
                    line = "";
                }
                if (!lines.isEmpty())
                {
                    runsItem.content.append(lines);
                    lines.clear();
                }
                if (!runsItem.content.isEmpty())
                    runs.append(runsItem);
                runsItem.position = i;
                runsItem.type = 'p';
                runsItem.content.clear();
                output = false;
            }
            if (ch == SpecialChars::LINEBREAK)
            {
                if (line != "")
                {
                    lines.append(line);
                    line = "";
                }
                output = false;
            }
            // accepted special chars:
            // ch == SpecialChars::NBSPACE

            if (
                style1 != lastStyle
            )
            {
                // something new, write pending chars
                if  (i - lastPos > 0)
                {
                    // docu.writeEmptyElement("ITEXT");
                    /*if (item->asPathText()) // seems to cause problems when loading pathtext elements
                        putCStylePT(docu, lastStyle);
                    else*/
                        // putCStyle(docu, lastStyle);
                    // docu.writeAttribute("CH", textWithSoftHyphens(item->itemText, lastPos, i));
                }
                // runs.append(i);
                if (line != "")
                {
                    lines.append(line);
                    line = "";
                }
                if (!lines.isEmpty())
                {
                    runsItem.content.append(lines);
                    lines.clear();
                }
                runsItem.length = i - runsItem.position;
                if (runsItem.type == 'p' || !runsItem.content.isEmpty())
                    runs.append(runsItem);
                runsItem.position = i;
                runsItem.type = 'f';
                runsItem.content.clear();
                lastStyle = style1;
                lastPos = i;
            }
            if (
                ch == SpecialChars::NBHYPHEN ||
                ch == SpecialChars::NBSPACE ||
                ch == SpecialChars::ZWNBSPACE ||
                ch == SpecialChars::ZWSPACE
            )
            {
            }
            // @todo: ignore the following chars
            if (
                ch == SpecialChars::OBJECT ||
                ch == SpecialChars::TAB ||
                ch == SpecialChars::PAGENUMBER ||
                ch == SpecialChars::PAGECOUNT ||
                ch.unicode() < 32 || 
                (0xd800 <= ch.unicode() && ch.unicode() < 0xe000) ||
                ch.unicode() == 0xfffe || ch.unicode() == 0xffff
            )
            {
            }

            if (output)
                line += ch;
        }
        runsItem.length = n - runsItem.position;
        if (line != "")
            lines.append(line);

        if (!lines.isEmpty())
            runsItem.content.append(lines);

        if (!runsItem.content.isEmpty())
            runs.append(runsItem);
    }
    // qDebug() << "runs: " << runs;
    // qDebug() << "paragraph: " << paragraph;
    return runs;
}


QString ScribusAPIDocumentItem::getStylenameSanitized(QString stylename)
{
    return stylename.remove(QRegExp("[^a-zA-Z\\d_-]"));
}

QDebug operator<<(QDebug dbg, ScribusAPIDocumentItem &item)
{
    dbg.nospace() << "(name:" << item.getName() << ")";
    return dbg.space();
}

// not tested yet
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemTextRuns run)
{
    dbg.nospace() << "(position:" << run.position << ", lenght:" << run.length  << ", type:" << run.type  << ", content:" << run.content << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const QVector<ScribusAPIDocumentItemTextRuns> &runs)
{
    dbg.nospace() << "[" ;
    foreach (ScribusAPIDocumentItemTextRuns run, runs)
    {
        dbg.nospace() << run;
    }
    dbg.nospace() << "]" ;
    return dbg.space();
}
/**
 * TODO: do not print the full content!
 */
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemResourceFile file)
{
    dbg.nospace() << "(filename:" << file.filename << ", filePath:" << file.filePath  << ", fileContent:" << file.fileContent << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemImageWeb image)
{
    dbg.nospace() << "(\npath:" << image.path << ",\nfilename:" << image.filename << ",\ncontent:" << image.content  << ",\ndescription:" << image.description << ",\nimageSize:" << image.imageSize << ",\nmediatype:" << image.mediatype << "\n)";
    return dbg.space();
}
