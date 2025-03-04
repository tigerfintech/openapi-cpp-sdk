# 编译步骤
## 环境要求
- Linux/Mac: GCC/Clang, CMake 3.0+
- Windows: Visual Studio 2015+, vcpkg



## Linux/Mac

### 安装 Boost
[参考文档](https://www.boost.org/doc/libs/1_81_0/more/getting_started/unix-variants.html)

安装方式一：使用 homebrew
```shell
brew install boost 
```

安装方式二：源码安装
1. 下载源码到任意路径： https://www.boost.org/users/history/version_1_81_0.html,  
   以 /usr/local/ 为例 (如果没有权限，请命令前加 sudo)
    ```shell
    cd /usr/local/
    wget https://archives.boost.io/release/1.81.0/source/boost_1_81_0.tar.bz2
    ```
2. 解压：
    ```
    tar --bzip2 -xf boost_1_81_0.tar.bz2
    ```
3. 编译 (注意权限, 如有报错可前面加sudo重试)
    ```shell
    cd /usr/local/boost_1_81_0
    ./bootstrap.sh
    ./b2 headers
    ./b2
    ```


### 编译安装 cpprestsdk
```
cd /tmp  # 选择暂存源码的目录，此处用 /tmp，也可根据本机环境指定其他系统路径
git clone https://github.com/microsoft/cpprestsdk.git
cd cpprestsdk
git submodule update --init
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug-DBOOST_ROOT=/usr/local/boost_1_81_0 -DCMAKE_INSTALL_PREFIX=/usr/local/opt/cpprest ..
make -j 8
make install
```
如果找不到openssl或boost 或其他错误，可尝试指定路径，关闭某些模块 (如果有报错最好删除build目录重新创建)：
```
cmake .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX=/usr/local/opt/cpprest \
  -DBUILD_SHARED_LIBS=ON \
  -DBUILD_SAMPLES=OFF \
  -DBUILD_TESTS=OFF \
  -DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl@3 \
  -DOPENSSL_CRYPTO_LIBRARY=/opt/homebrew/opt/openssl@3/lib/libcrypto.dylib \
  -DOPENSSL_SSL_LIBRARY=/opt/homebrew/opt/openssl@3/lib/libssl.dylib \
  -DOPENSSL_INCLUDE_DIR=/opt/homebrew/opt/openssl@3/include \
  -DBOOST_ROOT=/usr/local/boost_1_81_0 \
  -DCMAKE_CXX_FLAGS="-Wno-error=null-pointer-subtraction" 
```

**说明**
[参考文档](https://github.com/Microsoft/cpprestsdk/wiki/Getting-Started-Tutorial)
直接通过包管理软件安装的 cpprestsdk 可能不是最新版本， 可能会有未知问题

### 编译安装 protobuf
使用 v3.1.0 版本
```
cd /tmp  # 选择暂存源码的目录，此处用 /tmp，也可根据本机环境指定其他系统路径
git clone https://github.com/protocolbuffers/protobuf
cd protobuf
git checkout v3.1.0
mkdir cmake_build
cd cmake_build
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local/opt/protobuf \
    -Dprotobuf_BUILD_TESTS=OFF \
    -Dprotobuf_BUILD_SHARED_LIBS=ON
make -j 8
make install
```

### 编译 tigerapi sdk
若使用编译好的sdk，此步骤可跳过  
准备工作：根据系统环境调整 CMakeLists.txt 的相关路径

1. 进入源码目录
2. 创建编译目录
   ```
   mkdir build
   cd build
   ```
3. 编译
```
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug \
    -DBOOST_ROOT=/usr/local/boost_1_81_0  ..
make -j 6
make install
```

### 安装 tigerapi sdk
将头文件放入头文件路径， 如 /usr/local/include
将库文件放入库文件路径， 如 /usr/local/lib/


### 验证测试
编译运行demo项目
```
cd demo
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug \
    -DBOOST_ROOT=/usr/local/boost_1_81_0  ..
make -j 8
./openapi_cpp_test
```



## Windows
依赖：Visual Stodio C++ 开发环境 https://visualstudio.microsoft.com/zh-hans/
下载安装Visual Stodio Installer，勾选 "使用C++的桌面开发"

使用[vcpkg](https://vcpkg.io/en/getting-started.html)


### 安装 boost
如使用sdk自带的库文件，此步骤可跳过
[参考文档](https://www.boost.org/doc/libs/1_81_0/more/getting_started/windows.html)
``` 
vcpkg install boost
```

### 安装 openssl
如使用sdk自带的库文件，此步骤可跳过
``` 
vcpkg install openssl
```

### 安装 cpprestsdk
如使用sdk自带的库文件，此步骤可跳过
```
PS> vcpkg install cpprestsdk cpprestsdk:x64-windows
```
### 编译protobuf
git clone https://github.com/protocolbuffers/protobuf
```
git checkout v3.1.0

使用cmake-gui生成windows解决方案
```
解决方案中选择“libprotobuf”工程生成pb依赖库，代码生成配置为/MDd和/MD,
解决方案下创建lib目录，按需依次创建其他层级为：lib/x86/Debug、lib/x86/Release、lib/x64/Debug、lib/x64/Release
将pb解决方案编译好的lib库依次拷贝到对应的层级目录，Debug：libprotobufd.lib，Release：libprotobuf.lib
```
```

### 安装 tigerapi sdk
源码output里的dll或lib为sdk编译后的库，项目配置引入后即可调用sdk





# 快速开始
安装后，可参照demo目录的示例



# 常见编译问题
1. mac系统 dyld: Library not loaded: /usr/local/opt/icu4c/lib/libicudata.70.dylib
``` 
dyld: Library not loaded: /usr/local/opt/icu4c/lib/libicudata.70.dylib
  Referenced from: /usr/local/opt/boost/lib/libboost_log-mt.dylib
  Reason: image not found
```
Fix:
``` 
cd /usr/local/opt/icu4c/lib
ln -s libicudata.dylib libicudata.70.dylib
ln -s libicui18n.dylib libicui18n.70.dylib
ln -s libicuuc.dylib libicuuc.70.dylib
```
将环境变量添加到shell配置：(如 ~/.bashrc 或 ~/.zshrc)，添加后需重启shell生效
```shell
export LDFLAGS="-L/usr/local/opt/icu4c/lib"
export CPPFLAGS="-I/usr/local/opt/icu4c/include"
```
2. mac 系统编译 cpprestsdk 时，报错：
   ```
   CMake Error at Release/cmake/cpprest_find_openssl.cmake:40 (list):
  list GET given empty list
Call Stack (most recent call first):
  Release/cmake/cpprest_find_websocketpp.cmake:18 (cpprest_find_openssl)
  Release/src/CMakeLists.txt:68 (cpprest_find_websocketpp)
```
Fix 指定openssl路径：
```
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl@3 \
  -DOPENSSL_CRYPTO_LIBRARY=/opt/homebrew/opt/openssl@3/lib/libcrypto.dylib \
  -DOPENSSL_SSL_LIBRARY=/opt/homebrew/opt/openssl@3/lib/libssl.dylib \
  -DOPENSSL_INCLUDE_DIR=/opt/homebrew/opt/openssl@3/include \
  -DBOOST_ROOT=/usr/local/boost_1_81_0
```

