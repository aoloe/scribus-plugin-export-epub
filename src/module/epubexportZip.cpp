/**
 * this file extends the the OSDaB (http://osdab.42cows.org/) Zip class
 * to allow zipping in memory variables.
 * the goal is to extend it in a way that it allows to store it without first
 * creating a temp file
 */

#include <QDebug>
#include <QString>
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
    // TODO: "implement addFile for QString";
    qDebug() << "implement addFile for QString";
}
