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

#ifndef REEF_MOONSHINERS__ELEMENT_CONTAINER_HPP_
#define REEF_MOONSHINERS__ELEMENT_CONTAINER_HPP_

#include <reef_moonshiners/elements.hpp>

namespace reef_moonshiners
{

struct ElementContainer
{
public:
  ~ElementContainer() = default;

  static ElementContainer & get();
  constexpr ElementBase * operator[](std::string_view s);

private:
  ElementContainer() = default;

  /* Correction Elements */
  Barium m_barium;
  Bromine m_bromine;
  Boron m_boron;
  Fluorine m_fluorine;
  Molybdenum m_molybdenum;
  Nickel m_nickel;
  Zinc m_zinc;
  Strontium m_strontium;
  Potassium m_potassium;

  /* Daily Elements */
  Manganese m_manganese;
  Chromium m_chromium;
  Selenium m_selenium;
  Cobalt m_cobalt;
  Iron m_iron;
  Iodine m_iodine;
  Vanadium m_vanadium;

  Rubidium m_rubidium;

  inline static ElementContainer * m_p_instance = nullptr;
};
  
}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__ELEMENT_CONTAINER_HPP_
