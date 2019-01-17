
Summary:
--------
SimCRS aims at providing a clean API and a simple implementation, as
a C++ library, of a Travel-oriented Distribution System. It
corresponds to the simulated version of the real-world Computerized
Reservation Systems (CRS).

SimCRS makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the
Boost (C++ Standard Extensions: http://www.boost.org) library is used.

SimCRS is the one of the components of the Travel Market Simulator
(http://www.travel-market-simulator). However, it may be used in a
stand-alone mode.


Getting and installing from the Fedora/CentOS/RedHat distribution:
------------------------------------------------------------------
Just use Yum:
yum -y install simcrs-devel simcrs-doc

You can also get the RPM packages (which may work on Linux
distributions like Novel Suse and Mandriva) from the Fedora repository
(e.g., for Fedora 22, 
http://fr2.rpmfind.net/linux/fedora/releases/22/Everything/)


Building the library and test binary from Git repository:
----------------------------------------------------------------
The Git repository may be cloned as following:
$ git clone git@github.com:airsim/simcrs.git simcrsgit # through SSH
$ git clone https://github.com/airsim/simcrs.git # if the firewall filters SSH
cd simcrsgit
git checkout trunk

Then, you need the following packages (Fedora/RedHat/CentOS names here, 
but names may vary according to distributions):
* cmake
* gcc-c++
* boost-devel / libboost-dev
* python-devel / python-dev
* gettext-devel / gettext-dev
* sqlite3-devel / libsqlite3-dev
* readline-devel / readline-dev
* ncurses-devel
* soci-mysql-devel, soci-sqlite3-devel
* stdair-devel / libstdair-dev
* sevmgr-devel / libsevmgr-dev
* airrac-devel / libairrac-dev
* rmol-devel / librmol-dev
* airinv-devel / libairinv-dev
* simfqt-devel / libsimfqt-dev
* airtsp-devel / libairtsp-dev
* doxygen, ghostscript, graphviz
* tetex-latex (optional)
* rpm-build (optional)

Building the library and test binary from the tarball:
------------------------------------------------------
The latest stable source tarball (simcrs*.tar.gz or .bz2) can be found here:
https://sourceforge.net/projects/simcrs/files/

To customise the following to your environment, you can alter the path
to the installation directory:
export INSTALL_BASEDIR=/home/user/dev/deliveries
export CRS_VER=1.01.2
if [ -d /usr/lib64 ]; then LIBSUFFIX=64; fi
export LIBSUFFIX_4_CMAKE="-DLIB_SUFFIX=$LIBSUFFIX"

Then, as usual:
* To configure the project, type something like:
  mkdir build && cd build
  cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/simcrs-$CRS_VER \
   -DWITH_STDAIR_PREFIX=${INSTALL_BASEDIR}/stdair-stable \
   -DWITH_AIRRAC_PREFIX=${INSTALL_BASEDIR}/airtsp-stable \
   -DWITH_SEVMGR_PREFIX=${INSTALL_BASEDIR}/sevmgr-stable \
   -DWITH_AIRRAC_PREFIX=${INSTALL_BASEDIR}/airrac-stable \
   -DWITH_RMOL_PREFIX=${INSTALL_BASEDIR}/rmol-stable \
   -DWITH_RMOL_PREFIX=${INSTALL_BASEDIR}/airinv-stable \
   -DWITH_RMOL_PREFIX=${INSTALL_BASEDIR}/simfqt-stable \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DENABLE_TEST:BOOL=ON -DINSTALL_DOC:BOOL=ON \
   -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
* To build the project, type:
  make
* To test the project, type:
  make check
* To install the library (libsimcrs*.so*) and the binary (simcrs), just type:
  make install
  cd ${INSTALL_BASEDIR}
  rm -f simcrs-stable && ln -s simcrs-$CRS_VER simcrs-stable
  cd -
* To package the source files, type:
  make dist
* To package the binary and the (HTML and PDF) documentation:
  make package
* To browse the (just installed, if enabled) HTML documentation:
  midori file://${INSTALL_BASEDIR}/simcrs-$CRS_VER/share/doc/simcrs/html/index.html
* To browse the (just installed, if enabled) PDF documentation:
  evince ${INSTALL_BASEDIR}/simcrs-$CRS_VER/share/doc/simcrs/html/refman.pdf
* To run the local binary version:
  ./simcrs/simcrs -b
* To run the installed version:
  ${INSTALL_BASEDIR}/simcrs-$CRS_VER/bin/simcrs -b

Denis Arnaud (June 2015)
