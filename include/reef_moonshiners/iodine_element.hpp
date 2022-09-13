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

#ifndef REEF_MOONSHINERS__IODINE_ELEMENT_HPP_
#define REEF_MOONSHINERS__IODINE_ELEMENT_HPP_

#include <reef_moonshiners/daily_element.hpp>

namespace reef_moonshiners
{

class Iodine final : public reef_moonshiners::DailyElement
{
public:
  Iodine();
  ~Iodine() final = default;

  void write_to(std::ostream & stream) const final;

  void read_from(std::istream & stream) final;

  void set_multiplier(const double) final;

  double get_dose(const std::chrono::year_month_day &) const final;

  void set_drops(const size_t _drops);

  bool is_low() const;

  bool is_high() const;

private:
  /// number of drops to dose
  size_t m_drops = 2;
  /// maximum concentration, micrograms per liter
  const double m_max_concentration = 95.0;
};

}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__IODINE_ELEMENT_HPP_
