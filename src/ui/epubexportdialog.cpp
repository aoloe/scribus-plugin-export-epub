/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ui/epubexportdialog.h"
#include "epubexport.h"

// for file input widget
#include <QDirModel>
#include <QCompleter>
#include <QFileInfo>
#include "util.h" // for checkFileExtension
// for file chooser
#include "prefsfile.h" // for PrefsContext
#include "prefsmanager.h" // for PrefsManager
#include "scribusdoc.h" // for setting the default filename in the dialog
#include "ui/customfdialog.h"

#include <QList>
#include "ui/createrange.h" // for getting the page ranges

EpubExportDialog::EpubExportDialog(QWidget* parent, ScribusDoc* scribusdoc, const char* name, bool modal, Qt::WindowFlags fl)
	: QDialog (parent, fl),
	  doc(scribusdoc)
{
	setupUi(this);
	setObjectName(name);
	setModal(modal);
    // TODO: the plugin does not run if there is no document active... should we still check that
    // one is there?
    /*
	if (this->doc == 0)
        return;
    */

	//TODO: fill a bug report: what is the policy in scribus to set the initial value for the target? i think that we have multiple of them between the same place as the file or the last time a function has been used in scribus
	QFileInfo basename(this->doc->DocName);
	QString filename = basename.completeBaseName() + ".epub";
	qDebug() << "filename: " << filename;

	fileOutput->setText(filename);
	// fileOutput->setText( QDir::toNativeSeparators(PrefsManager::instance()->prefsFile->getContext("dirs")->get("epub", ".")) );
	// fileOutput->setText( QDir::toNativeSeparators(PrefsManager::instance()->prefsFile->getContext("dirs")->get("wdir", QDir::currentPath())) );

	QDirModel * dirModel = new QDirModel(this);
	dirModel->setFilter(QDir::AllDirs);
	fileOutput->setCompleter(new QCompleter(dirModel, this));

	connect(fileOutput, SIGNAL(editingFinished()), this, SLOT(fileOutputChanged()));
	connect(fileOutputChange, SIGNAL(clicked()), this, SLOT(chooseFile()));
	connect(intervalPagesRadio, SIGNAL(toggled(bool)), this, SLOT(enableIntervalPages(bool)));
	connect(imageMaxWidthCheckbox, SIGNAL(toggled(bool)), this, SLOT(enableImageMaxWidth(bool)));
	connect(intervalPagesButton, SIGNAL(clicked()), this, SLOT(readIntervalFromDialog()));

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_clicked()));
	connect(exportButton, SIGNAL(clicked()), this, SLOT(exportButton_clicked()));

	intervalPagesButton->setToolTip( "<qt>" + tr( "Insert a comma separated list of tokens where "
		                                    "a token can be * for all the pages, 1-5 for "
		                                    "a range of pages or a single page number.") + "</qt>" );
}

EpubExportDialog::~EpubExportDialog() {};

void EpubExportDialog::enableIntervalPages(bool enabled)
{
	intervalPagesValue->setEnabled( enabled );
	intervalPagesButton->setEnabled( enabled );
}

void EpubExportDialog::enableImageMaxWidth(bool enabled)
{
	imageMaxWidthSpinBox->setEnabled( enabled );
	imageMaxWidthLabel->setEnabled( enabled );
	imageMaxWidthThresholdSpinBox->setEnabled( enabled );
	imageMaxWidthThresholdLabel->setEnabled( enabled );
}

/**
 * TODO:
 * Add it to ScribusDoc, remove it from util.cpp and use it in scribus.cpp!
 * Eventually, rename to signify that it does not return xOffset, yOffset, ... but it adds the bleeds
 */
QList<int> EpubExportDialog::getPagesListFromString(const QString pages)
{
    std::vector<int> pageNumbers;
    parsePagesString(pages, &pageNumbers, this->doc->DocPages.count());
    QList<int> result = QList<int>::fromVector(QVector<int>::fromStdVector(pageNumbers));
    return result;
}


void EpubExportDialog::readIntervalFromDialog()
{
    CreateRange cr(intervalPagesValue->text(), this->doc->DocPages.count(), this);
    if (cr.exec())
    {
        CreateRangeData crData;
        cr.getCreateRangeData(crData);
        intervalPagesValue->setText(crData.pageRange);
        return;
    }
	intervalPagesValue->setText(QString::null);
}

void EpubExportDialog::cancelButton_clicked()
{
	reject();
}

/**
 * @brief Read the dialog values and set them in the EpubExportOptions structure.
 */
void EpubExportDialog::exportButton_clicked()
{
    // see gtfiledialog.ui for a way to integrate a file manager
	// qDebug() << "fileOutput->text()" << fileOutput->text();
    QString pagesString = "";
    if (thisPageRadio->isChecked())
        pagesString = QString::number(this->doc->currentPage()->pageNr() + 1);
    else if (intervalPagesRadio->isChecked())
        pagesString = intervalPagesValue->text();
    else if (thisSectionRadio->isChecked())
    {
        int i = this->doc->getSectionKeyForPageIndex(this->doc->currentPage()->pageNr());
        pagesString = QString("%1-%2").arg(this->doc->sections()[i].fromindex + 1).arg(this->doc->sections()[i].toindex + 1);
    }
	// qDebug() << "pagesString" << pagesString;
    options.targetFilename = fileOutput->text();
    options.pageRange = getPagesListFromString(pagesString);
    options.imageMaxWidth = imageMaxWidthSpinBox->value();
    options.imageMaxWidthThreshold = imageMaxWidthThresholdSpinBox->value();
	accept();
}

void EpubExportDialog::chooseFile()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	// TODO: check how to correctly add an epub dir (ale/20120815)
	// QString wdir = prefs->get("wdir", ".");
	QString wdir = dirs->get("epub", ".");
	// FIXME: what is fdNone?) (ale/20120815)
	CustomFDialog dia(this, wdir, tr("Save As"), tr("Epub Files (*.epub);;All Files (*)"), fdNone);

	if (!fileOutput->text().isEmpty())
	{
		QString filename = QDir::fromNativeSeparators(fileOutput->text()); 
		dia.setSelection(filename);
		/*
        QString wdir = prefs->get("wdir", ".");
        QScopedPointer<CustomFDialog> openDia( new CustomFDialog(doc->scMW(), wdir, QObject::tr("Save as"), QObject::tr("%1;;All Files (*)").arg(FormatsManager::instance()->extensionsForFormat(FormatsManager::SVG)), fdHidePreviewCheckBox) );
        openDia->setSelection(getFileNameByPage(doc, doc->currentPage()->pageNr(), "svg"));
        openDia->setExtension("svg");
        openDia->setZipExtension("svgz");
        QCheckBox* compress = new QCheckBox(openDia.data());
        compress->setText( tr("Compress File"));
        compress->setChecked(false);
		*/
	}
	if (dia.exec() == QDialog::Accepted)
	{
		// selectedFile() may return path with native separators
		QString filename = QDir::fromNativeSeparators(dia.selectedFile());
		dirs->set("epub", filename.left(filename.lastIndexOf("/")));
		fileOutput->setText( QDir::toNativeSeparators(filename) );
	}	
}

void EpubExportDialog::fileOutputChanged()
{
	QString filename = checkFileExtension(fileOutput->text(),"epub");
	fileOutput->setText( QDir::toNativeSeparators(filename) );
}
