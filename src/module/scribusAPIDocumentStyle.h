/**
 * ScribusAPIDocumentStyle gives access to the styles in the current document
 */
#ifndef SCRIBUSAPIDOCUMENTSTYLE_H
#define SCRIBUSAPIDOCUMENTSTYLE_H

#include <QDebug>

#include <QObject>

#include "scribusdoc.h"

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentStyle &scribusDocumentStyle);

class ScribusAPIDocumentStyle : public QObject
{
    Q_OBJECT

public:
	ScribusAPIDocumentStyle();
	~ScribusAPIDocumentStyle();

#endif // SCRIBUSAPIDOCUMENT_H 
