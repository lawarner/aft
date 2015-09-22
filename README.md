# aft
Abstract Framework for Testing

### NOTE THIS IS NOT WORKING YET

##### Please note that this project is in early stages.
##### Only base and core classes are being written now.
##### Try again later.

AFT is a toolkit for automated testing.  The core logic of defining and executing 
automated tests is abstracted from any underlying specifics, such as platform, 
database, transport, etc.  AFT includes a plug-in architecture to apply testing to 
specific environments.  An emulator can be plugged in for most components.

The primary intended usage of AF is for system testing.  It is most convienent when
inserted into a system of interconnected components (processes) and emulates one or
more of these components.  It is used for testing running components and sub-systems.

AFT is not intented as a unit test framework (for C++).  There are many good C++ unit 
test frameworks such as googletest, cppunit and cxxunit.  This project uses googletest
for the various AFT C++ class libraries.

## Building

The main library and functionality relies on very few dependencies:
- g++
- GNU make

The unit tests depend on googletest, but these are optional and not run by the
default targets.

The plugin directory and specific UI tools will have additional dependencies, as needed.

Will probably switch the cmake when the project's structure gets more complex.

## History

### Short

A rethink of ipctest whose original goal was UI abstraction.  During implementation I 
realized that almost all of the components could be similarly abstracted, thus making 
the tool usable is many more situations.

### Long

Maybe another day . . .

OK a little:  I have written several automated testing tools for various projects.  Each of 
these ran only on specific plaforms, fixed transport, data formats, etc.  But in each case 
the actual logic of defining, recording and executing test cases was generally the same.

# Copyright Information

The AFT package is licensed under the Apache License, Version 2.0.
