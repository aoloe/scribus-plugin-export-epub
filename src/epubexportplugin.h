 /**
 * This is the entry point for the plugin.
 * All the methods defined here are called by Scribus' plugin execution code.
 */

#ifndef EPUBEXPORTPLUGIN_H
#define EPUBEXPORTPLUGIN_H

#include "pluginapi.h"
#include "scplugin.h"

class QString;
class ScribusDoc;
class ScribusMainWindow;

/**
 * This is the standard entry point for the plugin and is automatically loaded by Scribus.
 * @brief Standard entry point for the plugin; Attaches itself to the menu, shows the "Export" dialog and calls `EpubExport::doExport()`
 */
class PLUGIN_API EpubExportPlugin : public ScActionPlugin
{
	Q_OBJECT
public:

	// Standard plugin implementation
	EpubExportPlugin();
	virtual ~EpubExportPlugin();
	/*!
	\author Ale Rimoldi
	\brief Run the EPUB export
	\param filename a file to export to
	\retval bool true
	*/
	virtual bool run(ScribusDoc* doc=0, QString filename = QString::null);
	virtual const QString fullTrName() const;
	virtual const AboutData* getAboutData() const;
	virtual void deleteAboutData(const AboutData* about) const;
	virtual void languageChange();
	virtual void addToMainWindowMenu(ScribusMainWindow *) {};
};

extern "C" PLUGIN_API int epubexportplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* epubexportplugin_getPlugin();
extern "C" PLUGIN_API void epubexportplugin_freePlugin(ScPlugin* plugin);

#endif // EPUBEXPORTPLUGIN_H
