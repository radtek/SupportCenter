## Process this file with automake to produce Makefile.in
# Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006 Free Software Foundation
#
# Author: Nikos Mavroyanopoulos
#
# This file is part of GNUTLS.
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

SUBDIRS = gl includes lib libextra src doc tests po

EXTRA_DIST = mkinstalldirs

ACLOCAL_AMFLAGS = -I m4 -I gl/m4

# Maintainer targets

update-po: refresh-po
	cvs add `ls po/*.po | grep -v quot.po` || true && \
	cvs commit -m "Sync with TP." po/LINGUAS `ls po/*.po | grep -v quot.po`

ChangeLog:
	cvs2cl --utc --fsf --FSF --usermap .cvsusers -I ChangeLog -I .cvs --window 120 -l "-d"">2005-11-07;today<"""
	cat .cvscopying >> ChangeLog

tag = $(PACKAGE)_`echo $(VERSION) | sed 's/\./_/g'`
htmldir = ../www-$(PACKAGE)

release:
	if cvs upd -p -r $(tag) NEWS > /dev/null; then false else true; fi
	cvs rtag $(tag) gnutls
	rm -f ChangeLog
	make ChangeLog
	cvs commit -m "" ChangeLog
	cvs rtag -F $(tag) gnutls
	make dist
	gpg -b $(distdir).tar.bz2
	gpg --verify $(distdir).tar.bz2.sig
	scp $(distdir).tar.bz2{,.sig} igloo.linux.gr:~ftp/pub/gnutls/devel/
	ssh igloo.linux.gr 'cd ~ftp/pub/gnutls/devel/ && sha1sum *.tar.bz2 > CHECKSUMS'
	cd doc && ./gendocs.sh -o ../$(htmldir)/manual/ $(PACKAGE) "GNU TLS"
	cp -v $(distdir).tar.bz2{,.sig} $(htmldir)/releases/
	cp -v doc/reference/html/*.{html,png,css,devhelp} $(htmldir)/reference/
	cd $(htmldir) && cvs add -kb releases/$(distdir).tar.bz2{,.sig} && \
		cvs commit -m "Update." manual/ reference/ releases/
