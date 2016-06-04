#pragma once
/*
 *  ui.h
 *  libaft
 *
 *  Copyright © 2016 Andy Warner. All rights reserved.
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

#include "base/prodcons.h"

namespace aft
{
namespace ui
{
// Forward reference
class UIProdConsImpl;
        
        
        /**
         *  A UI is basically a Producer/Consumer.
         *  The consumer receives commands and entities and the producer yields result values.
         */
        class UI : public aft::base::BaseProdCons
        {
        public:
            UI(int maxSize = -1);
            virtual ~UI();
            
            // Producer contract
            virtual bool read(base::TObject& object);
            virtual bool read(base::Result& result);
            virtual bool read(base::Blob& blob);
            virtual bool hasData();
            virtual bool hasObject(base::ProductType productType);
            
            // Consumer contract
            virtual bool needsData();
            
            virtual bool write(const base::TObject& object);
            virtual bool write(const base::Result& result);
            virtual bool write(const base::Blob& blob);
            
        private:
            UIProdConsImpl& impl_;
        };
        
    } // namespace core
} // namespace aft

