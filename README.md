# aft
Abstract Framework for Testing

#### Note that this project is in early stages.
##### Only first simple scenario written.
##### base and core classes are fairly stable and ui classes are still in flux. More commands and plugins are needed.


AFT is a toolkit for automated testing.  The core logic of defining and executing 
automated tests is abstracted from any underlying specifics, such as platform, 
database, transport, etc.  AFT includes a plug-in architecture to apply testing to 
specific environments.  An emulator can be plugged in for most components.

The primary intended usage of AFT is for system testing.  It is most convienent when
inserted into a system of interconnected components (processes) and emulates one or
more of these components.  It is used for testing running components and sub-systems.

AFT is not intented as a unit test framework (for C++).  There are many good C++ unit 
test frameworks such as googletest, cppunit and cxxunit.  This project uses googletest
for unit testing the various AFT C++ classes.

## Building

### Linux

The main library and functionality relies on very few dependencies:
- g++
- GNU make

The unit tests depend on googletest, but these are optional and can be removed from 
default 'make' targets.

The plugin directory and specific UI tools will have additional dependencies, as needed.

The AFT Makefile structure is very minimal (read "primitive").
Will probably switch to cmake when the project's structure gets more complex.

### MacOSX

An xcode workspace is in the aft/macosx folder that contains the following xcode projects:
- **libaft** Builds the static library.
- **libafttests** Builds unit/module test programs.  The test programs depend on googletest so the project searches for
  headers in /opt/local/include.  You need to copy libgtest.a to the aft/lib folder.  Or modify the project if you
  have installed googletest elsewhere.
- **examples** Builds example programs that illustrate using the AFT framework.

### Other Platforms

Any platform that supports POSIX, g++ and the c++14 standard should compile and run without problems. But any
errors that may arise should be isolated to classes within osdep. Currently the osdep section is only involved
in the UI, threads and dynamic librarires, but more could be added in the future.

## History

### Short

This project is a rethink of ipctest (a previous testing tool project) whose original
goal was UI abstraction.  While implementing ipctest I realized that almost all of the 
other components could be similarly abstracted, thus making 
the tool much more useful. This project also draws from previous testing tools that I have
written in C++ and one in Java.

### Long

Maybe another day . . .

OK a little:  I have written several automated testing tools for various projects.  Each of 
these ran only on a specific plaform, fixed transport, data formats, etc.  But in each case 
the actual logic of defining, recording and executing test cases was generally the same.

A UI can be constructed to allow easy input of steps that are added, edited, executed, saved, restored,
debugged, etc. These are expressed as test suites and test cases.

# Copyright Information

The AFT package is licensed under the Apache License, Version 2.0.
