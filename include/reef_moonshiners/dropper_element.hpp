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

#ifndef REEF_MOONSHINERS__DROPPER_ELEMENT_HPP_
#define REEF_MOONSHINERS__DROPPER_ELEMENT_HPP_

#include <reef_moonshiners/daily_element.hpp>

namespace reef_moonshiners
{

class DropperElement : public reef_moonshiners::DailyElement
{
public:
  explicit DropperElement(
    const std::string & _name,
    const double _low_concentration,
    const double _high_concentration);

  ~DropperElement() override = default;

  void write_to(std::ostream & stream) const final;

  void read_from(std::istream & stream) final;

  void set_multiplier(const double) final;

  double get_dose(const std::chrono::year_month_day &) const final;

  void set_drops(const size_t _drops);

  bool is_low() const;

  bool is_high() const;

private:
  /// number of drops to dose
  size_t m_drops;
};

}  // namespace reef_moonshiners

#define DROPPER_ELEMENT(name, min_concentration, max_concentration) \
  struct name final : public DropperElement { \
    name() : DropperElement( \
        #name, (min_concentration), \
        (max_concentration)) {} \
    ~name() final = default; \
  }

#endif  // REEF_MOONSHINERS__DROPPER_ELEMENT_HPP_
