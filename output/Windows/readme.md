重新生成全部 Windows SDK 包：

```bat
scripts\package_windows.bat
```

如果 8 个配置已经完成构建，只重新生成 zip：

```bat
scripts\package_windows.bat -SkipBuild
```

每个 zip 仅包含 SDK 运行与用户工程链接所需文件：

- `openapi-cpp-sdk.dll` / `openapi-cpp-sdk.lib`
- cpprestsdk、OpenSSL、Protobuf、Abseil 的运行时 DLL 和必要 import library
- OpenSSL `legacy.dll` provider

明确排除 demo/test `*.exe`、调试符号 `*.pdb`、链接中间文件 `*.exp`、
`libprotoc*` 和 `libprotobuf-lite*`。请始终使用打包脚本生成包，不要直接
压缩整个配置输出目录。Git 只提交 8 个 zip、此说明文件和打包脚本。
