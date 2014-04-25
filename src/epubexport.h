/**
 * This is the main epub export class.
 * It gets called from the plugin class after the options have been set in the dialog
 */
#ifndef EPUBEXPORT_H
#define EPUBEXPORT_H

#include <QObject>

class ScribusDoc;
class MultiProgressDialog;

class EpubExportScribusDoc;

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

    void addScribusDocument(ScribusDoc* doc);
	void doExport();

    void setProgressDialog(MultiProgressDialog* dialog) {progressDialog = dialog;}

private:
    MultiProgressDialog* progressDialog;

    EpubExportScribusDoc* doc;

};
#endif // EPUBEXPORT_H
