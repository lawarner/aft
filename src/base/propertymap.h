#pragma once
//
//  propertymap.hpp
//  libaft
//
//  Copyright © 2017 Andy Warner. All rights reserved.
//

#include <unordered_map>
#include <vector>

template<typename T>
class PropertyMap {
public:
    /** Return a list of all of the property names. */
    void getPropertyNames(std::vector<std::string>& names) const {
        for (auto const& prop : nameValues_) {
            names.push_back(prop.first);
        }
    }

    /** Get the value of a property.
     *  @param name Name of value to retrieve.
     *  @param value Reference to a string where the value will be copied to.
     *  @return true if value was retrieved, otherwise false.
     */
    bool getValue(const std::string& name, T& value) const {
        auto it = nameValues_.find(name);
        if (it == nameValues_.end())  return false;
        value = it->second;
        return true;
    }

    /** Get the value of a property.
     *  @return value associated with name, or empty string if no value is
     *          associated.
     */
    const T& getValue(const std::string& name, const T& defaultValue) const {
        auto it = nameValues_.find(name);
        if (it == nameValues_.end()) {
            return defaultValue;
        }
        return it->second;
    }

    /** Associate (bind) a named value */
    void setValue(const std::string& name, const T& value) {
        nameValues_[name] = value;
    }
    
    /** Unassoicate a named value */
    bool unsetValue(const std::string& name) {
        auto it = nameValues_.find(name);
        if (it == nameValues_.end()) {
            return false;
        }
        nameValues_.erase(it);
        return true;
    }
private:
    std::unordered_map<std::string, T> nameValues_;
};
