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
   以 /usr/local/ 为例
    ```shell
    cd /usr/local/
    wget https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.bz2
    ```
2. 解压：
    ```
    tar --bzip2 -xf boost_1_81_0.tar.bz2
    ```
3. 编译
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
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make -j 8
make install
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
cmake ../cmake \
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
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make -j 6
make install
```

### 安装 tigerapi sdk
将头文件放入头文件路径， 如 /usr/local/include
将库文件放入库文件路径， 如 /usr/local/lib/


### 验证测试
编译运行demo项目



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


