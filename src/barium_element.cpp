// Copyright 2022 Hunter L. Allen
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

#include <reef_moonshiners/barium_element.hpp>

namespace reef_moonshiners
{

Barium::Barium()
: CorrectionElement("Barium", 100E3, 15.0, 8.0)
{
}

double Barium::get_target_concentration() const
{
  if (get_last_measured_concentration() < m_sponge_threshold) {
    /* double to account for the "shitty barium sponge effect" */
    return m_target_concentration * 2.0;
  }
  return m_target_concentration;
}

}  // namespace reef_moonshiners
