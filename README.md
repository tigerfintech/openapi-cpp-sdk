# 编译步骤
1. 安装 cpprestsdk
[参考文档](https://github.com/Microsoft/cpprestsdk/wiki/Getting-Started-Tutorial)

With vcpkg on Windows
```
PS> vcpkg install cpprestsdk cpprestsdk:x64-windows
```

With apt-get on Debian/Ubuntu
```
$ sudo apt-get install libcpprest-dev
```

With brew on OSX
```
$ brew install cpprestsdk
```

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
添加到shell环境变量：



