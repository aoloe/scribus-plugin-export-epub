# scribus-plugin-export-epub


epub export plugin for scribus

## Install

Clone the `scribus-plugin-export-plugin` repository and put – or symlink – its `src` directory into `scribus/plugins/export/` as `epub`. Then add 

    ADD_SUBDIRECTORY(epub)

to `CMakeList.txt` file in `scribus/plugins/export/`.
