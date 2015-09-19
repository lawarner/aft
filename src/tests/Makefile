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
INCDIR = $(TOP)/include

CC = g++
CCFLAGS = $(INCS)

ARMPREFIX=arm-linux-gnueabihf
ARMAR=$(ARMPREFIX)-ar
ARMCC=$(ARMPREFIX)-g++

ANDROIDPREFIX=arm-linux-androideabi
ANDROIDAR=$(ANDROIDPREFIX)-ar
ANDROIDCC=$(ANDROIDPREFIX)-g++

LIBAFT = $(TOP)/lib/libaft.a

LDFLAGS = -pthread
LDLIBS =

SUBDIRS =

OBJS := t_logger.o t_result.o
SRCS := $(OBJS:.o=.cpp)
INCS := -I$(TOP)/src

PROGRAMS = t_logger t_result

DEPCPPFLAGS = -I. $(INCS)
DEPLIBS = $(LIBAFT)

.cpp.o: ; $(CC) $(CCFLAGS) -c $<

all: subdirs $(PROGRAMS)

subdirs:
	for dir in $(SUBDIRS); do \
		make -C $$dir; \
	done

$(PROGRAMS): $(DEPLIBS)

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
	rm -f $(OBJS) $(PROGRAMS)

.PHONY: depends
depends: $(SRCS)
	for dir in $(SUBDIRS); do \
		make -C $$dir depends; \
	done
	$(CC) $(DEPCPPFLAGS) -MM $(SRCS) > .makedepends
	touch .mkdep-timestamp

.makedepends:
include .makedepends
