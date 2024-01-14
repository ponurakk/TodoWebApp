#!/bin/bash

build() {
  mkdir -p build
  cd build
  cmake ..
  make
}

clean() {
  rm -r build/
}

run() {
  ./build/todowebapp
}

if [ "$1" == "run" ]; then
  run
elif [ "$1" == "build" ]; then
  build
elif [ "$1" == "clean" ]; then
  clean
else
  echo "Usage: $0 {run|build|clean}"
fi
