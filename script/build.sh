pushd `pwd`
cd ../elevator/build
cmake ..
make -j4
popd

pushd `pwd`
cd ../mainController/build
cmake ..
make -j4
popd

pushd `pwd`
cd ../panel/build
cmake ..
make -j4
popd
