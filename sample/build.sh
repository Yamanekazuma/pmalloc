#!/usr/bin/env bash

usage() {
  echo "Usage: ./build.sh {x86|x64} {debug|release}"
}

if [[ $# < 2 ]]; then
  usage
  exit
fi

if [[ "$1" != "x86" && "$1" != "x64" ]]; then
  usage
  exit
fi

if [[ "$2" != "debug" && "$2" != "release" ]]; then
  usage
  exit
fi

mkdir -p build
mkdir -p deps
rm deps/pmalloc 2> /dev/null
rm -r build/${1}-${2}-linux 2> /dev/null

ln -s ../../ deps/pmalloc                 \
  && cmake --preset=${1}-${2}-linux       \
  && cmake --build build/${1}-${2}-linux
