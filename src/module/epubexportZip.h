#ifndef EPUBEXPORTZIP_H
#define EPUBEXPORTZIP_H

#include "third_party/zip/zip.h"

class QString;
class QByteArray;

/**
 * Extends the OSDaB (http://osdab.42cows.org/) Zip class and allow zipping of character streams.
 *
 * Currently, temporary files are used, but the goal is to allow real in memory zipping.
 *
 * This  class is used by `EpubExportEpubfile` to add the files to the Epub file.
 *
 * @brief Extend the Zip class and allow zipping strings instead of files
 */
class EpubExportZip : public Zip
{
public:
    EpubExportZip();
    ~EpubExportZip();
public:
    void addString(const QString& path, QString& content, CompressionLevel level);
    void addByteArray(const QString& path, QByteArray& content, CompressionLevel level);
};

#endif // EPUBEXPORTZIP_H
