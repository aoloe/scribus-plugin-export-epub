// Extend the Zip class and allow zipping strings instead of files

#include <QDebug>
#include <QString>
#include <QTemporaryDir>
#include <QFile>
#include <QByteArray>
#include "module/epubexportZip.h"

EpubExportZip::EpubExportZip()
    : Zip()
{
}
EpubExportZip::~EpubExportZip()
{
}

/**
 * @param level Zip::Deflate1 (minimum compression) .. Zip::Deflate9 (maximum compression); Zip::Store (no compression)
 * @param path The path inside of the zip file
 * @param content The stream of characters to be stored as a file
 * @todo:
 * - shouldn't the temporary file be deleted?
 */
void EpubExportZip::addString(const QString& path, QString& content, CompressionLevel level)
{
    // TODO: find a way to use in memory files... if not possible, maybe patch the zip library
    // Qresources or named pipes might be the way to go (ale/20140430)
    QTemporaryDir dir;
    if (dir.isValid()) {
        // dir.path() returns the unique directory path
        QFileInfo fileinfo(path);
        QString filename = QDir(dir.path()).filePath(fileinfo.fileName());
        // qDebug() << "filename" << filename;
        QFile file(filename);
        if (file.open(QFile::WriteOnly)) {
            QTextStream out(&file);
            out << content;
            file.close();
            addFile(filename, fileinfo.path(), level);
        } else {
            qDebug() << "could not open temporary file" << file.fileName();
        }
    }
}

/**
 * @param level if you don't set a level, maximum compression will be level: you will probably only want
 * to set it, if you don't want any compression (`Zip::Store`).
 * @param path The path inside of the zip file
 * @param content The stream of characters to be stored as a file
 * @todo:
 * - shouldn't the temporary file be deleted?
 */
void EpubExportZip::addByteArray(const QString& path, QByteArray& content, CompressionLevel level)
{
    // TODO: find a way to use in memory files... if not possible, maybe patch the zip library
    // Qresources or named pipes might be the way to go (ale/20140430)
    QTemporaryDir dir;
    if (dir.isValid()) {
        // dir.path() returns the unique directory path
        QFileInfo fileinfo(path);
        QString filename = QDir(dir.path()).filePath(fileinfo.fileName());
        // qDebug() << "filename" << filename;
        QFile file(filename);
        if (file.open(QFile::WriteOnly)) {
            file.write(content);
            file.close();
            addFile(filename, fileinfo.path(), level);
        } else {
            qDebug() << "could not open temporary file" << file.fileName();
        }
    }
}
