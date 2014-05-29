Introduction
--------

A tool to map binary trace to source code with help of objdump and Emacs.

Compile
--------
        cd src
        make

Emacs Configuration 
--------

Put 'tracemapping.el' to your Emacs plugin directory, such as ~/.emacs.d. Then put the
following code in your .emacs, init.el, site-load.el, or other
relevant file

    (add-to-list 'load-path "path-to-tracemapping.el")
    (require 'tracemapping)


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
Open example.tracemap with Emacs, "Ctrl+z" to jump to source code, you
may need to select your source code directory first.

Open example.fun with Emacs, click to jump to source code or tracemap file.
