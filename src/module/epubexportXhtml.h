/**
 * collects each xhtml structure for the chapters of the epub file
 */
#ifndef EPUBEXPORTXHTML_H
#define EPUBEXPORTXHTML_H

#include <QObject>
#include <QDebug>

#include <QDomElement>

class QString;

class EpubExportXhtml : public QObject
{
    Q_OBJECT

public:
	EpubExportXhtml();
	~EpubExportXhtml();
    void initialize(QString language, QString title, QString styleUrl);
    QString getString();
private:
    QDomDocument xhtmlDocument;
    QDomElement xhtmlRoot;
    QDomElement xhtmlBody;

    QString getFixedXhtml(QString xhtml);

};

QDebug operator<<(QDebug dbg, const EpubExportXhtml xhtml);

#endif // EPUBEXPORTXHTML_H

