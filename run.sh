pushd `pwd`
cd elevator/build
./elevator_main ../config/ev_config.json &
popd

pushd `pwd`
cd mainController/build/src
./mainController ../../config/config.json &
popd

pushd `pwd`
cd panel/build
./panel_main ../config/panel_config.json &
popd
