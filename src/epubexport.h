/**
 * This is the main epub export class.
 * It gets called from the plugin class after the options have been set in the dialog
 */
#ifndef EPUBEXPORT_H
#define EPUBEXPORT_H

#include <QObject>

class ScribusDoc;
class MultiProgressDialog;

/**
 * @brief The options for the Epub export, as defined in the Export dialog.
 */
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

/**
 * @brief This is the main epub export class. It gets called by `EpubExportPlugin` after the the export dialog.
 *
 * Inspired by svgexplugin.
 */
class EpubExport : public QObject
{
    Q_OBJECT

public:
	EpubExport();
	~EpubExport();

	EpubExportOptions options;
    void setOptions(EpubExportOptions options) { this->options = options; }

    void setScribusDocument(ScribusDoc* scribusDoc) { this->scribusDoc = scribusDoc; }
	void doExport();

    void setProgressDialog(MultiProgressDialog* dialog) { progressDialog = dialog; }

public slots:
    void cancelRequested();

private:
    MultiProgressDialog* progressDialog;

    ScribusDoc* scribusDoc;

};

QDebug operator<<(QDebug dbg, const EpubExportOptions options);

#endif // EPUBEXPORT_H
