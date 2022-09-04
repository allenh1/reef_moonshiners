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

CORRECTION_ELEMENT(Bromine, 54000E3, 85E3, 10E3);
CORRECTION_ELEMENT(Boron, 4000E3, 7E3, 1E3);
CORRECTION_ELEMENT(Flourine, 1000E3, 1.5E3, 0.1E3);
CORRECTION_ELEMENT(Rubidium, 1000E3, 0.2E3, 0.1E3);
CORRECTION_ELEMENT(Barium, 100E3, 15.0, 8.0);
CORRECTION_ELEMENT(Molybdenum, 100E3, 15.0, 3.0);
CORRECTION_ELEMENT(Nickel, 100E3, 5.0, 0.5);
CORRECTION_ELEMENT(Zinc, 1000E3, 5.0, 2.0);
CORRECTION_ELEMENT(Strontium, 54000E3, 10E3, 1E3);
CORRECTION_ELEMENT(Potassium, 49000E3, 410E3, 20E3);

DAILY_ELEMENT(Manganese, 100E3, 0.1, 1.0);
DAILY_ELEMENT(Chromium, 100E3, 0.02, 0.1);
DAILY_ELEMENT(Selenium, 100E3, 0.02, 0.1);
DAILY_ELEMENT(Cobalt, 101E3, 0.02, 0.1);
DAILY_ELEMENT(Iron, 108E3, 0.01, 0.1);


}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__ELEMENTS_HPP_
