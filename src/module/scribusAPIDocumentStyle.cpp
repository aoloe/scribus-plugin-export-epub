#include <QDebug>

#include "plugins/scribusAPI/Document.h"

ScribusAPIDocumentStyle::ScribusAPIDocumentStyle()
{
}

ScribusAPIDocumentStyle::~ScribusAPIDocumentStyle()
{
}

// TODO: move here ScribusAPIDocument::getStylesAsCss

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentStyle &scribusDocumentStyle)
{
    dbg.nospace() << "(" << "Debug not implemented" << ")";
    return dbg.space();
}
