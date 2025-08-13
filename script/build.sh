pushd `pwd`
mkdir -p ../elevator/build
cd ../elevator/build
cmake ..
make -j4
popd

pushd `pwd`
mkdir -p ../mainController/build
cd ../mainController/build
cmake ..
make -j4
popd

pushd `pwd`
mkdir -p ../panel/build
cd ../panel/build
cmake ..
make -j4
popd
