/**
 * ScribusAPIDocumentItem is a proxy to the scribus' PageItem and is thought as
 * an interface to get information about the scribus items
 *
 * This file is part of the ScribusAPIDocument plugin
 * (c) GPL 2, Ale Rimoldi <ale@graphicslab.org>
 * For the full copyright and license information, please view the LICENSE
 */
#ifndef SCRIBUSAPIDOCUMENTITEM_H
#define SCRIBUSAPIDOCUMENTITEM_H

#include <QDebug>
#include <QObject>

#include <QVector>
#include <QSize>

class QDomDocument;
class QDomElement;

#include "scribusdoc.h"

#include "module/scribusAPIDocumentItemFormatting.h"

struct ScribusAPIDocumentItemTextRuns
{
    int position;
    int length;
    char type; // p=paragraph, f=formatting
    QVector< QVector<QString> > content;
};

struct ScribusAPIDocumentItemResourceFile
{
    QString filename;
    QString filePath;
    QString fileContent;
};
struct ScribusAPIDocumentItemImageWeb
{
    QString path;
    QString filename;
    QString content;
    QSize imageSize;
    QString description; // for alt text
    QString mediatype;
};

class ScribusAPIDocumentItem : public QObject
{
    Q_OBJECT
public:
	ScribusAPIDocumentItem();
	~ScribusAPIDocumentItem();
    void setPageNumber(int pageNumber) { this->pageNumber = pageNumber; }
    void setItem(PageItem* item) { this->item = item; }
    bool isBefore(ScribusAPIDocumentItem* const item) const;
    /**
     * used by qSort to sort the items by their place on the page
     */
    static bool isBeforeQSort(ScribusAPIDocumentItem* const item1, ScribusAPIDocumentItem* const item2) { /*return true; } // */ return item1->isBefore(item2); }
    int getX() const { return this->item->gXpos; }
    int getY() const { return this->item->gYpos; }

    bool isTextFrame() { return this->item->asTextFrame(); }
    bool isImageFrame() { return this->item->asImageFrame(); }
    bool isTextFirstInChain() { return this->item->prevInChain() == NULL; }
    bool isTextEmpty() { return this->item->itemText.length() == 0; }

    QString getName() { return this->item->itemName(); }

    ScribusAPIDocumentItemFormatting getFormattingAtPosition(int position);

    // QString getXhtmlContent(QDomDocument xhtmlDocument);
    QList<QDomElement> getTextDom(QDomDocument xhtmlDocument);
    ScribusAPIDocumentItemImageWeb getImageWeb();
    QList<ScribusAPIDocumentItemResourceFile> getResourceFiles();

    QVector<ScribusAPIDocumentItemTextRuns> getTextRuns();

protected:
    int pageNumber;
    PageItem* item;

    QString getStylenameSanitized(QString stylename);

};

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItem &scribusDocumentItem);
QDebug operator<<(QDebug dbg, const QVector<ScribusAPIDocumentItemTextRuns> &runs);
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemTextRuns run);
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemResourceFile file);
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemImageWeb image);

#endif //SCRIBUSAPIDOCUMENTITEM_H 
