/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
    begin                : Fri Dec 29 18:30:00 CEST 2012
    copyright            : (C) 2012 by Ale Rimoldi
    email                : a.l.e@ideale.ch

 ***************************************************************************/

/**
 * Store information related to the structure of content
 * and return the strings for the "structural" files for the .epub file
 */

#include <QDebug>
#include <QUuid> // for generated the uuid if no isbn&co has been defined

#include <QDomDocument> // for creating OPF files

#include "module/epubexportStructure.h"
#include "scribuscore.h" // for reading the gui language

#include "documentinformation.h"

EpubExportStructure::EpubExportStructure()
{
    cover = QByteArray();
}

EpubExportStructure::~EpubExportStructure()
{
}

void EpubExportStructure::readMetadata(EpubExportStructureMetadata metadata)
{
    this->metadata = metadata;
	if (this->metadata.title == "")
		this->metadata.title = filename;
	// TODO: if (documentMetadata.author() == "") // -> it's recommended not obligatory!
	// TODO: if (documentMetadata.authorSort() == "") // -> it's recommended not obligatory!
	if (metadata.language == "")
		metadata.language = ScCore->getGuiLanguage();
	if (metadata.language == "")
		metadata.language = "en_GB"; // scribus' default language is english (or rather en-GB?)
	if (metadata.id == "")
		metadata.id = "urn:uuid:"+QUuid::createUuid().toString().remove("{" ).remove("}" ); // Sigil/Misc/Utility.cpp -> Utility::CreateUUID()
	// TODO: store the generated uuid in the scribus document information?
	if (metadata.date == "")
		metadata.date = QDate::currentDate().toString(Qt::ISODate);
}

void EpubExportStructure::addContent(QString id, QString path, QString mediatype)
{
        EpubExportStructureContent content = EpubExportStructureContent();
        content.id = id;
        content.filename = path;
        content. mediatype = mediatype;
        addContent(content);
}

/**
  * creates the xml string for the OPF file
  * The OPF file, traditionally named content.opf, houses the EPUB book's metadata,
  * file manifest, and linear reading order.
  * on the IDPF site you can find the full list of the possible metadata
  * http://idpf.org/epub/20/spec/OPF_2.0.1_draft.htm#Section2.2
  * <?xml version="1.0"?>
  * <package version="2.0" xmlns="http://www.idpf.org/2007/opf" unique-identifier="BookId">
  *   <metadata xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:opf="http://www.idpf.org/2007/opf">
  *     <dc:title>Pride and Prejudice</dc:title>
  *     <dc:language>en</dc:language>
  *     <dc:identifier id="BookId" opf:scheme="ISBN">123456789X</dc:identifier>
  *     <dc:creator opf:file-as="Austen, Jane" opf:role="aut">Jane Austen</dc:creator>
  *   </metadata>
  *   <manifest>
  *     <item id="chapter1" href="chapter1.xhtml" media-type="application/xhtml+xml"/>
  *     <item id="stylesheet" href="style.css" media-type="text/css"/>
  *     <item id="ch1-pic" href="ch1-pic.png" media-type="image/png"/>
  *     <item id="myfont" href="css/myfont.otf" media-type="application/x-font-opentype"/>
  *     <item id="ncx" href="toc.ncx" media-type="application/x-dtbncx+xml"/>
  *   </manifest>
  *   <spine toc="ncx">
  *     <itemref idref="chapter1" />
  *   </spine>
  *   <guide>
  *     <reference type="loi" title="List Of Illustrations" href="appendix.html#figures" />
  *   </guide>
  * </package>
  */
QString EpubExportStructure::getOPF()
{
	QDomDocument xmlDocument = QDomDocument();
	QDomElement element;
	QDomText text;

	QDomProcessingInstruction xmlDeclaration = xmlDocument.createProcessingInstruction("xml", "version=\"1.0\"");
	xmlDocument.appendChild(xmlDeclaration);

	QDomElement xmlRoot = xmlDocument.createElement("package");
	xmlRoot.setAttribute("version", "2.0");
	xmlRoot.setAttribute("xmlns", "http://www.idpf.org/2007/opf");
	xmlRoot.setAttribute("unique-identifier", "BookId");
	xmlDocument.appendChild(xmlRoot);

	QDomElement elementMetadata = xmlDocument.createElement("metadata");
	elementMetadata.setAttribute("xmlns:dc", "http://purl.org/dc/elements/1.1/");
	elementMetadata.setAttribute("xmlns:opf", "http://www.idpf.org/2007/opf");
	xmlRoot.appendChild(elementMetadata);

	element = xmlDocument.createElement("dc:title");
	text = xmlDocument.createTextNode(metadata.title);
	element.appendChild(text);
	elementMetadata.appendChild(element);

	element = xmlDocument.createElement("dc:language");
	text = xmlDocument.createTextNode(metadata.language);
	element.appendChild(text);
	elementMetadata.appendChild(element);

	element = xmlDocument.createElement("dc:identifier");
	element.setAttribute("id", "BookId");
	element.setAttribute("opf:scheme", "UUID");
	text = xmlDocument.createTextNode(metadata.id);
	element.appendChild(text);
	elementMetadata.appendChild(element);

	element = xmlDocument.createElement("dc:creator");
	element.setAttribute("opf:file-as", metadata.author); // TODO: use for the to be created authorFileAs / authorSort
	element.setAttribute("opf:role", "aut");
	text = xmlDocument.createTextNode(metadata.author);
	element.appendChild(text);
	elementMetadata.appendChild(element);

	element = xmlDocument.createElement("dc:date");
	element.setAttribute("opf:event", "publication");
	text = xmlDocument.createTextNode(metadata.date);
	element.appendChild(text);
	elementMetadata.appendChild(element);

	// non mandatory fields
	if (metadata.subject != "")
	{
        element = xmlDocument.createElement("dc:subject");
        text = xmlDocument.createTextNode(metadata.subject);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	if (metadata.keywords != "")
	{
        element = xmlDocument.createElement("dc:subject");
        text = xmlDocument.createTextNode(metadata.keywords);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	if (metadata.description != "")
	{
        element = xmlDocument.createElement("dc:description");
        text = xmlDocument.createTextNode(metadata.description);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	if (metadata.publisher != "")
	{
		element = xmlDocument.createElement("dc:publisher");
		text = xmlDocument.createTextNode(metadata.publisher);
		element.appendChild(text);
		elementMetadata.appendChild(element);
	}

	if (metadata.contributor != "")
	{
        element = xmlDocument.createElement("dc:contributor");
        text = xmlDocument.createTextNode(metadata.contributor);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	if (metadata.type != "")
	{
        element = xmlDocument.createElement("dc:type");
        text = xmlDocument.createTextNode(metadata.type);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	if (metadata.format != "")
	{
        element = xmlDocument.createElement("dc:format");
        text = xmlDocument.createTextNode(metadata.format);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	if (metadata.source != "")
	{
        
        element = xmlDocument.createElement("dc:source");
        text = xmlDocument.createTextNode(metadata.source);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	if (metadata.relation != "")
	{
        element = xmlDocument.createElement("dc:relation");
        text = xmlDocument.createTextNode(metadata.relation);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	if (metadata.coverage != "") // that's not the cover, but the coverage... space and time covered by the work
	{
        element = xmlDocument.createElement("dc:coverage");
        text = xmlDocument.createTextNode(metadata.coverage);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	if (metadata.rights != "")
	{
        element = xmlDocument.createElement("dc:rights");
        text = xmlDocument.createTextNode(metadata.rights);
        element.appendChild(text);
        elementMetadata.appendChild(element);
	}

	element = xmlDocument.createElement("meta");
	// element.setAttribute("properties", "cover-image");
	element.setAttribute("content", "cover.png");
	element.setAttribute("name", "cover");
	elementMetadata.appendChild(element);

	QDomElement manifest = xmlDocument.createElement("manifest");
	xmlRoot.appendChild(manifest);

	// dynamically add the content items (contentItems.add(EPUBExportContentItem))
    foreach (EpubExportStructureContent contentItem, content)
	{
		element = xmlDocument.createElement("item");
		element.setAttribute("id", contentItem.id);
		element.setAttribute("href", contentItem.href);
		element.setAttribute("media-type", contentItem.mediatype);
		manifest.appendChild(element);
	}

	// TODO: dynamically add the fonts
	// <item id="myfont" href="css/myfont.otf" media-type="application/x-font-opentype"/>

	element = xmlDocument.createElement("item");
	element.setAttribute("id", "ncx"); // TODO: set chapter name
	element.setAttribute("href", "toc.ncx");
	element.setAttribute("media-type", "application/x-dtbncx+xml");
	manifest.appendChild(element);

	QDomElement spine = xmlDocument.createElement("spine");
	spine.setAttribute("toc", "ncx");
	xmlRoot.appendChild(spine);

    foreach (EpubExportStructureContent contentItem, content)
    {
		element = xmlDocument.createElement("itemref");
		element.setAttribute("idref", filename);
		spine.appendChild(element);
	}

	QDomElement guide = xmlDocument.createElement("guide");
	xmlRoot.appendChild(guide);

	// TODO: what goes into the reference?
	/*
	element = xmlDocument.createElement("reference");
	element.setAttribute("type", "loi");
	element.setAttribute("title", "List of Illustrations");
	element.setAttribute("href", "appendix.html#figures");
	manifest.appendChild(element);
	*/

    return xmlDocument.toString();
}

/**
  * creates the xml string for the NCX file
  * <?xml version="1.0" encoding="UTF-8"?>
  * <!DOCTYPE ncx PUBLIC "-//NISO//DTD ncx 2005-1//EN"
  * "http://www.daisy.org/z3986/2005/ncx-2005-1.dtd">
  * <ncx version="2005-1" xml:lang="en" xmlns="http://www.daisy.org/z3986/2005/ncx/">
  *   <head>
  *     <meta name="dtb:uid" content="123456789X"/> <!-- same as in .opf -->
  *     <meta name="dtb:depth" content="1"/> <!-- 1 or higher -->
  *     <meta name="dtb:totalPageCount" content="0"/> <!-- must be 0 -->
  *     <meta name="dtb:maxPageNumber" content="0"/> <!-- must be 0 -->
  *   </head>
  *   <docTitle>
  *     <text>Pride and Prejudice</text>
  *   </docTitle>
  *   <docAuthor>
  *     <text>Austen, Jane</text>
  *   </docAuthor>
  *   <navMap>
  *     <navPoint class="chapter" id="chapter1" playOrder="1">
  *       <navLabel><text>Chapter 1</text></navLabel>
  *       <content src="chapter1.xhtml"/>
  *     </navPoint>
  *   </navMap>
  * </ncx>
  */ 
QString EpubExportStructure::getNCX()
{
    const QDomDocumentType doctype = (new QDomImplementation())->createDocumentType("ncx", "-//NISO//DTD ncx 2005-1//EN", "http://www.daisy.org/z3986/2005/ncx-2005-1.dtd");
    QDomDocument xmlDocument = QDomDocument(doctype);

	QDomElement element;
	QDomElement elementText;
	QDomText text;

	QDomProcessingInstruction xmlDeclaration = xmlDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
	xmlDocument.appendChild(xmlDeclaration);

	QDomElement xmlRoot = xmlDocument.createElement("ncx");
	xmlRoot.setAttribute("version", "2005-1");
	xmlRoot.setAttribute("xml:lang", metadata.language);
	xmlRoot.setAttribute("xmlns", "http://www.daisy.org/z3986/2005/ncx/");
	xmlDocument.appendChild(xmlRoot);

    QDomElement head = xmlDocument.createElement("head");
    xmlRoot.appendChild(head);

	element = xmlDocument.createElement("meta");
	element.setAttribute("name", "dtb:uid");
	element.setAttribute("content", metadata.id);
	head.appendChild(element);

	element = xmlDocument.createElement("meta");
	element.setAttribute("name", "dtb:depth");
	element.setAttribute("content", "1");  // 1 or higher // TODO: set it to the correct value
	head.appendChild(element);

	element = xmlDocument.createElement("meta");
	element.setAttribute("name", "dtb:totalPageCount");
	element.setAttribute("content", "0");  // must be 0
	head.appendChild(element);

	element = xmlDocument.createElement("meta");
	element.setAttribute("name", "dtb:maxPageNumber");
	element.setAttribute("content", "0"); // must be 0
	head.appendChild(element);

    element = xmlDocument.createElement("docTitle");
    xmlRoot.appendChild(element);
	elementText = xmlDocument.createElement("text");
	element.appendChild(elementText);
	text = xmlDocument.createTextNode(metadata.title);
	elementText.appendChild(text);

    element = xmlDocument.createElement("docAuthor");
    xmlRoot.appendChild(element);
	elementText = xmlDocument.createElement("text");
	element.appendChild(elementText);
	text = xmlDocument.createTextNode(metadata.author);
	elementText.appendChild(text);

	QDomElement nav = xmlDocument.createElement("navMap");
	xmlRoot.appendChild(nav);

    int i = 0; // TODO: shouldn't i be in EpubExportStructureContent?
    foreach (EpubExportStructureContent contentItem, content)
    {
		QDomElement navPoint = xmlDocument.createElement("navPoint");
		navPoint.setAttribute("class", "chapter");
		navPoint.setAttribute("id", contentItem.filename);
		navPoint.setAttribute("playOrder", i + 1);
		nav.appendChild(navPoint);

		element = xmlDocument.createElement("navLabel");
		navPoint.appendChild(element);
		elementText = xmlDocument.createElement("text");
		element.appendChild(elementText);
		text = xmlDocument.createTextNode(contentItem.title);
		elementText.appendChild(text);

		element = xmlDocument.createElement("content");
		element.setAttribute("src", "Text/" + contentItem.filename);
		navPoint.appendChild(element);
        i++;
	}

    return xmlDocument.toString();
}

/**
  * add META-INF/container.xml to the current epub file
  * <?xml version="1.0" encoding="UTF-8" ?>
  * <container version="1.0" xmlns="urn:oasis:names:tc:opendocument:xmlns:container">
  *   <rootfiles>
  * 	<rootfile full-path="OEBPS/content.opf" media-type="application/oebps-package+xml"/>
  *   </rootfiles>
  * </container>
  */ 
QString EpubExportStructure::getContainer()
{
	QDomDocument xmlDocument = QDomDocument();
	QDomElement element;

	QDomProcessingInstruction xmlDeclaration = xmlDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
	xmlDocument.appendChild(xmlDeclaration);

	QDomElement xmlRoot = xmlDocument.createElement("container");
	xmlRoot.setAttribute("version", "1.0");
	xmlRoot.setAttribute("xmlns", "urn:oasis:names:tc:opendocument:xmlns:container");
	xmlDocument.appendChild(xmlRoot);

	QDomElement rootfiles = xmlDocument.createElement("rootfiles");
	xmlRoot.appendChild(rootfiles);

	element = xmlDocument.createElement("rootfile");
	element.setAttribute("full-path", "OEBPS/content.opf");
	element.setAttribute("media-type", "application/oebps-package+xml");
	rootfiles.appendChild(element);

    return xmlDocument.toString();
}

QByteArray EpubExportStructure::getCover()
{
    return cover;
}

QDebug operator<<(QDebug dbg, const EpubExportStructure &structure)
{
    dbg.nospace() << "(" << "Debug not implemented" << ")";
    return dbg.space();
}
QDebug operator<<(QDebug dbg, const EpubExportStructureMetadata &metadata)
{
    QStringList output;
    dbg.nospace() << "( title:" << metadata.title << "\n"
                  << ", author:" << metadata.author << "\n"
                  << ", subject:" << metadata.subject << "\n"
                  << ", date:" << metadata.date << "\n"
                  << ", id:" << metadata.id << "\n"
                  << ", language:" << metadata.language << "\n"
                  << ", keywords:" << metadata.keywords << "\n"
                  << ", description:" << metadata.description << "\n"
                  << ", publisher:" << metadata.publisher << "\n"
                  << ", contributor:" << metadata.contributor << "\n"
                  << ", type:" << metadata.type << "\n"
                  << ", format:" << metadata.format << "\n"
                  << ", source:" << metadata.source << "\n"
                  << ", relation:" << metadata.relation << "\n"
                  << ", coverage:" << metadata.coverage << "\n"
                  << ", rights:" << metadata.rights << "\n"
                  << " )";
    return dbg.space();
}
QDebug operator<<(QDebug dbg, const EpubExportStructureContent &content)
{
    dbg.nospace() << "( title:" << content.title << "\n"
                  << ", id:" << content.id << "\n"
                  << ", href:" << content.href << "\n"
                  << ", mediatype:" << content.mediatype << "\n"
                  << ", title:" << content.title << "\n"
                  << " )";
    return dbg.space();
}
