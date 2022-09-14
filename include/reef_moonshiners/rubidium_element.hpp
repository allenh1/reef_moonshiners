#ifndef REEF_MOONSHINERS__RUBIDIUM_ELEMENT_HPP_
#define REEF_MOONSHINERS__RUBIDIUM_ELEMENT_HPP_
#include <reef_moonshiners/daily_element.hpp>

namespace reef_moonshiners
{

enum class RubidiumSelection : uint8_t
{
  DAILY = 0,
  MONTHLY = 1,
  QUARTERLY = 2,
  INITIAL = 3,
};

class Rubidium final : public DailyElement
{
public:
  Rubidium();
  ~Rubidium() final = default;

  void set_initial_dose_date(const std::chrono::year_month_day & date);

  std::chrono::year_month_day get_initial_dose_date() const;

  void set_dosing_frequency(RubidiumSelection dosing_frequency);

  double get_target_concentration() const final;

  double get_dose(const std::chrono::year_month_day & date) const final;

  RubidiumSelection get_dosing_frequency() const;

  void write_to(std::ostream & stream) const final;

  void read_from(std::istream & stream) final;

private:
  const double m_daily_concentration = 0.0011E3;
  const double m_monthly_concentration = 0.033E3;
  const double m_quarterly_concentration = 0.1E3;
  const double m_initial_concentration = 0.2E3;

  mutable RubidiumSelection m_dosing_frequency = RubidiumSelection::INITIAL;
  std::chrono::year_month_day m_initial_rubidium_dose_date;
};

}  // namespace reef_moonshiners
#endif  // REEF_MOONSHINERS__RUBIDIUM_ELEMENT_HPP_
