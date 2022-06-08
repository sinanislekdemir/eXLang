#!/bin/bash
meson build --buildtype=release --wipe
cd build
ninja
mv ./exlang ../samples/
cd ../samples/
