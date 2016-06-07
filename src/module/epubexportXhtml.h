/**
 * collects each xhtml structure for the chapters of the epub file
 */
#ifndef EPUBEXPORTXHTML_H
#define EPUBEXPORTXHTML_H

#include <QObject>
#include <QDebug>

#include <QDomElement>
#include <QList>

class QString;

class ScribusAPIDocumentItemImageWeb;

/**
 * @brief collect the content of the Xhtml files that are to be added to the epub file.
 */
class EpubExportXhtml : public QObject
{
    Q_OBJECT

public:
	EpubExportXhtml();
	~EpubExportXhtml();
    void initialize(QString language, QString title, QString styleUrl);
    QString getString();
    QDomDocument getQDomDocument() { return xhtmlDocument; }
    void addContentText(QList<QDomElement> content);
    void addContentImage(ScribusAPIDocumentItemImageWeb image);
private:
    QDomDocument xhtmlDocument;
    QDomElement xhtmlRoot;
    QDomElement xhtmlBody;

    QString getFixedXhtml(QString xhtml);

};

QDebug operator<<(QDebug dbg, const EpubExportXhtml xhtml);

#endif // EPUBEXPORTXHTML_H

