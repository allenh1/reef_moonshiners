#include <reef_moonshiners/daily_element.hpp>

namespace reef_moonshiners
{

DailyElement::DailyElement(const std::string & _name, const double _element_concentration)
: ElementBase(_name, _element_concentration)
{
}


double DailyElement::get_dose(const std::chrono::year_month_day &) override
{
  if (this->get_current_concentration_estimate() >= this->get_target_concentration()) {
    /* no need to supplement this, we should not be detecting these elements */
    return 0.0;
  }
}

double DailyElement::get_current_concentration_estimate() override
{
  /* under ordinary circumstances, this is constant */
  return this->get_last_mesasured_concentration();
}


void DailyElement::apply_dose(const double, const std::chrono::year_month_day &) override
{
  /* no-op for dailies */
}


}  // namespace reef_moonshiners
