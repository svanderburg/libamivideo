lib_LTLIBRARIES = libamivideo.la
pkginclude_HEADERS = amivideotypes.h viewportmode.h bitplane.h palette.h

libamivideo_la_SOURCES = viewportmode.c bitplane.c palette.c

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libamivideo.pc

apidox:
	doxygen Doxyfile

EXTRA_DIST = libamivideo.pc.in
