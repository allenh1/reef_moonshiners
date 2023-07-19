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
  ElementContainer() = default;
  ~ElementContainer() = default;

  constexpr ElementBase * operator[](std::string_view _name)
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

  friend std::istream & operator>>(std::istream & stream, ElementContainer & element);

  friend std::ostream & operator<<(std::ostream & stream, const ElementContainer & element);

  constexpr std::vector<DailyElement *> get_dailies()
  {
    return {
      & m_manganese,
      & m_chromium,
      & m_selenium,
      & m_cobalt,
      & m_iron,
      & m_iodine,
      & m_vanadium
    };
  }

  constexpr std::vector<CorrectionElement *> get_corrections()
  {
    return {
      & m_barium,
      & m_bromine,
      & m_boron,
      & m_fluorine,
      & m_molybdenum,
      & m_nickel,
      & m_zinc,
      & m_strontium,
      & m_potassium
    };
  }

  constexpr Rubidium * rubidium()
  {
    return &m_rubidium;
  }

  constexpr Iodine * iodine()
  {
    return &m_iodine;
  }

  constexpr Vanadium * vanadium()
  {
    return &m_vanadium;
  }

private:
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
};

}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__ELEMENT_CONTAINER_HPP_
