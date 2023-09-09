#!/bin/bash

build_dir="build";
executable_name="Emu_Intel8080"
files=("sounds" "roms" "$executable_name");
additional_libs=("libQt6XcbQpa.so.6" "libQt6OpenGL.so.6")
qt_folder="$HOME/Qt/6.5.2/gcc_64"

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

# Copying the libs
mkdir tmp
for lib in $(ldd $executable_name | cut -d' ' -f3); do
  if [[ $lib == *"Qt"* ]]; then
    cp "$qt_folder"/lib/"$(echo $lib | cut -d"/" -f8)" lib;
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
cp "$qt_folder"/plugins/platforms/libqxcb.so lib/plugins/platforms;

# Creating the qt.conf file
if [ ! -f qt.conf ]; then
    touch qt.conf;
    echo "[Paths]" >> qt.conf;
    echo "Plugins = ./lib/plugins" >> qt.conf;
fi

# Creating the run script
if [ ! -f run.sh ]; then
  touch run.sh
  echo '#!/bin/bash' >> run.sh;
  echo 'program_name=$(echo $0 | rev | cut -d"/" -f1 | rev)' >> run.sh;
  echo 'program_dir=${0//$program_name/}' >> run.sh;
  echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$program_dir/lib' >> run.sh;
  echo '$program_dir/Emu_Intel8080' >> run.sh;
  chmod u+x run.sh;
fi