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

std::ostream & operator<<(std::ostream & stream, const reef_moonshiners::ElementContainer & element)
{
  element.m_barium.write_to(stream);
  element.m_bromine.write_to(stream);
  element.m_boron.write_to(stream);
  element.m_fluorine.write_to(stream);
  element.m_molybdenum.write_to(stream);
  element.m_nickel.write_to(stream);
  element.m_zinc.write_to(stream);
  element.m_strontium.write_to(stream);
  element.m_potassium.write_to(stream);

  element.m_manganese.write_to(stream);
  element.m_chromium.write_to(stream);
  element.m_selenium.write_to(stream);
  element.m_cobalt.write_to(stream);
  element.m_iron.write_to(stream);

  element.m_iodine.write_to(stream);
  element.m_vanadium.write_to(stream);

  element.m_rubidium.write_to(stream);
  return stream;
}

std::istream & operator>>(std::istream & stream, reef_moonshiners::ElementContainer & element)
{
  element.m_barium.read_from(stream);
  element.m_bromine.read_from(stream);
  element.m_boron.read_from(stream);
  element.m_fluorine.read_from(stream);
  element.m_molybdenum.read_from(stream);
  element.m_nickel.read_from(stream);
  element.m_zinc.read_from(stream);
  element.m_strontium.read_from(stream);
  element.m_potassium.read_from(stream);

  element.m_manganese.read_from(stream);
  element.m_chromium.read_from(stream);
  element.m_selenium.read_from(stream);
  element.m_cobalt.read_from(stream);
  element.m_iron.read_from(stream);

  element.m_iodine.read_from(stream);
  element.m_vanadium.read_from(stream);

  element.m_rubidium.read_from(stream);
  return stream;
}

}  // namespace reef_moonshiners
