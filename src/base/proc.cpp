/*
 *   Copyright © 2016-2017 Andy Warner
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

#include "proc.h"
using namespace aft::base;

BaseProc::BaseProc(WriterContract* writerDelegate, ReaderContract* readerDelegate)
: producer_(writerDelegate)
, consumer_(readerDelegate) {
    
}

Result BaseProc::read(TObject& object) {
    return producer_.read(object);
}

Result BaseProc::read(Result& result) {
    return producer_.read(result);
}

Result BaseProc::read(Blob& blob) {
    return producer_.read(blob);
}

bool BaseProc::hasData() {
    return producer_.hasData();
}

bool BaseProc::hasObject(ProductType productType) {
    return hasObject(productType);
}

bool BaseProc::registerDataCallback(const ReaderContract* reader) {
    return producer_.registerDataCallback(reader);
}

bool BaseProc::unregisterDataCallback(const ReaderContract* reader) {
    return producer_.unregisterDataCallback(reader);
}

void BaseProc::flowData() {
    producer_.flowData();
    //TODO note that consumer also has a flowData
}

bool BaseProc::canAcceptData() {
    return consumer_.canAcceptData();
}

Result BaseProc::write(const TObject& object) {
    return consumer_.write(object);
}

Result BaseProc::write(const Result& result) {
    return consumer_.write(result);
}

Result BaseProc::write(const Blob& blob) {
    return consumer_.write(blob);
}

int BaseProc::write(const std::vector<TObject>& objects) {
    return consumer_.write(objects);
}

int BaseProc::write(const std::vector<Result>& results) {
    return consumer_.write(results);
}

int BaseProc::write(const std::vector<Blob>& blobs) {
    return consumer_.write(blobs);
}

bool BaseProc::registerWriteCallback(const WriterContract* writer) {
    return consumer_.registerWriteCallback(writer);
}

bool BaseProc::unregisterWriteCallback(const WriterContract* writer) {
    return consumer_.unregisterWriteCallback(writer);
}
