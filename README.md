# 编译步骤

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

### 编译安装 tigerapi sdk


## Windows
依赖：Visual Stodio C++ 开发环境 https://visualstudio.microsoft.com/zh-hans/
下载安装Visual Stodio Installer，勾选 "使用C++的桌面开发" 

### 安装 boost
[参考文档](https://www.boost.org/doc/libs/1_81_0/more/getting_started/windows.html)

- 方式一：使用源码安装
说明，对于Windows使用Cygwin或WSL的，参考Linux的步骤
1. 下载 https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.zip
2. 解压到任意目录， 如 D:\boost_1_81_0
3. 编译
    ``` 
    bootstrap.bat
    .\b2
    ```
- 方式二：使用[vcpkg](https://vcpkg.io/en/getting-started.html)
``` 
vcpkg install boost
```

### 安装 openssl
``` 
vcpkg install openssl
```

### 编译安装 cpprestsdk

- 方式一：源码安装  
1. `git clone https://github.com/Microsoft/cpprestsdk.git`
2. `vcpkg install --triplet x64-windows zlib openssl boost-system boost-date-time boost-regex boost-interprocess websocketpp brotli`
3. 编译  
``` 
cd cpprestsdk

cpprestsdk> mkdir build.x64
cpprestsdk> cd build.x64
cpprestsdk/build.x64> cmake ../Release -A x64 -DCMAKE_TOOLCHAIN_FILE=/REPLACE_THIS_WITH_PATH_TO/vcpkg/scripts/buildsystems/vcpkg.cmake
```


- 方式二：使用vcpkg  

```
PS> vcpkg install cpprestsdk cpprestsdk:x64-windows
```


### 编译 tigerapi sdk




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


