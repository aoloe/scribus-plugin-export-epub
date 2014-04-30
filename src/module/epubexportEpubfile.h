/**
 * EpubExportEpubfile manages the creation and "filling" of the epub file
 */
#ifndef EPUBEXPORTEPUBFILE_H
#define EPUBEXPORTEPUBFILE_H

#include <QObject>
#include <QDebug>

#include "module/epubexportZip.h"

class EpubExportEpubfile : public QObject
{
    Q_OBJECT

public:
	EpubExportEpubfile();
	~EpubExportEpubfile();

    void setFilename(QString filename) {this->filename = filename;}

    void create();
    void close();

private:
	EpubExportZip *file;
    QString filename;
};

QDebug operator<<(QDebug dbg, const EpubExportEpubfile &epub);

#endif // EPUBEXPORTEPUBFILE_H
