# pmalloc
pmalloc is a executable-memory allocator for Windows.

## How to use
I have two ideas.

---

### Install theis library on your system

At first, follow the Visual Studio GUI to install the library.

Or, using the "Developer Command Prompt for VS", execute the following commands to install the software:

```sh
cd pmalloc
cmake --preset [x86-debug|x86-release|x64-debug|x64-release]
cmake --build out/build/[preset-name]
cmake --install out/build/[preset-name]
```

Then, move created files to the pmalloc directory in the search target directory of cmake's find_package command.

And then add the following to your CMakeLists.txt:

```cmake
find_package(pmalloc)
target_link_library(${PROJECT_NAME} PRIVATE pmalloc::pmalloc)
```

---

### Use this repository as a submodule of git

At first, add it as a submodule as follows:

```sh
cd (your project)
git init
mkdir deps/
git submodule add https://github.com/Yamanekazuma/pmalloc.git deps/pmalloc
```

And then add the following to your CMakeLists.txt:

```cmake
add_subdirectory(deps/pmalloc EXCLUDE_FROM_ALL)
target_link_library(${PROJECT_NAME} PRIVATE pmalloc)
```

---

## License
- Copyright 2023 Yamanekazuma
- [Licensed under the MIT License.](LICENSE.txt)
