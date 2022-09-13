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

#ifndef REEF_MOONSHINERS__BARIUM_ELEMENT_HPP_
#define REEF_MOONSHINERS__BARIUM_ELEMENT_HPP_

#include <reef_moonshiners/correction_element.hpp>

namespace reef_moonshiners
{

class Barium final : public CorrectionElement
{
public:
  Barium();
  ~Barium() final = default;

  double get_target_concentration() const final;

private:
  /// target is 15.0 micrograms / liter
  const double m_target_concentration = 15.0;
  /// threshold at which we double target concentration
  const double m_sponge_threshold = 8.0;
};

}  // reef_moonshiners

#endif  // REEF_MOONSHINERS__BARIUM_ELEMENT_HPP_
