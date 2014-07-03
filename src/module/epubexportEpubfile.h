/**
 * EpubExportEpubfile manages the creation and "filling" of the epub file
 */
#ifndef EPUBEXPORTEPUBFILE_H
#define EPUBEXPORTEPUBFILE_H

#include <QObject>
#include <QDebug>

class QByteArray;

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

    void addFile(const QString& filename, const QString& path);
    void addFileUncompressed(const QString& filename, const QString& path);
    void add(QString filename, QString content);
    void addUncompressed(QString filename, QString content);
    void addUncompressed(QString filename, QByteArray content);

private:
	EpubExportZip *file;
    QString filename;
    /**
     * we're only interested in maximal and no compression
     */
    inline EpubExportZip::CompressionLevel getDefaultCompressionLevel(bool compressing)
    {
        return compressing ? EpubExportZip::Deflate9 : EpubExportZip::Store;
    }
};

QDebug operator<<(QDebug dbg, const EpubExportEpubfile &epub);

#endif // EPUBEXPORTEPUBFILE_H
