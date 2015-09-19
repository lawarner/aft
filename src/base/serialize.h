#pragma once
/*
 *   Copyright 2015 Andy Warner
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

namespace aft
{
namespace base
{
// Forward reference
class Blob;

/**
 *  Interface that classes must implement to serialize to and from a Blob.
 */
class SerializeContract
{
public:
    virtual Blob* serialize() = 0;
    virtual bool deserialize(const Blob* blob) = 0;
};

} // namespace base
} // namespace aft
