/**
 * EpubExportScribusDoc contains a reference to the current scribus document and is used as
 * an interface to get information and parse the scribus document itself
 */
#ifndef EPUBEXPORTSCRIBUSDOC_H
#define EPUBEXPORTSCRIBUSDOC_H

#include <QDebug>

#include <QObject>

#include <QList>
#include <QVector>

#include "scribusdoc.h"

// class ScribusDoc;
// class PageItem;
class ScPage; // for getPageRect() remove it, it's moved to ScPage
class MarginStruct; // for getPageRect() remove it, it's moved to ScPage

class EpubExportStructureMetadata;

class EpubExportScribusDoc : public QObject
{
    Q_OBJECT

public:
	EpubExportScribusDoc();
	~EpubExportScribusDoc();

    void set(ScribusDoc* scribusDoc) { this->scribusDoc = scribusDoc; }
    void setPageRange(QList<int> pageRange) { this->pageRange = pageRange; }

    EpubExportStructureMetadata getMetadata();

    void readItems();

    int getPageCount() { return this->scribusDoc->DocPages.count(); }
private:
    ScribusDoc* scribusDoc;

    QList<int> pageRange; // from the options

    QVector< QList<PageItem*> > items;
    QList<ScPage *> getPagesWithItem(PageItem* item);
    QRect getPageRect(const ScPage* page);
    MarginStruct getPageBleeds(const ScPage* page);
};

QDebug operator<<(QDebug dbg, const EpubExportScribusDoc &scribusDoc);

#endif // EPUBEXPORTSCRIBUSDOC_H
