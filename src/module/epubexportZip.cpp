/**
 * this file extends the the OSDaB (http://osdab.42cows.org/) Zip class
 * to allow zipping in memory variables.
 * the goal is to extend it in a way that it allows to store it without first
 * creating a temp file
 */

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

void EpubExportZip::addString(const QString& path, QByteArray& content)
{
    addString(path, content, EpubExportZip::Deflate9);
}

void EpubExportZip::addString(const QString& path, QByteArray& content, CompressionLevel level)
{
    // TODO: "implement addFile for QByteArray";
    qDebug() << "implement addFile for QByteArray";
}

void EpubExportZip::addString(const QString& path, QString& content, CompressionLevel level)
{
    // TODO: find a way to use in memory files... if not possible, maybe patch the zip library
    // Qresources or named pipes might be the way to go (ale/20140430)
    QTemporaryDir dir;
    if (dir.isValid()) {
        // dir.path() returns the unique directory path
        QString filename = QDir(dir.path()).filePath(QFileInfo(path).fileName());
        // qDebug() << "filename" << filename;
        QFile file(filename);
        if (file.open(QFile::WriteOnly)) {
            QTextStream out(&file);
            out << content;
            file.close();
            addFile(filename, path, level);
        } else {
            qDebug() << "could not open temporary file" << file.fileName();
        }
    }
}
