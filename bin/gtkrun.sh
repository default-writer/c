#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

. "${pwd}/bin/scripts/load.sh"

"${pwd}/bin/gtkbuild.sh" --all --silent


docker run -e XDG_RUNTIME_DIR=/tmp \
           -e WAYLAND_DISPLAY=$WAYLAND_DISPLAY \
           -e FONTCONFIG_PATH=/etc/fonts \
           -v $XDG_RUNTIME_DIR/$WAYLAND_DISPLAY:/tmp/$WAYLAND_DISPLAY  \
           -e UI_MODE=--ui \
           -v "/dev/dri":"/dev/dri" \
           -v "/etc/fonts":"/etc/fonts" \
           -v "/usr/share":"/usr/share" \
           -v "/usr/lib":"/usr/lib" \
           -v "${pwd}/gtk":"/workspace/gtk" \
           --user=$(id -u):$(id -g) \
           ubuntu:22.04 "/workspace/gtk/gtk-experiment"
