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

TOP = ..

ARMPREFIX=arm-linux-gnueabihf
ARMAR=$(ARMPREFIX)-ar
ARMCC=$(ARMPREFIX)-g++

ANDROIDPREFIX=arm-linux-androideabi
ANDROIDAR=$(ANDROIDPREFIX)-ar
ANDROIDCC=$(ANDROIDPREFIX)-g++

LIBSDIR = $(TOP)/lib
LIBAFT = $(LIBSDIR)/libaft.a

SUBDIRS = base core
# storage transport plugin ui

PROGRAMS = tests

all: subdirs lib $(PROGRAMS)

.PHONY: lib
lib:
	rm -f $(LIBAFT)
	make $(LIBAFT)

.PHONY: tests
tests: lib
	make -C tests

$(LIBAFT):
	mkdir -p $(LIBSDIR)
	rm -f tests/*.o
	$(AR) rsv $@ */*.o

subdirs:
	for dir in $(SUBDIRS); do \
		make -C $$dir; \
	done


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
	for dir in $(SUBDIRS) tests; do \
		make -C $$dir clean; \
	done

.PHONY: depends
depends:
	for dir in $(SUBDIRS) tests; do \
		make -C $$dir depends; \
	done
