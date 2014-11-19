dnl Check for libconfig
AC_MSG_CHECKING([for libconfig installation])

AC_ARG_WITH([libconfig],
[  --with-libconfig=DIR   path to directory containing libconfig (default=
    /usr/local or /usr if not found in /usr/local)],
[
if test "$withval"; then
    LIBCONFIG_HOME="$withval"
fi
], [
LIBCONFIG_HOME=/usr/local
if test ! -f "$LIBCONFIG_HOME/include/libconfig.h"
then
    LIBCONFIG_HOME=/usr
fi
AC_MSG_RESULT([$LIBCONFIG_HOME])
])

if test ! -f "$LIBCONFIG_HOME/include/libconfig.h"
then
    AC_MSG_ERROR([libconfig not found.])
fi

save_LDFLAGS="$LDFLAGS"
LDFLAGS="-L$LIBCONFIG_HOME/lib -lconfig"
AC_CHECK_LIB([config], [config_init], [FRESHCLAM_LIBS="$FRESHCLAM_LIBS -L$LIBCONFIG_HOME/lib -lconfig"],
        [AC_MSG_ERROR([Your libconfig installation is misconfigured])])

CLAMSCAN_LIBS="$CLAMSCAN_LIBS -L$LIBCONFIG_HOME/lib -lconfig"
CLAMDSCAN_LIBS="$CLAMDSCAN_LIBS -L$LIBCONFIG_HOME/lib -lconfig"
CLAMD_LIBS="$CLAMD_LIBS -L$LIBCONFIG_HOME/lib -lconfig"
CLAMMEM_LIBS="$CLAMMEM_LIBS -L$LIBCONFIG_HOME/lib -lconfig"
CLAMAV_MILTER_LIBS="$CLAMAV_MILTER_LIBS -L$LIBCONFIG_HOME/lib -lconfig"
LIBCLAMAV_LIBS="$LIBCLAMAV_LIBS -L$LIBCONFIG_HOME/lib -lconfig"
CLAMDTOP_LIBS="$CLAMDTOP_LIBS -L$LIBCONFIG_HOME/lib -lconfig"
CLAMBC_LIBS="$CLAMBC_LIBS -L$LIBCONFIG_HOME/lib -lconfig"

CLAMSCAN_CPPFLAGS="$FRESHCLAM_CPPFLAGS -I$LIBCONFIG_HOME/include"
CLAMDSCAN_CPPFLAGS="$CLAMDSCAN_CPPFLAGS -I$LIBCONFIG_HOME/include"
CLAMD_CPPFLAGS="$CLAMD_CPPFLAGS -I$LIBCONFIG_HOME/include"
CLAMMEM_CPPFLAGS="$CLAMMEM_CPPFLAGS -I$LIBCONFIG_HOME/include"
CLAMAV_MILTER_CPPFLAGS="$CLAMAV_MILTER_CPPFLAGS -I$LIBCONFIG_HOME/include"
LIBCLAMAV_CPPFLAGS="$LIBCLAMAV_CPPFLAGS -I$LIBCONFIG_HOME/include"
LIBCLAMSHARED_CPPFLAGS="$LIBCLAMSHARED_CPPFLAGS -I$LIBCONFIG_HOME/include"
SIGTOOL_CPPFLAGS="$SIGTOOL_CPPFLAGS -I$LIBCONFIG_HOME/include"
CLAMCONF_CPPFLAGS="$CLAMCONF_CPPFLAGS -I$LIBCONFIG_HOME/include"
CLAMDTOP_CPPFLAGS="$CLAMDTOP_CPPFLAGS -I$LIBCONFIG_HOME/include"
CLAMBC_CPPFLAGS="$CLAMBC_CPPFLAGS -I$LIBCONFIG_HOME/include"

LDFLAGS="$save_ldflags"
