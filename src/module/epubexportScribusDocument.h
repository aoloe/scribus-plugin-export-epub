/**
 * EpubExportScribusDoc contains a reference to the current scribus document and is used as
 * an interface to get information and parse the scribus document itself
 */
#ifndef EPUBEXPORTSCRIBUSDOCUMENT_H
#define EPUBEXPORTSCRIBUSDOCUMENT_H

#include <QDebug>

#include <QObject>

#include <QList>
#include <QVector>

#include "scribusdoc.h"

#include "module/epubexportScribusDocumentItem.h"

// class ScribusDoc;
// class PageItem;
class ScPage; // for getPageRect() remove it, it's moved to ScPage
class MarginStruct; // for getPageRect() remove it, it's moved to ScPage
class PageItem;

class EpubExportStructureMetadata;
class EpubExportScribusDocumentItem;

class EpubExportScribusDocument : public QObject
{
    Q_OBJECT

public:
	EpubExportScribusDocument();
	~EpubExportScribusDocument();

    void set(ScribusDoc* scribusDoc) { this->scribusDoc = scribusDoc; }
    void setPageRange(QList<int> pageRange) { this->pageRange = pageRange; }

    EpubExportStructureMetadata getMetadata();

    void readItems();
    // QList<EpubExportScribusDocumentItem*> getPageItems(int page) { return this->items[page];}
    QList<EpubExportScribusDocumentItem*> getPageItems(int page) { QList<EpubExportScribusDocumentItem*> result; return result;}

    void readSections();
    QVector< QList<int> > getSections() { return this->sections; }
    QString getStylesAsCss();
    QByteArray getFirstPageAsCoverImage();

    int getPageCount() { return this->scribusDoc->DocPages.count(); }
    int getSectionCount() { return this->scribusDoc->prefsData().docSectionMap.count(); }
private:
    ScribusDoc* scribusDoc;

    QList<int> pageRange; // from the options

    QVector< QList<EpubExportScribusDocumentItem*> > items;
    QVector< QList<int> > sections;

    QString getStylenameSanitized(QString stylename);


    static bool isDocItemTopLeftLessThan(const PageItem *docItem1, const PageItem *docItem2);
    static bool isItemBefore(const EpubExportScribusDocumentItem& item1, const EpubExportScribusDocumentItem& item2);

    // ==== functions to be moved to scribusDoc? =====
    QList<ScPage *> getPagesWithItem(PageItem* item);
    QRect getPageRect(const ScPage* page);
    MarginStruct getPageBleeds(const ScPage* page);
    bool isPortrait(const ScPage* page);
    QList<ScPage*> getPagesList();

};

QDebug operator<<(QDebug dbg, const EpubExportScribusDocument &scribusDocument);

#endif // EPUBEXPORTSCRIBUSDOCUMENT_H
