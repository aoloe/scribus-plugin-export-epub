/**
 * ScribusAPIDocumentItem is a proxy to the scribus' PageItem and is thought as
 * an interface to get information about the scribus items
 */
#ifndef SCRIBUSAPIDOCUMENTITEM_H
#define SCRIBUSAPIDOCUMENTITEM_H

#include <QDebug>
#include <QObject>

#include <QVector>

class QDomDocument;

#include "scribusdoc.h"

#include "module/scribusAPIDocumentItemFormatting.h"

struct ScribusAPIDocumentItemTextRuns
{
    int position;
    int length;
    char type; // p=paragraph, f=formatting
    QVector< QVector<QString> > content;
};

class ScribusAPIDocumentItem : public QObject
{
    Q_OBJECT
public:
	ScribusAPIDocumentItem();
	~ScribusAPIDocumentItem();
    void setItem(PageItem* item) { this->item = item; }
    bool isBefore(ScribusAPIDocumentItem* const item) const;
    /**
     * used by qSort to sort the items by their place on the page
     */
    static bool isBeforeQSort(ScribusAPIDocumentItem* const item1, ScribusAPIDocumentItem* const item2) { /*return true; } // */ return item1->isBefore(item2); }
    int getX() const { return this->item->gXpos; }
    int getY() const { return this->item->gYpos; }

    bool isTextFrame() { return this->item->asTextFrame(); }
    bool isTextFirstInChain() { return this->item->prevInChain() == NULL; }
    bool isTextEmpty() { return this->item->itemText.length() == 0; }

    ScribusAPIDocumentItemFormatting getFormattingAtPosition(int position);

    // QString getXhtmlContent(QDomDocument xhtmlDocument);
    QList<QDomElement> getDomContent(QDomDocument xhtmlDocument);
    QVector<ScribusAPIDocumentItemTextRuns> getTextRuns();

protected:
    PageItem* item;

    QString getStylenameSanitized(QString stylename);

};

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItem &scribusDocumentItem);
QDebug operator<<(QDebug dbg, const QVector<ScribusAPIDocumentItemTextRuns> &runs);
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemTextRuns run);

#endif //SCRIBUSAPIDOCUMENTITEM_H 
