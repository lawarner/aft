#
#   Copyright (C) 2015
#   Andy Warner
#   This file is part of the aft package.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

# Simple makefile for the aft package

TOP = ../..
INCDIR = $(TOP)/src
INCS := -I$(INCDIR)

CC = g++
CCFLAGS = -std=c++14 -g $(INCS)

ARMPREFIX=arm-linux-gnueabihf
ARMAR=$(ARMPREFIX)-ar
ARMCC=$(ARMPREFIX)-g++

ANDROIDPREFIX=arm-linux-androideabi
ANDROIDAR=$(ANDROIDPREFIX)-ar
ANDROIDCC=$(ANDROIDPREFIX)-g++

LIBAFT = $(TOP)/lib/libaft.a
LIBGTEST = -lgtest

LDFLAGS = -pthread -Wl,--no-as-needed -ldl
LDLIBS =

PLUGINOBJ = sample_plugin.o
PLUGINSRC = $(PLUGINOBJ:.o=.cpp)
PLUGINSO = sample_plugin.so

SUBDIRS =

OBJS := t_basetests.o t_coretests.o t_logger.o t_osdep.o t_plugin.o t_result.o \
        t_testsuite.o t_ui.o
SRCS := $(OBJS:.o=.cpp)

PROGRAMS = t_basetests t_coretests t_logger t_osdep t_plugin t_result \
           t_testsuite t_ui

DEPCPPFLAGS = -I. $(INCS)
DEPLIBS = $(LIBAFT) $(LIBGTEST)

.cpp.o: ; $(CC) $(CCFLAGS) -c $<

all: $(PROGRAMS) $(PLUGINSO)
# subdirs

subdirs:
	for dir in $(SUBDIRS); do \
		make -C $$dir; \
	done

$(PROGRAMS): $(DEPLIBS)

# On Mac OS X, compile with:
#     g++ -dynamiclib -flat_namespace myclass.cc -o myclass.so
#     g++ class_user.cc -o class_user

$(PLUGINSO): $(LIBAFT) $(PLUGINSRC)
	$(CC) -fPIC -shared $(INCS) $(PLUGINSRC) $(LIBAFT) -o $(PLUGINSO)

arm:
	for dir in $(SUBDIRS); do \
		make -C $$dir CC=$(ARMCC) AR=$(ARMAR) ; \
	done

android:
	for dir in $(SUBDIRS); do \
		make -C $$dir CC=$(ANDROIDCC) AR=$(ANDROIDAR) ; \
	done

.PHONY: clean
clean:
	for dir in $(SUBDIRS); do \
		make -C $$dir clean; \
	done
	rm -f $(OBJS) $(PROGRAMS) $(PLUGINOBJ) $(PLUGINSO)

.PHONY: depends
depends: $(SRCS)
	for dir in $(SUBDIRS); do \
		make -C $$dir depends; \
	done
	$(CC) $(DEPCPPFLAGS) -MM $(SRCS) > .makedepends
	touch .mkdep-timestamp

.makedepends:
include .makedepends
