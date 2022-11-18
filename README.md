# C++ Dev Server
That's pretty simple utility which can make coding GUI applications using C++ even easier! It watches folder that you provide for file changes, and when changes occur, executes command that you provide, and... your application has been restarted.
## Compilation
Compilation is pretty easy, just follow those steps:
```
cd cpp-dev-server
mkdir build
cd build
cmake -GNinja ..
ninja -j 16
```
Fantastic! C++ Dev Server was compiled.
## Links
There's anything that was used:

- [CMake](https://cmake.org/)
- [Watcher](https://github.com/e-dant/watcher)
- [JSON for Modern C++](https://github.com/nlohmann/json)

## Contributing
- If you have any ideas how to improve this project, open an issue and describe your idea.
- If you want to contribute, fork this project, change what you want and open new pull request. If your changes makes this tiny project better, it will probably be merged.
- If you find any bugs, feel free to open issue and use template for bugs.

## License
MIT. Feel free to use anything from this repository.
