#include <QDebug>

#include "module/epubexportEpubfile.h"

#include "module/epubexportZip.h"

EpubExportEpubfile::EpubExportEpubfile()
{
}

EpubExportEpubfile::~EpubExportEpubfile()
{
}

void EpubExportEpubfile::create()
{
file = new EpubExportZip();
file->createArchive(filename);

// add mimetype to the current epub file
// The mimetype file must be a text document in ASCII that contains the string application/epub+zip.
// It must also be uncompressed, unencrypted, and the first file in the ZIP archive.
// addUncompressed("mimetype", QString("application/epub+zip"));
}

void EpubExportEpubfile::close()
{
delete file;
}

QDebug operator<<(QDebug dbg, const EpubExportEpubfile &epub)
{
    dbg.nospace() << "(" << "Debug not implemented" << ")";
    return dbg.space();
}
