#include <QDebug>

#include <QByteArray>
#include <QFileInfo>

#include "module/epubexportEpubfile.h"

#include "module/epubexportZip.h"

EpubExportEpubfile::EpubExportEpubfile()
{
}

EpubExportEpubfile::~EpubExportEpubfile()
{
}

/**
 * @brief Create the internal EpubExportZip file
 */
void EpubExportEpubfile::create()
{
    file = new EpubExportZip();
    file->createArchive(filename);
    // add mimetype to the current epub file
    // The mimetype file must be a text document in ASCII that contains the string application/epub+zip.
    // It must also be uncompressed, unencrypted, and the first file in the ZIP archive.
	addUncompressed("mimetype", QString("application/epub+zip"));
}

void EpubExportEpubfile::close()
{
delete file;
}

/**
 * @brief Add a file compressed.
 * @param filename Path to the file in the Epub file
 * @param path Path to the file to be added
 */
void EpubExportEpubfile::addFile(const QString& filename, const QString& path)
{
    QFileInfo fileinfo(filename);
    file->addFile(path, fileinfo.path(), getDefaultCompressionLevel(true));
}

/**
 * @brief Add a file uncompressed. You need this for images.
 * @param filename Path to the file in the Epub file
 * @param path Path to the file to be added
 */
void EpubExportEpubfile::addFileUncompressed(const QString& filename, const QString& path)
{
    QFileInfo fileinfo(filename);
    file->addFile(path, fileinfo.path(), getDefaultCompressionLevel(false));
}

/**
 * @brief Add a string compressed. You need this for Html content.
 * @param filename Path to the file in the Epub file
 * @param content The content of the file to be added
 */
void EpubExportEpubfile::add(QString filename, QString content)
{
    file->addString(filename, content, getDefaultCompressionLevel(true));
}

/**
 * @brief Add a string uncompressed. You need this for the manifest.
 * @param filename Path to the file in the Epub file
 * @param content The content of the file to be added
 */
void EpubExportEpubfile::addUncompressed(QString filename, QString content)
{
    file->addString(filename, content, getDefaultCompressionLevel(false));
}

void EpubExportEpubfile::addUncompressed(QString filename, QByteArray content)
{
    file->addByteArray(filename, content, getDefaultCompressionLevel(false));
}

QDebug operator<<(QDebug dbg, const EpubExportEpubfile &epub)
{
    dbg.nospace() << "(" << "Debug not implemented" << ")";
    return dbg.space();
}
