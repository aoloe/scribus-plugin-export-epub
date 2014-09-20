#include <QDebug>

#include <QRegExp>

#include <QDomElement>

#include "styles/charstyle.h"

#include "module/scribusAPIDocumentItem.h"
#include "module/scribusAPIDocumentItemFormatting.h"

ScribusAPIDocumentItemFormatting::ScribusAPIDocumentItemFormatting()
{
}

ScribusAPIDocumentItemFormatting::~ScribusAPIDocumentItemFormatting()
{
}


void ScribusAPIDocumentItemFormatting::readAtPosition(PageItem* item, int position)
{

    // paragraphStyleName = item->itemText.paragraphStyle(position + 1).parent(); // TODO: + 1 might be dangerous!
    // characterStyleName = item->itemText.charStyle(position + 1).displayName();
    qDebug() << "position" << position;
    qDebug() << "paragraphStyle" << item->itemText.paragraphStyle(position + 1).displayName();
    ParagraphStyle paragraphStyle = item->itemText.paragraphStyle(position + 1);
    paragraphStyleName = "";
    /*
            <PAGEOBJECT XPOS="" YPOS="" OwnPage="0" ItemID="" PTYPE="4" WIDTH="" HEIGHT="" FRTYPE="0" CLIPEDIT="0" PWIDTH="1" PLINEART="1" LOCALSCX="1" LOCALSCY="1" LOCALX="0" LOCALY="0" LOCALROT="0" PICART="1" SCALETYPE="1" RATIO="1" TXTFILL="FromSVG#f57900" COLUMNS="1" COLGAP="0" AUTOTEXT="0" EXTRA="0" TEXTRA="0" BEXTRA="0" REXTRA="0" VAlign="0" FLOP="0" PLTSHOW="0" BASEOF="0" textPathType="0" textPathFlipped="0" path="M0 0 L286.933 0 L286.933 22.7579 L0 22.7579 L0 0 Z" copath="M0 0 L286.933 0 L286.933 22.7579 L0 22.7579 L0 0 Z" gXpos="560.09905511811" gYpos="498.424488188977" gWidth="0" gHeight="0" ALIGN="0" LAYER="0" NEXTITEM="-1" BACKITEM="-1">
            <ITEXT CPARENT="Sous-titre" FONT="Gillius ADF Bold" FCOLOR="Black" CH="x"/>
                <trail ALIGN="0"/>
                </PAGEOBJECT>
    */

    if (paragraphStyle.displayName() != "")
        paragraphStyleName = paragraphStyle.parent(); // TODO: + 1 might be dangerous!
    characterStyleName = item->itemText.charStyle(position + 1).displayName();

    const CharStyle& style(item->itemText.charStyle(position));
    style.validate();

    QString fontname = style.font().scName();
    // qDebug() << "fontname:" << fontname;

    QStringList characterFeatures = style.features();
    // qDebug() << "characterFeatures" << characterFeatures;

    // as long as bold and italic are not in the features list, get the property
    // by guessing from the font name (ale/20120916)
    QRegExp regexpItalic("(\\bitalic\\b)"); // TODO: use other keywords for italics (also for style.css)
    regexpItalic.setCaseSensitivity(Qt::CaseInsensitive);
    if (regexpItalic.indexIn(fontname) >= 0)
        characterFeatures << CharStyle::ITALIC;

    QRegExp regexpBold("(\\bbold\\b)"); // TODO: use also other keywords for bold (also for style.css)
    regexpBold.setCaseSensitivity(Qt::CaseInsensitive);
    if (regexpBold.indexIn(fontname) >= 0)
        characterFeatures << CharStyle::BOLD;
    // qDebug() << "characterFeatures" << characterFeatures;

    QDomElement element;
    formatting = 0;
    // QStringList characterFormatting;

    foreach (QString feature, characterFeatures)
    {
        feature = feature.trimmed();
        qDebug() << "feature" << feature;
        if (feature == CharStyle::BOLD)
            formatting = formatting | formattingFlag_Bold;
        else if (feature == CharStyle::ITALIC)
            formatting = formatting | formattingFlag_Italic;
        else if (feature == CharStyle::SUPERSCRIPT)
            formatting = formatting | formattingFlag_Superscript;
        else if (feature == CharStyle::SUBSCRIPT)
            formatting = formatting | formattingFlag_Subscript;
        else if ((feature == CharStyle::UNDERLINE) || (feature == CharStyle::UNDERLINEWORDS))
            formatting = formatting | formattingFlag_Underline;
        else if (feature == CharStyle::STRIKETHROUGH)
            formatting = formatting | formattingFlag_Strikethrough;
        else if (feature != CharStyle::INHERIT)
            qDebug() << "else feature" << feature;
            /*
             * The following character formats are supported by Scribus
             * but not (yet) exported to epub:
             * OUTLINE: text-outline: 1px 1px #ccc;
             * SHADOWED: text-shadow: 2px 2px #ff0000;
             * ALLCAPS: text-transform:uppercase;
             * SMALLCAPS: font-variant:small-caps;
             * SHYPHEN: hyphenation is possible... how to use it?
             */
    }
}

// not tested yet
QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemFormatting formatting)
{
     dbg.nospace() << "(" << "Debug not implemented" << ")";
    return dbg.space();
}
