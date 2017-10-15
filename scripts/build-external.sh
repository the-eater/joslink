#!/usr/bin/env bash
set -e;

cd "$(dirname "$0")";

. ../../../setenv.sh

##
# Re-set prefix to something where we can write
#

echo "Configure options: $STEAMLINK_CONFIGURE_OPTS";

##
#
# libsamplerate dependency of jack2

if [ ! -d "external/libsamplerate" ]; then
 git clone https://github.com/erikd/libsamplerate.git external/libsamplerate;
fi;

cd external/libsamplerate;
./autogen.sh;
./configure $STEAMLINK_CONFIGURE_OPTS --with-libtool-sysroot="$MARVELL_ROOTFS"
steamlink_make;

# Install in rootfs for other builds
steamlink_make_install;

cd ../..;

##
#
# jack2
if [ ! -d "external/jack2" ]; then
  git clone https://github.com/jackaudio/jack2.git external/jack2;
fi;

cd external/jack2

git checkout tags/v1.9.10;

./waf configure --prefix="$JOSLINK_PREFIX"
./waf install --destdir="$MARVELL_ROOTFS";