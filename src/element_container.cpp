// Copyright 2023 Hunter L. Allen
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <reef_moonshiners/element_container.hpp>

namespace reef_moonshiners
{

ElementContainer & ElementContainer::get()
{
  if (nullptr == m_p_instance) {
    m_p_instance = new ElementContainer();
  }
  return *m_p_instance;
}

constexpr ElementBase * ElementContainer::operator[](std::string_view _name)
{
  if (_name == "Barium") {
      return &m_barium;
  } else if (_name == "Bromine") {
    return &m_bromine;
  } else if (_name == "Boron") {
    return &m_boron;
  } else if (_name == "Fluorine") {
    return &m_fluorine;
  } else if (_name == "Molybdenum") {
    return &m_molybdenum;
  } else if (_name == "Nickel") {
    return &m_nickel;
  } else if (_name == "Zinc") {
    return &m_zinc;
  } else if (_name == "Strontium") {
    return &m_strontium;
  } else if (_name == "Potassium") {
    return &m_potassium;
  } else if (_name == "Manganese") {
    return &m_manganese;
  } else if (_name == "Chromium") {
    return &m_chromium;
  } else if (_name == "Selenium") {
    return &m_selenium;
  } else if (_name == "Cobalt") {
    return &m_cobalt;
  } else if (_name == "Iron") {
    return &m_iron;
  } else if (_name == "Iodine") {
    return &m_iodine;
  } else if (_name == "Vanadium") {
    return &m_vanadium;
  } else if (_name == "Rubidium") {
    return &m_rubidium;
  }
  return nullptr;
}
  
}  // namespace reef_moonshiners
