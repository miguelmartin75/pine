# pine

Pine is a general, lightweight, header-only C++ library, which is designed to make organisation of a game much simpler.

## Author

Miguel Martin - [miguel.martin.7.5@hotmail.com](mailto:miguel.martin7.5@hotmail.com)

## Tested Compilers

- clang 3.1/Apple Clang Version 4.1 (LLVM 3.1svn)

## Dependencies

- chrono
	- either `boost::chrono` or `std::chrono`, you may change this in configuration* file.

*see below on how to configure the library


## Installation 
To install the library, simply drop and drag the headers into your project file (or reference to them on your local system). Since this library is header-only, there is no need to compile :)

## Configuration
You may configure the library to suit your needs. For example, this library depends on `chrono`, however you may use either boost or the C++11 standard's implementation of the library, it is your choice.

To configure the library, look in the `config.hpp` file in the `pine` directory. Alter the macros present in there to change the library's settings, a comment above each macro should describe what each one does.

## Basic Usage

The Pine Game Framework is simply a framework to help orgnisation of your games. In order to do that, it requires heavy use of C++ templates throughout the code; but do not worry, as typedef's are used frequently through the source code. All code is defined in the `pine` namespace, and it is assumed in this basic tutorial that we are using the `pine` namespace.

TODO
