#ifndef REEF_MOONSHINERS__CORRECTION_ELEMENT_HPP_
#define REEF_MOONSHINERS__CORRECTION_ELEMENT_HPP_

#include <reef_moonshiners/element_base.hpp>

#include <unordered_map>

namespace std
{
template<>
struct hash<std::chrono::year_month_day>
{
  size_t operator()(const std::chrono::year_month_day & date) const
  {
    std::chrono::sys_time<std::chrono::days> days{std::chrono::sys_days{date}};
    return days.time_since_epoch().count();
  }
};

namespace chrono
{

/**
 * @brief addition operator overload
 *
 * I legit don't understand how this didn't make the standard...
 *
 * @param lhs date to add to
 * @param rhs number of days to increase it
 *
 * @return rhs days after lhs
 */
constexpr year_month_day operator+(const year_month_day & lhs, const days & rhs)
{
  return year_month_day(sys_days{lhs} + rhs);
}
}  // namespace chrono
}  // namespace std

namespace reef_moonshiners
{

class CorrectionElement : public ElementBase
{
public:
  explicit CorrectionElement(
    const std::string & _name, const double _element_concentration,
    const double _target_concentration, const double _max_adjustment);
  ~CorrectionElement() override = default;

  double get_dose(const std::chrono::year_month_day &) override;

  double get_current_concentration_estimate() const override;

  void apply_dose(const double _dose, const std::chrono::year_month_day & _date) override;

private:
  /// map of date -> mL dosed
  std::unordered_map<std::chrono::year_month_day, double> m_dosed_amounts;
};

}  // namespace reef_moonshiners

#define CORRECTION_ELEMENT(name, element_concentration, target_concentration, max_adjustment) \
  struct name : public CorrectionElement { \
    name() : CorrectionElement( \
        #name, (element_concentration), (target_concentration), \
        (max_adjustment)) {} }

#endif  // REEF_MOONSHINERS__CORRECTION_ELEMENT_HPP_
