# -*- mode: shell-script -*-

define([D2X_MAJOR],0)
define([D2X_MINOR],2)
define([D2X_MICRO],6)

AC_INIT([d2x],[D2X_MAJOR.D2X_MINOR.D2X_MICRO],[https://github.com/btb/d2x/issues])
AC_PREREQ([2.52])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([subdir-objects])

AC_CONFIG_HEADERS(conf.h)

AC_DEFINE_UNQUOTED(D2XMAJOR,D2X_MAJOR,[d2x major version])
AC_DEFINE_UNQUOTED(D2XMINOR,D2X_MINOR,[d2x minor version])
AC_DEFINE_UNQUOTED(D2XMICRO,D2X_MICRO,[d2x micro version])

AM_MAINTAINER_MODE

#kludge to keep autoconf from adding -g -O2
CFLAGS=" $CFLAGS"

AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_SEARCH_LIBS([strerror],[cposix])
AC_CHECK_TOOL(CC, gcc, :)
AC_CHECK_TOOL(RANLIB, ranlib, :)
AC_CHECK_TOOL(STRIP, strip, :)
AC_CHECK_TOOL(AR, ar, :)
AC_PROG_CC
AC_PATH_PROGS(NASM, nasm nasmw, no)
AC_C_BIGENDIAN
AC_CHECK_SIZEOF(void *)

case $host_cpu in
    sparc)
        AC_DEFINE(WORDS_NEED_ALIGNMENT,,[Define if your processor needs data to be word-aligned])
    ;;
esac

case $host_os in
    cygwin)
        AC_MSG_WARN([Cygwin found, setting -mno-cygwin])
        CFLAGS="-mno-cygwin $CFLAGS"
        MINGW32=yes
        ;;
    mingw*)
        MINGW32=yes
        ;;
    darwin*)
        MACOSX=yes
        CFLAGS="-no-cpp-precomp $CFLAGS"
        ;;
    msdos*)
        MSDOS=yes
        ;;
esac

AC_CHECK_DECLS(nanosleep,,,[#include <time.h>])
AC_CHECK_TYPES([struct timespec, struct timeval],,,[#include <sys/time.h>])

AM_CONDITIONAL(MINGW32, test x$MINGW32 = xyes)

AM_CONDITIONAL(MACOSX, test x$MACOSX = xyes)

AM_CONDITIONAL(MSDOS, test x$MSDOS = xyes)

dnl Set $prefix and $exec_prefix to $ac_default_prefix if they are not set
test "x$prefix" = "xNONE" && prefix=$ac_default_prefix
test "x$exec_prefix" = "xNONE" && exec_prefix="${prefix}"

dnl Set default locations to find data files.
default_sharepath="$datadir/games/$PACKAGE"

CFLAGS="-Wall -Werror -Wno-deprecated-declarations -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-value -Wno-address $CFLAGS"
AX_CHECK_COMPILE_FLAG([-Wno-ignored-attributes], [CFLAGS="$CFLAGS -Wno-ignored-attributes"])
AX_CHECK_COMPILE_FLAG([-Wno-unused-but-set-variable], [CFLAGS="$CFLAGS -Wno-unused-but-set-variable"])

dnl Enable editor build?
AC_ARG_ENABLE(editor,
    [  --enable-editor         Build editor? (not functional)],,)

dnl Enable macdata build?
AC_ARG_ENABLE(macdata,
    [  --enable-macdata        Build for mac datafiles],,)
if test x$enable_macdata = xyes; then
    AC_DEFINE(MACDATA,,[Define if you want to build for mac datafiles])
    D2X_FEATURES="macdata ${D2X_FEATURES}"
fi
AM_CONDITIONAL(MACDATA, test x$enable_macdata = xyes)

dnl Build with NDEBUG?
AC_ARG_ENABLE(debug,
    [  --disable-debug         Take out debug symbols, Asserts, Int3, etc ],,)
if test x$enable_debug = xno; then
    AC_DEFINE(NDEBUG,,[Define to disable asserts, int3, etc.])
    CFLAGS="-O2 $CFLAGS"
else
    CFLAGS="-g $CFLAGS"
    D2X_FEATURES="debug ${D2X_FEATURES}"
fi

dnl Build with RELEASE?
AC_ARG_ENABLE(release,
    [  --enable-release        Release build.  Removes debug/editor things ],,)
if test x$enable_release = xyes; then
    AC_DEFINE(RELEASE,,[Define for a "release" build])
    D2X_FEATURES="release ${D2X_FEATURES}"
fi

dnl Build without using sdl-joystick ?
AC_ARG_ENABLE(linuxjoy,
    [  --enable-linuxjoy       do not use the SDL Joystick. (eg for MS Sidewinder)],,)
if test x$enable_linuxjoy = xyes; then
    AC_DEFINE(USE_LINUX_JOY,,[define to not use the SDL_Joystick routines.])
    D2X_FEATURES="linuxjoy ${D2X_FEATURES}"
fi
AM_CONDITIONAL(USE_LINUX_JOY, test x$enable_linuxjoy = xyes)

dnl Build with FAST_FILE_IO?
AC_ARG_ENABLE(fastfileio,
    [  --disable-fastfileio    Disable fast file i/o. ],,)
if test x$enable_fastfileio != xno; then
    if test x$ac_cv_c_bigendian = xyes; then
        AC_MSG_WARN([big-endian cpu detected.  disabling fastfileio])
        enable_fastfileio="no";
    fi
    if test $ac_cv_sizeof_void_p != 4; then
        AC_MSG_WARN([pointers not 32 bits.  disabling fastfileio])
        enable_fastfileio="no";
    fi
fi
if test x$enable_fastfileio != xno; then
    AC_DEFINE(FAST_FILE_IO,,[Define for faster i/o on little-endian cpus])
    D2X_FEATURES="fastfileio ${D2X_FEATURES}"
fi

AC_ARG_WITH(sharepath,
    [[  --with-sharepath=DIR    Use DIR for shared game data (unix only) [DATADIR/games/d2x]]],
    sharepath=$withval, sharepath="auto")
if test "x$sharepath" = "xauto" -o "x$sharepath" = "xyes" -o "x$sharepath" = "x"; then
    sharepath="$default_sharepath"
fi
AC_SUBST(sharepath)
eval sharepath=$sharepath
eval sharepath=$sharepath
AC_DEFINE_UNQUOTED(SHAREPATH, "$sharepath",[Define this to be the shared game directory root])

TARGETS=

# Test for MSDOS
if test x$MSDOS = xyes; then
    TARGETS="d2x$ac_cv_exeext $TARGETS"
	if test x$enable_editor = xyes; then
		TARGETS="miner$ac_cv_exeext $TARGETS"
	fi
else

    CFLAGS="-pipe $CFLAGS"

    dnl Check for SDL
    AM_PATH_SDL(1.1.0,
        :,
        AC_MSG_ERROR(SDL not found. Make sure sdl-config is in your PATH, or specify with --with-sdl-prefix)
    )

    TARGETS="d2x-sdl$ac_cv_exeext $TARGETS"
	if test x$enable_editor = xyes; then
		TARGETS="miner-sdl$ac_cv_exeext $TARGETS"
	fi

    AC_CHECK_HEADERS(netipx/ipx.h)

  # Check for SDL_image
    AC_CHECK_LIB(SDL_image, IMG_ReadXPMFromArray,
        SDLIMAGE_LIBS="-lSDL_image"
        AC_DEFINE(SDL_IMAGE,,[Define if you have the SDL_image library]))
    AC_SUBST(SDLIMAGE_LIBS)

  # Check for SDL_mixer
    PKG_CHECK_MODULES(SDLMIXER, SDL_mixer)
    AC_SUBST(SDLMIXER_CFLAGS)
    AC_SUBST(SDLMIXER_LIBS)

  # Check for libpng
    have_libpng=no
    #PKG_CHECK_MODULES(LIBPNG, libpng,
    #    AC_DEFINE(HAVE_LIBPNG,,[Define if you have libpng])
    #    have_libpng=yes
    #    LIBS="$LIBPNG_LIBS $LIBS",
    #    AC_MSG_WARN([libpng not found. disabling png replacement texture support])
    #)
    AM_CONDITIONAL(USE_LIBPNG, test x$have_libpng = xyes)

  # Check for PhysicsFS
    AC_CHECK_HEADERS(physfs.h, , [AC_MSG_ERROR([physfs.h required])])
    AC_CHECK_LIB(physfs, PHYSFS_init, PHYSFS_LIBS="-lphysfs",
                 [AC_MSG_ERROR([physfs library required])])
    AC_SUBST(PHYSFS_LIBS)

  # Check for OpenGL
    AC_ARG_WITH(opengl,
        [  --with-opengl           Build OpenGL support ],,)
    if test x$with_opengl = xyes; then
        TARGETS="d2x-gl$ac_cv_exeext $TARGETS"
		if test x$enable_editor = xyes; then
			TARGETS="miner-gl$ac_cv_exeext $TARGETS"
		fi
    fi

  # Check for SVGALib
    AC_ARG_WITH(svga,
        [  --with-svga             Build SVGALib support ],,)
    if test x$with_svga = xyes; then
        TARGETS="d2x-svga$ac_cv_exeext $TARGETS"
		if test x$enable_editor = xyes; then
			TARGETS="miner-svga$ac_cv_exeext $TARGETS"
		fi
    fi
    AC_SUBST(SVGA_LIBS)
fi

dnl Check for network
AC_ARG_ENABLE(network,
    [  --disable-network       Do not build network/serial support ],,)
if test x$enable_network != xno; then
    case $host_os in
        cygwin* | mingw* | msdos* | linux*)
            enable_native_ipx="yes";
           ;;
       *)
            AC_MSG_WARN([d2x does not support native ipx on this host.  disabling native ipx])
            enable_native_ipx="no";
            ;;
    esac
    case $host_os in
        cygwin* | mingw* | msdos*)
            enable_kalinix="no";
            ;;
    esac
    case $host_os in
        *solaris*)
            AC_CHECK_LIB(socket, socket, LIBS="${LIBS} -lsocket",
                [AC_MSG_ERROR(socket lib required for net support on solaris not found)])
            AC_CHECK_LIB(nsl, inet_addr, LIBS="${LIBS} -lnsl",
                [AC_MSG_ERROR(nsl lib required for net support on solaris not found)])
            ;;
    esac
fi
AC_ARG_ENABLE(kalinix,
    [  --disable-kalinix       Disable the KaliNix driver (unix-only)],,)
if test x$enable_network != xno; then
    if test x$enable_native_ipx != xno; then
        D2X_FEATURES="ipx ${D2X_FEATURES}"
        AC_DEFINE(NATIVE_IPX,,[Define to use the IPX support of the OS])
    fi
    if test x$enable_kalinix != xno; then
        D2X_FEATURES="kalinix ${D2X_FEATURES}"
        AC_DEFINE(KALINIX,,[Define to enable use of the KaliNix driver])
    fi
    AC_DEFINE(NETWORK,,[Define if you want a network build])
    D2X_FEATURES="network ${D2X_FEATURES}"
fi
AM_CONDITIONAL(USE_NETWORK, test x$enable_network != xno)
AM_CONDITIONAL(USE_NATIVE_IPX, test x$enable_native_ipx != xno)
AM_CONDITIONAL(USE_KALINIX, test x$enable_kalinix != xno)

dnl Check for assembler
AC_ARG_ENABLE(assembler,
    [  --disable-assembler     Do not use assembler optimization ],,)
if test x$enable_assembler != xno; then
    if test x$NASM = xno; then
        AC_MSG_WARN([nasm not found. disabling assembler])
        enable_assembler="no"
    else
        NASMFLAGS="-i\$(srcdir)/ $NASMFLAGS"
        case $host_os in
            cygwin* | mingw*)
                NASMFLAGS="-f win32 $NASMFLAGS"
                ;;
            msdos*)
                NASMFLAGS="-f coff $NASMFLAGS"
                ;;
            linux* | freebsd*)
                NASMFLAGS="-f elf -d__ELF__ $NASMFLAGS"
                ;;
            *)
                AC_MSG_WARN([nasm not supported for this OS. disabling assembler])
                enable_assembler="no"
                ;;
        esac
        case $host_cpu in
            x86_64)
                AC_MSG_WARN([nasm not supported for this cpu. disabling assembler])
                enable_assembler="no"
                ;;
        esac
    fi
fi
if test x$enable_assembler = xno; then
    AC_DEFINE(NO_ASM,,[Define if you want an assembler free build])
    D2X_FEATURES="no_asm ${D2X_FEATURES}"
fi
AM_CONDITIONAL(USE_ASM, test x$enable_assembler != xno)

AH_BOTTOM([
        /* General defines */
#ifndef PACKAGE_STRING
#define PACKAGE_STRING PACKAGE " " VERSION
#endif
#define VERSION_NAME PACKAGE_STRING
#define NMONO 1
#define PIGGY_USE_PAGING 1
#define NEWDEMO 1

#if defined(__APPLE__) && defined(__MACH__)
#define __unix__ /* since we're doing a unix-style compilation... */
#endif
        ])

AM_PROG_AS

AC_SUBST(TARGETS)

AC_SUBST(NASMFLAGS)

AC_SUBST(CFLAGS)

AC_CONFIG_FILES([Makefile
    debian/changelog
    rpm/d2x.spec
])
AC_OUTPUT

AC_MSG_RESULT([
        d2x has been configured successfully.

        Targets: $TARGETS
        Features: $D2X_FEATURES

        Shared game data directory (unix only): $sharepath
        ])
