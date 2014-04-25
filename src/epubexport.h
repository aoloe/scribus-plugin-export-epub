/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EPUBEXPORT_H
#define EPUBEXPORT_H

#include <QObject>

class ScribusDoc;
class MultiProgressDialog;

struct EpubExportOptions
{
    QString targetFilename;
    QList<int> pageRange;
    int imageMaxWidth;
    int imageMaxWidthThreshold;
    EpubExportOptions()
    {
        targetFilename = "";
        imageMaxWidth = 0;
        imageMaxWidthThreshold = 0;
    }
};

class EpubExport : public QObject
/*
 * Inspired by svgexplugin
 */
{
    Q_OBJECT

public:
	EpubExport();
	~EpubExport();

	EpubExportOptions options;
    void setOptions(EpubExportOptions options) {this->options = options;}

    void setScribusDocument(ScribusDoc* doc);
	void doExport();

    void setProgressDialog(MultiProgressDialog* dialog) {progressDialog = dialog;}

private:
    MultiProgressDialog* progressDialog;

    // EpubExportScribusDoc* doc;

};
#endif // EPUBEXPORT_H
