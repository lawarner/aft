#pragma once
/*
 *   Copyright 2015, 2016 Andy Warner
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#define PLATFORM Posix
#define PLATFORM_NAME "Posix"

#define PosixPluginLoader PlatformPluginLoader
#define PosixThreadHandler PlatformThreadHandler

#define PTHREAD_INIT -1

#include "osdep/posix/posixpluginloader.h"
#include "osdep/posix/posixthread.h"
