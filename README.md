# openjtalk-test

A bunch of unit tests for OpenJTalk to understand what is going on. This project is just for my study.


## Build

```
npm install
```

To create project of, for example, xcode,

```
gyp test.gyp --depth=. -f xcode --generator-output=./build.xcodefiles/ -D OS=mac -D target_arch=x64 -D module_root_dir=`pwd`/node_modules/node-openjtalk
```
