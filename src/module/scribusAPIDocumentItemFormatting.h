/**
 * EpubExportScribusDocument is a proxy to the scribus' PageItem and is thought as
 * an interface to get information about the scribus items
 */
#ifndef SCRIBUSAPIDOCUMENTITEMFORMATTING_H
#define SCRIBUSAPIDOCUMENTITEMFORMATTING_H

#include <QDebug>
#include <QObject>

#include <QString>
#include <QVector>

class PageItem;

class ScribusAPIDocumentItem;

enum ScribusAPIDocumentItemFormattingFlag {
    formattingFlag_None          = 0,
    formattingFlag_Superscript   = 1 << 0, // 1
    formattingFlag_Subscript     = 1 << 1, // 2
    formattingFlag_Outline       = 1 << 2, // 4,
    formattingFlag_Underline     = 1 << 3, // 8
    formattingFlag_Strikethrough = 1 << 4,
    formattingFlag_AllCaps       = 1 << 5,
    formattingFlag_SmallCaps     = 1 << 6,
    formattingFlag_HyphenationPossible = 1 << 7, //Hyphenation possible here (Soft Hyphen)
    formattingFlag_Shadowed      = 1 << 8,
    formattingFlag_UnderlineWords= 1 << 9,
    formattingFlag_Reserved01    = 1 << 10, //free, not used in the moment
    formattingFlag_DropCap       = 1 << 11,
    formattingFlag_Bold          = 1 << 12, // not in scribus
    formattingFlag_Italic        = 1 << 13,
    formattingFlag_All           = (1 << 14) - 1,
};


class ScribusAPIDocumentItemFormatting : public QObject
{
    Q_OBJECT

public:
	ScribusAPIDocumentItemFormatting();
	~ScribusAPIDocumentItemFormatting();

    void setItem(PageItem* item) { this->item = item; }
    void setPosition(int position) { this->position = position; }
    void readAtPosition(PageItem* item, int position);
    void readAtPosition(int position) { readAtPosition(this->item, position); }
    void readAtPosition() { readAtPosition(this->item, this->position); }

    QString getParagraphStyleName() { return paragraphStyleName; }
    QString getCharacterStyleName() { return characterStyleName; }

    // QString getXhtmlContent();
    bool isBold() { return formatting & formattingFlag_Bold; }
    bool isItalic() { return formatting & formattingFlag_Italic; }
    bool isSuperscript() { return formatting & formattingFlag_Superscript; }
    bool isSubscript() { return formatting & formattingFlag_Subscript; }
    bool isUnderline() { return formatting & formattingFlag_Underline; }
    bool isUnderlineWords() { return formatting & formattingFlag_UnderlineWords; }
    bool isStrikethrough() { return formatting & formattingFlag_Strikethrough; }
    // else if (feature != CharStyle::INHERIT) // what to do with inherit?

protected:
    PageItem* item;
    int position;
    QString paragraphStyleName;
    QString characterStyleName;
    int formatting;
};

QDebug operator<<(QDebug dbg, const ScribusAPIDocumentItemFormatting &formatting);

#endif //SCRIBUSAPIDOCUMENTITEMFORMATTING_H
