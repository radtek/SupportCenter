## Process this file with automake to produce Makefile.in
# Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006 Free Software Foundation
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

EXTRA_DIST = README README.srptool gnutls-http-serv params.pem

SUBDIRS = srp x509 openpgp cfg

INCLUDES = -I../includes -I$(top_srcdir)/includes \
	-I$(top_srcdir)/gl -I$(top_builddir)/gl -I$(srcdir)/cfg

bin_PROGRAMS = gnutls-serv gnutls-cli psktool gnutls-cli-debug certtool
if ENABLE_SRP
bin_PROGRAMS += srptool
endif

gnutls_serv_SOURCES = serv.gaa serv-gaa.h serv-gaa.c list.h serv.c	\
	common.h common.c select.c
gnutls_serv_LDADD = ../lib/libgnutls.la ../libextra/libgnutls-extra.la	\
	$(LIBGCRYPT_LIBS) $(LIBTASN1_LIBS) $(LIBOPENCDK_LIBS)

if ENABLE_SRP
srptool_SOURCES = crypt.gaa crypt-gaa.h crypt-gaa.c crypt.c
srptool_LDADD = ../lib/libgnutls.la ../libextra/libgnutls-extra.la	\
	$(LIBGCRYPT_LIBS) $(LIBTASN1_LIBS) $(LIBOPENCDK_LIBS)		\
	../gl/libgnu.la
endif

psktool_SOURCES = psk.gaa psk-gaa.h psk-gaa.c psk.c
psktool_LDADD = ../lib/libgnutls.la ../libextra/libgnutls-extra.la	\
	$(LIBGCRYPT_LIBS) $(LIBTASN1_LIBS) $(LIBOPENCDK_LIBS)		\
	../gl/libgnu.la

gnutls_cli_SOURCES = cli.gaa cli-gaa.h cli-gaa.c cli.c common.h	\
	common.c select.c
gnutls_cli_LDADD = ../lib/libgnutls.la ../libextra/libgnutls-extra.la	\
	$(LIBGCRYPT_LIBS) $(LIBTASN1_LIBS) $(LIBOPENCDK_LIBS)

gnutls_cli_debug_SOURCES = tls_test.gaa tls_test-gaa.h tls_test-gaa.c	\
	tls_test.c tests.h tests.c common.h common.c
gnutls_cli_debug_LDADD = ../lib/libgnutls.la			\
	../libextra/libgnutls-extra.la $(LIBGCRYPT_LIBS)	\
	$(LIBTASN1_LIBS) $(LIBOPENCDK_LIBS)

certtool_SOURCES = certtool.gaa certtool-gaa.h certtool-cfg.h	\
	certtool-gaa.c certtool.c prime.c certtool-cfg.c
certtool_LDADD = ../lib/libgnutls.la $(LIBGCRYPT_LIBS)	\
	$(LIBTASN1_LIBS) ../gl/libgnu.la @LTLIBREADLINE@
if HAVE_LIBCFG
certtool_LDADD += -lcfg+
else
certtool_SOURCES += cfg/cfg+.c cfg/cfgfile.c cfg/cmdline.c cfg/parse.c	\
	cfg/props.c cfg/shared.c cfg/platon/str/dynfgets.c		\
	cfg/platon/str/strctype.c cfg/platon/str/strdyn.c		\
	cfg/platon/str/strplus.c
endif

noinst_PROGRAMS = errcodes
errcodes_SOURCES = errcodes.c
errcodes_LDADD = ../lib/libgnutls.la $(LIBGCRYPT_LIBS) $(LIBTASN1_LIBS)



psk-gaa.c: psk.gaa
	-gaa psk.gaa -o psk-gaa.c -i psk-gaa.h
crypt-gaa.c: crypt.gaa
	-gaa crypt.gaa -o crypt-gaa.c -i crypt-gaa.h
certtool-gaa.c: certtool.gaa
	-gaa certtool.gaa -o certtool-gaa.c -i certtool-gaa.h
cli-gaa.c: cli.gaa
	-gaa cli.gaa -o cli-gaa.c -i cli-gaa.h
tls_test-gaa.c: tls_test.gaa
	-gaa tls_test.gaa -o tls_test-gaa.c -i tls_test-gaa.h
serv-gaa.c: serv.gaa
	-gaa serv.gaa -o serv-gaa.c -i serv-gaa.h

