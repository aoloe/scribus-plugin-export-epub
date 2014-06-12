INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

SET(SCRIBUS_EPUBEXPORT_PLUGIN_UI_SRC
  ui/epubexportdialog.ui
)

SET(SCRIBUS_EPUBEXPORT_PLUGIN_MOC_CLASSES
  epubexportplugin.h
  ui/epubexportdialog.h
  module/epubexportZip.h
  module/epubexportStructure.h
  module/scribusAPIDocument.h
  module/scribusAPIDocumentItem.h
  module/scribusAPIDocumentItemFormatting.h
  module/epubexportContent.h
  module/epubexportXhtml.h
  module/epubexportEpubfile.h
  epubexport.h
)

SET(SCRIBUS_EPUBEXPORT_PLUGIN_SOURCES
  epubexportplugin.cpp
  ui/epubexportdialog.cpp
  module/epubexportZip.cpp
  module/epubexportStructure.cpp
  module/scribusAPIDocument.cpp
  module/scribusAPIDocumentItem.cpp
  module/scribusAPIDocumentItemFormatting.cpp
  module/epubexportContent.cpp
  module/epubexportXhtml.cpp
  module/epubexportEpubfile.cpp
  epubexport.cpp
)

SET(SCRIBUS_EPUBEXPORT_PLUGIN "epubexportplugin")

QT5_WRAP_UI(SCRIBUS_EPUBEXPORT_PLUGIN_UI_SOURCES ${SCRIBUS_EPUBEXPORT_PLUGIN_UI_SRC} )
QT5_WRAP_CPP(SCRIBUS_EPUBEXPORT_PLUGIN_MOC_SOURCES ${SCRIBUS_EPUBEXPORT_PLUGIN_MOC_CLASSES})

ADD_LIBRARY(${SCRIBUS_EPUBEXPORT_PLUGIN} MODULE ${SCRIBUS_EPUBEXPORT_PLUGIN_SOURCES} ${SCRIBUS_EPUBEXPORT_PLUGIN_MOC_SOURCES} ${SCRIBUS_EPUBEXPORT_PLUGIN_UI_CPP} ${SCRIBUS_EPUBEXPORT_PLUGIN_UI_SOURCES})

TARGET_LINK_LIBRARIES(${SCRIBUS_EPUBEXPORT_PLUGIN} ${PLUGIN_LIBRARIES})

INSTALL(TARGETS ${SCRIBUS_EPUBEXPORT_PLUGIN}
  LIBRARY
  DESTINATION ${PLUGINDIR}
  PERMISSIONS ${PLUGIN_PERMISSIONS}
)

ADD_DEPENDENCIES(${SCRIBUS_EPUBEXPORT_PLUGIN} ${EXE_NAME})
