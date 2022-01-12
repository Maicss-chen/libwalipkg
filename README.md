# 说明
这是一个基于libpackagekit包装的C++类库，简化了大部分接口。依赖libpackagekit。其中main.cpp为该库的使用案例，该库尚且需要完善和补充。packagekitmm意为package minus minus（参考GTMMM的命名方案）。

# 使用方法
## 示例代码的构建
```bash
mkdir build
cd build
cmake ..
make
```
## 在你的程序中引用此库
由于该库仅仅是PackageKit库的C++包装，因此你需要首先安装如下包：
```bash
sudo apt install libpackagekit-glib2-dev
```
再将src目录下的代码加入到你的代码结构中。

然后在您使用的构建工具中引入该库：

CMake的引入方式如下：
```cmake
find_package(PkgConfig)
pkg_check_modules(PackageKit REQUIRED IMPORTED_TARGET packagekit-glib2)
include_directories(${PackageKit_INCLUDE_DIRS})
```
最后将`packagekit-glib2`库连接到您的程序。
