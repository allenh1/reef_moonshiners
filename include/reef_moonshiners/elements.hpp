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
