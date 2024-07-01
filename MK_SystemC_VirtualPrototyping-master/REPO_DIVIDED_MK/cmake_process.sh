# remove old folder, create new, go to it, create makefile, build the project
rm -r build
mkdir build
cd build
cmake ..
make -j 6
