/**
 * Prepare the image for for the epub Exporting
 */
#ifndef EPUBEXPORTIMAGE_H
#define EPUBEXPORTIMAGE_H

#include <QDebug>

#include <QObject>

class EpubExportImage : public QObject
{
    Q_OBJECT

public:
	EpubExportImage();
	~EpubExportImage();
};

QDebug operator<<(QDebug dbg, const EpubExportImage &image);

#endif //EPUBEXPORTIMAGE_H 
