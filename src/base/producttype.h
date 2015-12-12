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

/**
 * Used to define type of object when multiple types are applicable.
 */
enum ProductType
{
    TYPE_NONE,
    TYPE_TOBJECT,
    TYPE_RESULT,
    TYPE_BLOB
};

/**
 *  Used to control how producers parse their sources.
 */
enum ParcelType
    {
        PARCEL_BLOB_CHARACTER,
        PARCEL_BLOB_WORD,
        PARCEL_BLOB_LINE,
        PARCEL_BLOB_FILE,
        PARCEL_RESULT,
        PARCEL_TOBJECT
    };

} // namespace base
} // namespace aft
