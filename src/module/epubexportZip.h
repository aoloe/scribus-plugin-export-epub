#ifndef EPUBEXPORTZIP_H
#define EPUBEXPORTZIP_H

#include "third_party/zip/zip.h"

class QString;
class QByteArray;

class EpubExportZip : public Zip
{
public:
    EpubExportZip();
    ~EpubExportZip();
public:
    void addString(const QString& path, QByteArray& content);
    void addString(const QString& path, QByteArray& content, CompressionLevel level);
    void addString(const QString& path, QString& content, CompressionLevel level);
};

#endif // EPUBEXPORTZIP_H
