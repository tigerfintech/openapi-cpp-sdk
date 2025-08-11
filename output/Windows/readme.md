重新生成output的步骤：

1. 进入 openapi-cpp-sdk\output\Windows\x64，解压Debug-MD, Debug-MT, Release-MD, Release-MT 四个压缩包到同名目录
2. 进入 openapi-cpp-sdk\output\Windows\Win32,  解压Debug-MD, Debug-MT, Release-MD, Release-MT 四个压缩包到同名目录
3. 解决方案配置里选择平台后，vs右侧资源管理器，右键 openapi-cpp-sdk,重新生成, 依次生成以上8个目录的内容
4. 依次重新压缩，git只提交压缩包