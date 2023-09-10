#!/bin/bash

# Note that this script only work on linux
# It is used after cross-compiling the windows version with x86_64-w64-mingw32

build_dir="build";
executable_name="Emu_Intel8080.exe"
files=("resources" "$executable_name");
additional_libs=("libwinpthread-1.dll")
qt_folder="$HOME/Qt/6.5.2/mingw-w64"
prefix="x86_64-w64-mingw32";
libraries_path=("/usr/$prefix/bin"
               "/usr/$prefix/lib"
               "/usr/lib/gcc/$prefix/12-posix"
               "$qt_folder/bin"
               "/usr/local/bin")

function findDLL() {
  local ret="Not found";
  for file in "${libraries_path[@]}"; do
    file="$file/$1";
    if [ -f "$file" ]; then
      ret=$file;
      echo "$ret";
      return 0;
    fi
  done
  echo "NF"
  return 1;
}

# Create the directory on the final build location
if [ -d build ]; then
  rm -rf $build_dir;
fi
mkdir $build_dir;

# Copy the necessary files
for file in "${files[@]}" ; do
  cp -r "$file" $build_dir;
done

# Creating the lib directory
cd $build_dir || exit 1;
if [ ! -d lib ]; then
    mkdir lib;
fi

for lib in $(x86_64-w64-mingw32-objdump -p Emu_Intel8080.exe | grep dll | cut -d" " -f3); do
  found=$(findDLL "$lib");
  if [[ $found == "NF" ]]; then
    echo "$lib not found";
  else
    echo "Found $lib in $found";
    cp "$found" lib;
  fi
done

for lib in "${additional_libs[@]}"; do
  found=$(findDLL "$lib");
  if [[ $found == "NF" ]]; then
    echo "$lib not found";
  else
    echo "Found $lib in $found";
    cp "$found" lib;
  fi
done

# Copying specific Qt lib
if [ ! -d lib/plugins/platforms ]; then
    mkdir -p lib/plugins/platforms;
fi
cp "$qt_folder"/plugins/platforms/qwindows.dll lib/plugins/platforms;

# Creating the qt.conf file
if [ ! -f qt.conf ]; then
    touch qt.conf;
    echo "[Paths]" >> qt.conf;
    echo "Plugins = ./lib/plugins" >> qt.conf;
fi
# Creating the run script
if [ ! -f run.bat ]; then
  touch run.bat
  echo 'set PATH=.\lib;%PATH%' >> run.bat;
  echo 'Emu_Intel8080.exe' >> run.bat;
fi
