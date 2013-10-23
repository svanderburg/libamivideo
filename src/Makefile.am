lib_LTLIBRARIES = libamivideo.la
pkginclude_HEADERS = amivideotypes.h viewportmode.h palette.h screen.h

libamivideo_la_SOURCES = viewportmode.c palette.c screen.c

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libamivideo.pc

apidox:
	doxygen Doxyfile

EXTRA_DIST = libamivideo.pc.in
