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

# Note the base directory holds primary base and abstract classes

TOP = ../..
INCDIR = $(TOP)/src

#AR = ar
CC = g++
CCFLAGS = -std=c++14 -Wall -g -fPIC -I$(TOP) -I$(INCDIR)
DEPCPPFLAGS = -std=c++14 -I$(TOP) -I$(INCDIR)

OBJS := blob.o command.o consumer.o context.o entity.o factory.o hasher.o operation.o \
    plugin.o proc.o producer.o propertyhandler.o result.o structureddata.o \
    structureddataname.o thread.o tobasictypes.o tobject.o tobjectiterator.o \
    tobjecttree.o tobjecttype.o

SRCS := $(OBJS:.o=.cpp)
INCS = $(OBJS:.o=.h)

LIBR = libaftbase.a


.cpp.o: ; $(CC) $(CCFLAGS) -c $<


all: $(LIBR)

$(LIBR): $(OBJS)
	$(AR) rsv $@ $(OBJS)


.PHONY: clean
clean:
	rm -f $(OBJS) $(LIBR)

.PHONY: depends
depends: $(SRCS) $(INCS)
	$(CC) $(DEPCPPFLAGS) -MM $(SRCS) > .makedepends
	touch .mkdep-timestamp

.makedepends:
include .makedepends
