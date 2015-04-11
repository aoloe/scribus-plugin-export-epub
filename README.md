# Epub export plugin for Scribus


This plugin is not a tool to design epub files, but it takes your SLA file (the scribus file) and exports all the content -- text with paragraph style, character styles, and some of the local formatting; images -- and packs it in a well formatted epub file.

You will then be able to open the epub file with sigil (or any other epub editor) and tweak it to your needs.


## Install

Clone the `scribus-plugin-export-plugin` repository and put – or symlink – its `src` directory into `scribus/plugins/export/` as `epub`. Then add 

    ADD_SUBDIRECTORY(epub)

to `CMakeList.txt` file in `scribus/plugins/export/`.

## Samples

You can submit samples .sla by forking

https://github.com/aoloe/scribus-plugin-export-epub

and adding a new directory in `test/samples/` with each sample (collected for output!).

Please don't forget to add a `README.md` file explaining your sample is relevant and -- eventually -- an `.epub` file showing how the result should be.

All the content in the samples must be freely shareable. You might have to replace the images through cc-by-sa or public domain ones (you can search for such images on wikimedia / flickr or take/generate yourself dummy pictures). The same applies for the text. The files will be published in this repository.

And, finally, just make a pull request to let me know about it!

PLEASE:

- don't submit files that you don't want to see published on the internet and
- try to keep the files as simple as possible and
- submit one sample file per case.

## Features

### Text exporting

- Exports all the text that is not on master pages.
- All defined styles are converted to css in the style file.
- For each section start a new chapter
- The text frames are rendered as placed on the page, from the top left to the bottom right corner.
- Chained text are considered as one single text frame (the position of the first frame will be used).
- Paragraph styles are rendered as `<p class="">` and character styles as `<span class="">`.
- If the name of the font hints to a bold or italic formatting, the corresponding tag will be used.
- All types of underlines are rendered as standard underlines.
- Subscripts and superscripts are rendered through the corresponding tags.
- Strikethrough is rendered through the corresponding tag.

### Image exporting

- Exports all the images that are not on master pages.
- For now only PNG and JPPEG images are supporetd (in the future all other formats will be converted to PNG)
- If the image does not fill exactly the frame, the images is cropped to its visible part.
- If a maximal size is defined, the image is scaled down proportionally to its larger side (or always the width?).

### Vector graphics and shapes

Currently all shapes, lines and courbes are ignored.

## Code flow

- `epubexportplugin.cpp`:
  - implement the plug interface
  - launch and evaluate the export dialog
  - pass the the current document and the options from the dialog to `EpubExport`
- `EpubExport` (`epubexport.cpp`):
  - create `EpubExportEpubfile`
  - create a `ScribusAPIDocument` with the current document, read the list of items and sections
  - create `EpubExportStructure`
  - get the metadata from `ScribusAPIDocument` and pass it to `EpubExportStructure`
  - create `EpubExportContent`, pass `ScribusAPIDocument`and `EpubExportStructure` and get it to fill `
  - get the cover from the structure (if one is defined) or from the first page of `ScribusAPIDocument` otherwise
  - get the specific data from EpubExportStructure for the ncx and opf files and store them in the EpubExportEpubfile.
-  `EpubExportEpubfile` (epubexportEpubfile.cpp`)
-  `EpubExportContent` (epubexportContent.cpp`)
   - create one `EpubExportXhtml` file foreach section
   - for each item in each page
     - add the content for the `ScribusAPIDocumentItem` to the `EpubExportXhtml` file
     - add the files needed for `ScribusAPIDocumentItem`to the `EpubExportEpubfile` and the `EpubExportStructure`'s manifest and, eventually, table of contents (Toc)

## TODO

- create the scripter binding so that i can run the tests in an automatic way
- use the code to create an image from the first page and use it as a cover
- reactivate the part on marks in getTextRuns() (warning: StoryText::item(i) is now private and i have to find a new way to detect marks
- the new zip library does not correctly write the mimetype as the first file. fix the order
  or add an openArchive method (taken from unzip?) to be able to append to an existing file (and first add only the manifest, close the file and reopen it)
- have a look at David Tardon's (and Document Liberation's) epup generator. It's probably not very useful to us since it adds a dependency on boost, but it might have some good parts that we can copy.


## Inherited TODO

This todo list is taken from the older Epub plugin project and is not fully checked yet.


### ASAP

- make sure that getStylenameSanitized returns unique names
- bug to add:
- define what the sample tests do:
  - `rennes_600.jpg` must be included as is
  - `penguin.jpg` should be cropped, included once, used twice

- correctly handle images that are used multiple times in a file
  - still missing the second penguin
  - check for images that are inserted 1:1 (which don't need resizing nor cropping)
- correctly handle iamges with the same name and size but coming from different directories
- get the GUI to do something for each existing field (text color and font information)
- check that the items are not embedded or a pattern
- remove all references to OwnPage
- add support for footnotes (http://blog.epubbooks.com/183/creating-an-epub-document)
  - link and backlink
  - always at the end of the chapter
- correctly impelement captions (using the welding references?)
- "Default Paragraph Style" should be translates as p and not as `p.Default_Paragraph_Style` (the same for the default char style)


### BEFORE 1.0

- clean up the progress bar and implement the cancel button.
- if the images are not jpg or png (or .gif?) convert them to jpg or png.
- implement TOC with the fix `h\d.*` style names if cezary implemention of a real TOC does not make it in 1.5
- correctly set dtb:depth with the TOC depth (TODO)
- add the font files
- font sizes: 12 pt = 16px = 1 em = 100%
- convert all colors to rgb
- optionally (?) use h1 to break into files
- detect lists (option = glyph to look for <- no use cezary's lists!); if we're on a list retain the indent 
- for images where the resolution is defined, keep the size (in px???)
- select directory where to put the ebook
- in document settings optionally define the directory where low resolution images are placed (this can be useful for pdf created for the web)
- keeping the colors of the text and the background of frames should be an option
- what happens if a char and a para style have the same name? always create a span to apply the character style?
- correctly set the language in the metadata (TODO)
- store the generated UUID in the document configuration (TODO)
- all file names must have ASCII chars only (manage the clashes when renaming)
  - Your filenames have spaces or encoded characters. If your EPUB has any spaces in filenames, be sure the spaces are properly encoded in the EPUB manifest by using "%20" in their place. Filenames may not contain periods (“.”) other than to separate the filename from its extension.
- check that the cover is correctly generated as as a jpg with 1400px height (generate a png and convert it to jpg)
- cover:
      <div>
        <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="100%" preserveAspectRatio="xMidYMid meet" version="1.1" viewBox="0 0 600 800" width="100%">
          <image height="800" width="600" xlink:href="../Images/cover.jpeg"></image>
        </svg>
      </div>
- patch scribus to enable an identifier type in the document information (#11055)
- check that for each section a new file is created (should already be so)
- check how the exporter behaves with embedded frames
- fix the <span>s around the <br>s (cf. fm/scribus sample by cédric)



### LATER

- optionally allow a special rule for (big) portrait pictures (ex.: max width = 80%)
- eventually and optionally set the chapter title as the name of the section in the epub file (TODO)
- eventually store the epu date in the document configuration (TODO)
- define the condition under which a new span builds upon the current one or replaces it. (as an example: if a font is defined and it does not change, build upon it)
- show the document infos in the export dialog
- add a checkbox to open the created epub with an application (sigil) and let it set in the preferences
- groups only containing shapes (no text nor images) should be exported as svg
- patch scribus to enable an authorFileAs the document information (lastname, first,)
  - should the document information also be in the preferences (mostly not needed, but some fields can be handy for epub in some cases
- we may need to obfuscate fonts on demand (or leave it to sigil?) (Sigil/Importers/ImportEPUB.cpp FontObfuscation; Sigil/Misc/FontObfuscation)
- QDomDocument has some limitations (like putting newlines around <span>s): in the future we should probably rather use SAX
- allow a separate folder for low-res images (this should probably be a general feature in scribus, since it also useful for web oriented PDFs: it should be carefully designed)
- load an external css
- show a report
  - which styles are not matching between .sla and .css?

