Compile
--------
        cd src
        make

Emacs Configuration 
--------

Add elisp snippet in file tracemapping.el to your .emacs or
.emacs.d/init.el, and make the directory point to your source code
directory.

Source File Generation
--------
        objdump -dl vmlinux > example.dl
        ./sourceline example.dl > example.sourceline

Generate Trace Mapping File From Binary Tracing
--------
        ./tracemap example.sourceline example.trace > example.tracemap

Generate Function index from tracemap file
--------
        ./get-fun.py example.tracemap > example.fun

Usage
--------
Open example.tracemap with Emacs, "Ctrl+z" to jump to source code.

Open example.fun with Emacs, click to jump to source code or tracemap file.
