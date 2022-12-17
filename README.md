# 编译步骤
1. 编译cpprestsdk (CXX需要设置为g++, export CXX="/usr/bin/g++")
```
cd libs/cpprestsdk
git submodule update --init
cd ..
sh build_libs.sh
```

# 常见编译问题
1. dyld: Library not loaded: /usr/local/opt/icu4c/lib/libicudata.70.dylib
``` 
dyld: Library not loaded: /usr/local/opt/icu4c/lib/libicudata.70.dylib
  Referenced from: /usr/local/opt/boost/lib/libboost_log-mt.dylib
  Reason: image not found
```
Fix:


