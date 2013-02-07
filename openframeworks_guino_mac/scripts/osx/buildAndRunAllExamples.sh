#!/bin/bash

cd ../../
for f in `find examples -iname "*.xcodeproj"`
do echo $f
cd `dirname $f`
dir="$( dirname "$f" )"
xcodebuild
./bin/$( basename "$dir" ).app/Contents/MacOS/$( basename "$dir" ) &
cd - 
done
