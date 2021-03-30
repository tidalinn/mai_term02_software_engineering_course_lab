# hl_event_server

## install C++
sudo apt-get install gcc g++ cmake git libssl-dev zlib1g-dev librdkafka-dev mysql-server mysql-client libmysqlclient-dev libboost-all-dev

## install java

sudo apt install openjdk-8-jdk

sudo apt install openjdk-8-jre

## install iginte

Download from https://ignite.apache.org/download.cgi#binaries

build platforms/cpp

## install CPPRDKafkfa


// https://github.com/edenhill/librdkafka
https://github.com/mfontanini/cppkafka
mkdir build
cd build
cmake <OPTIONS> ..
make
make install


## Install poco

git clone -b master https://github.com/pocoproject/poco.git

cd poco

mkdir cmake-build

cd cmake-build

cmake ..

cmake --build . --config Release

sudo cmake --build . --target install

## Install gtest
sudo apt-get install libgtest-dev

cd /usr/src/gtest/

sudo cmake -DBUILD_SHARED_LIBS=ON

sudo make

sudo cp *.so /usr/lib
