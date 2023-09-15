#!/bin/bash

build_dir="build";
executable_name="Emu_Intel8080"
files=("resources");
additional_libs=("libQt6XcbQpa.so.6" "libQt6OpenGL.so.6")
platforms_libs=("libqxcb.so")
excluded_libs=("libc.so.6" "libm.so.6" "librt.so.1" "libdl.so.2" "libpthread.so.0" "libstdc++.so.6" "libfontconfig.so.1" "libglib-2.0.so.0" "libasound.so.2" "libsystemd.so.0" "libgpg-error.so.0")
qt_folder="$HOME/Qt/6.5.2/gcc_64"

# Create the directory on the final build location
if [ -d build ]; then
  rm -rf $build_dir;
fi
mkdir $build_dir;

# Copy the executable
mkdir $build_dir/bin
cp $executable_name $build_dir/bin

# Copy the necessary files
for file in "${files[@]}" ; do
  cp -r "$file" $build_dir;
done

# Creating the lib directory
cd $build_dir || exit 1;
if [ ! -d lib ]; then
    mkdir lib;
fi

# Copying the libs
mkdir tmp
for lib in $(ldd ./bin/$executable_name | cut -d' ' -f3); do
  if [[ $lib == *"Qt"* ]]; then
    cp "$qt_folder"/lib/"$(echo $lib | rev | cut -d"/" -f1 | rev)" lib;
  elif [[ $lib == *"libicu"* ]]; then # For some reason the libicu lib is changing shared object names between folder and/or system
    lib_name=$(echo "$lib" | cut -d"/" -f4);
    new_name=${lib_name//"72"/"56"};
    cp "$lib" lib;
    cp "$lib" tmp;
    mv "tmp/$lib_name" lib/"$new_name";
  else
    cp "$lib" lib;
  fi
done
rm -rf tmp

for lib in "${additional_libs[@]}"; do
  if [[ $lib == *"Qt"* ]]; then
    cp "$qt_folder"/lib/"$lib" lib;
  else
    cp "$lib" lib;
  fi
done

# Copying specific Qt lib
if [ ! -d lib/plugins/platforms ]; then
    mkdir -p lib/plugins/platforms;
fi

mkdir tmp;
for platform_lib in "${platforms_libs[@]}"; do
  cp "$qt_folder"/plugins/platforms/"$platform_lib" lib/plugins/platforms;
  for lib in $(ldd lib/plugins/platforms/"$platform_lib" | cut -d' ' -f3); do
    if [[ $lib == *"Qt"* ]]; then
      cp "$qt_folder"/lib/"$(echo "$lib" | rev | cut -d"/" -f1 | rev)" lib;
    elif [[ $lib == *"libicu"* ]]; then # For some reason the libicu lib is changing shared object names between folder and/or system
      lib_name=$(echo "$lib" | cut -d"/" -f4);
      new_name=${lib_name//"72"/"56"};
      cp "$lib" lib;
      cp "$lib" tmp;
      mv "tmp/$lib_name" lib/"$new_name";
    else
      cp "$lib" lib;
    fi
  done
done
rm -rf tmp;

# Removing excluded libraries
for lib in "${excluded_libs[@]}"; do
  rm lib/"$lib";
done

# Creating the qt.conf file
cd bin || exit 1;
if [ ! -f qt.conf ]; then
    touch qt.conf;
    echo "[Paths]" >> qt.conf;
    echo "Plugins = ../lib/plugins" >> qt.conf;
fi

cd ..
# Copying the wrapper
cp ../wrapper/Emu_Intel8080_Wrapper ./;
mv Emu_Intel8080_Wrapper SpaceInvader;
