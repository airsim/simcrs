dnl @synopsis AX_MYSQL
dnl 
dnl This macro tries to find MySQL C API header and library locations.
dnl
dnl We define the following configure script flags:
dnl
dnl		--with-mysql: Give prefix for both library and headers, and try
dnl			to guess subdirectory names for each.  (e.g. Tack /lib and
dnl			/include onto given dir name, and other common schemes.)
dnl		--with-mysql-lib: Similar to --with-mysql, but for library only.
dnl		--with-mysql-include: Similar to --with-mysql, but for headers
dnl			only.
dnl
dnl @version 1.2, 2007/02/20
dnl @author Warren Young <mysqlpp@etr-usa.com>

AC_DEFUN([AX_MYSQL],
[
	#
	# Set up configure script macros
	#
	AC_ARG_WITH(mysql,
		[  --with-mysql=<path>     root directory path of MySQL installation],
		[MYSQL_lib_check="$with_mysql/lib64/mysql $with_mysql/lib/mysql $with_mysql/lib64 $with_mysql/lib"
		MYSQL_inc_check="$with_mysql/include $with_mysql/include/mysql"
		MYSQL_bin_check="$with_mysql/bin"],
		[MYSQL_lib_check="/usr/lib64 /usr/lib /usr/lib64/mysql /usr/lib/mysql /usr/local/lib64 /usr/local/lib /usr/local/lib/mysql /usr/local/mysql/lib /usr/local/mysql/lib/mysql /opt/mysql/lib /opt/mysql/lib/mysql"
		MYSQL_inc_check="/usr/include /usr/local/include /usr/local/mysql/include /opt/mysql/include"
		MYSQL_bin_check="/usr/bin /usr/local/bin /usr/local/mysql/bin"])

	AC_ARG_WITH(mysql-lib,
		[  --with-mysql-lib=<path> directory path of MySQL library installation],
		[MYSQL_lib_check="$with_mysql_lib $with_mysql_lib/lib64 $with_mysql_lib/lib $with_mysql_lib/lib64/mysql $with_mysql_lib/lib/mysql"])

	AC_ARG_WITH(mysql-include,
		[  --with-mysql-include=<path> directory path of MySQL header installation],
		[MYSQL_inc_check="$with_mysql_include $with_mysql_include/include $with_mysql_include/include/mysql"])


    #
	# Look for MySQL Configuration Script
	#
	AC_MSG_CHECKING([for MySQL configuration script])
	MYSQL_CONFIG=
	MYSQL_bindir=
	for m in $MYSQL_bin_check
		do
			if test -d "$m" && test -f "$m/mysql_config"
			then
				MYSQL_CONFIG=$m/mysql_config
				MYSQL_bindir=$m
			break
		fi
	done

	if test -z "$MYSQL_bindir"
	then
		AC_MSG_ERROR([Didn't find $MYSQL_CONFIG binary in '$MYSQL_bin_check'])
	fi

	case "$MYSQL_bindir" in
		/* ) ;;
		* )  AC_MSG_ERROR([The MySQL binary directory ($MYSQL_bindir) must be an absolute path.]) ;;
	esac

	AC_MSG_RESULT([$MYSQL_bindir])

#	AC_PATH_PROG(MYSQL_CONFIG, mysql_config, $MYSQL_bindir)

	#
	# Decide which C API library to use, based on thread support
	#
	if test "x$acx_pthread_ok" = xyes
	then
		MYSQL_C_LIB=mysqlclient_r
		MYSQL_LIB_OPTION=libs_r
	else
		MYSQL_C_LIB=mysqlclient
		MYSQL_LIB_OPTION=libs
	fi

	if test "x${MYSQL_CONFIG+set}" != xset
	then
		MYSQL_VERSION=`${MYSQL_CONFIG} --version`
		MYSQL_CFLAGS=`${MYSQL_CONFIG} --cflags`
		MYSQL_LIBS=`${MYSQL_CONFIG} --${MYSQL_LIB_OPTION}`
	else
		#
		# Look for MySQL C API library
		#
		AC_MSG_CHECKING([for MySQL library directory])
		MYSQL_libdir=
		for m in $MYSQL_lib_check
		do
			if test -d "$m" && \
				(test -f "$m/lib$MYSQL_C_LIB.so" \
				|| test -f "$m/lib$MYSQL_C_LIB.a")
			then
				MYSQL_libdir=$m
				break
			fi
		done

		if test -z "$MYSQL_libdir"
		then
			AC_MSG_ERROR([Didn't find $MYSQL_C_LIB library in '$MYSQL_lib_check'])
		fi

		case "$MYSQL_libdir" in
			/* ) ;;
			* )  AC_MSG_ERROR([The MySQL library directory ($MYSQL_libdir) must be an absolute path.]) ;;
		esac

		AC_MSG_RESULT([$MYSQL_libdir])

		case "$MYSQL_libdir" in
		  /usr/lib) ;;
		  *) LDFLAGS="$LDFLAGS -L${MYSQL_libdir}" ;;
		esac

		#
		# Look for MySQL C API headers
		#
		AC_MSG_CHECKING([for MySQL include directory])
		MYSQL_incdir=
		for m in $MYSQL_inc_check
		do
			if test -d "$m" && test -f "$m/mysql/mysql.h"
			then
				MYSQL_incdir=$m
				break
			fi
		done

		if test -z "$MYSQL_incdir"
		then
			AC_MSG_ERROR([Didn't find the MySQL include dir in '$MYSQL_inc_check'])
		fi

		case "$MYSQL_incdir" in
			 /* ) ;;
			 * )  AC_MSG_ERROR([The MySQL include directory ($MYSQL_incdir) must be an absolute path.]) ;;
		esac

		AC_MSG_RESULT([$MYSQL_incdir])

		MYSQL_CFLAGS="-I${MYSQL_incdir}"
		MYSQL_LIBS="-L${MYSQL_libdir} -l${MYSQL_C_LIB}"
	fi

	AC_SUBST(MYSQL_VERSION)
	AC_SUBST(MYSQL_CFLAGS)
	AC_SUBST(MYSQL_LIBS)

	save_LIBS="$LIBS"
	LIBS="$LIBS $MYSQL_LIBS"
	AC_CHECK_LIB($MYSQL_C_LIB, mysql_store_result,
       			[], 
               	[AC_MSG_ERROR([Could not find working MySQL client library!])]
               	)
	AC_SUBST(MYSQL_C_LIB)
	LIBS="$save_LIBS"
]) dnl AX_MYSQL
