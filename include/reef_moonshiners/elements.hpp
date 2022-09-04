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

#ifndef REEF_MOONSHINERS__ELEMENTS_HPP_
#define REEF_MOONSHINERS__ELEMENTS_HPP_

#include <reef_moonshiners/daily_element.hpp>
#include <reef_moonshiners/correction_element.hpp>

namespace reef_moonshiners
{

CORRECTION_ELEMENT(Flourine, 1.5 * 1E6, 1.5 * 1E3, 0.1 * 1E3);

DAILY_ELEMENT(Selenium, 1E5, 0.02, 0.1);
DAILY_ELEMENT(Chromium, 1E5, 0.02, 0.1);
DAILY_ELEMENT(Cobalt, 1E5, 0.02, 0.1);
DAILY_ELEMENT(Iron, 1E5, 0.01, 0.1);

}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__ELEMENTS_HPP_
