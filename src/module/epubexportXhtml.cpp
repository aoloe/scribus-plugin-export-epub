#include <QDebug>

#include "module/epubexportXhtml.h"

#include <QString>

#include <QDomDocumentType>
#include <QDomImplementation>

#include "text/specialchars.h"


EpubExportXhtml::EpubExportXhtml()
{
}

EpubExportXhtml::~EpubExportXhtml()
{
}

/**
  * create the xhtml document structure and give access to xhtmlBody where addText and addImage
  * will add the content
  *
  * <?xml version="1.0" encoding="UTF-8" ?>
  * <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
  * <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
  *   <head>
  *     <meta http-equiv="Content-Type" content="application/xhtml+xml; charset=utf-8" />
  *     <title>Pride and Prejudice</title>
  *     <link rel="stylesheet" href="css/main.css" type="text/css" />
  *   </head>
  *   <body>
  *   </body>
  * </html>
  */
void EpubExportXhtml::initialize(QString language, QString title, QString styleUrl)
{
    QDomText text;
    QDomElement element;

    const QDomDocumentType doctype = (new QDomImplementation())->createDocumentType("html", "-//W3C//DTD XHTML 1.1//EN", "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd");
    xhtmlDocument = QDomDocument(doctype);

    QDomProcessingInstruction xmlDeclaration = xhtmlDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    xhtmlDocument.appendChild(xmlDeclaration);


    QDomElement xhtmlRoot = xhtmlDocument.createElement("html");
    xhtmlRoot.setAttribute("xmlns", "http://www.w3.org/1999/xhtml");
    // xhtmlRoot.setAttribute("xml:lang", documentMetadata.langInfo());
    xhtmlRoot.setAttribute("xml:lang", language);
    xhtmlDocument.appendChild(xhtmlRoot);

    QDomElement head = xhtmlDocument.createElement("head");
    xhtmlRoot.appendChild(head);

    element = xhtmlDocument.createElement("meta");
    element.setAttribute("http-equiv", "Content-Type");
    element.setAttribute("content", "application/xhtml+xml; charset=utf-8");
    head.appendChild(element);

    element = xhtmlDocument.createElement("title");
    head.appendChild(element);
    // text = xhtmlDocument.createTextNode(documentMetadata.title());
    text = xhtmlDocument.createTextNode(title);
    element.appendChild(text);

    element = xhtmlDocument.createElement("link");
    element.setAttribute("rel", "stylesheet");
    // element.setAttribute("href", "../Styles/style.css");
    element.setAttribute("href", styleUrl);
    element.setAttribute("type", "text/css");
    head.appendChild(element);

    xhtmlBody = xhtmlDocument.createElement("body");
    xhtmlRoot.appendChild(xhtmlBody);
}

void EpubExportXhtml::addContent(QList<QDomElement> content)
{
    foreach (QDomElement paragraph, content)
    {
        // qDebug() << "paragraph" << paragraph;
        xhtmlBody.appendChild(paragraph);
    }
}

/**
 * TODO: refactor and include getFixedXhtml() in here
 */
QString EpubExportXhtml::getString() {
    return getFixedXhtml(xhtmlDocument.toString(-1));
}

/**
 * replace the unicode characters to entities if needed.
 * TODO: we should find a way to correctly insert them in addText() itself, without having to "fix"
 * them afterwards (ale/20120606)
 */
QString EpubExportXhtml::getFixedXhtml(QString xhtml) {

    // don't use unicode non breaking spaces
    xhtml = xhtml.replace(SpecialChars::NBSPACE, "&nbsp;");

    // add some line breaks for readability
    QRegExp pattern("<(html|head|meta|/title|link|/head|body|/body|/html|/p)([^>]*)>");
    xhtml = xhtml.replace(pattern, "<\\1\\2>\n");

    return xhtml;
}

QDebug operator<<(QDebug dbg, const EpubExportXhtml &xhtml)
{
    dbg.nospace() << "(" << "Debug not implemented" << ")";
    return dbg.space();
}

