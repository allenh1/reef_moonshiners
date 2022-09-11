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

#ifndef REEF_MOONSHINERS__ELEMENT_BASE_HPP_
#define REEF_MOONSHINERS__ELEMENT_BASE_HPP_

#include <string>
#include <cmath>
#include <chrono>
#include <unordered_map>
#include <ostream>
#include <istream>

namespace reef_moonshiners
{

template<size_t places>
inline double round_places(const double d)
{
  return round_places<places - 1>(d * 10) / 10;
}

template<>
inline double round_places<1>(const double d)
{
  return std::ceil(d * 10) / 10;
}

template<>
inline double round_places<0>(const double d)
{
  return std::round(d);
}

template<size_t places>
inline double truncate_places(const double d)
{
  return truncate_places<places - 1>(d * 10) / 10;
}

template<>
inline double truncate_places<1>(const double d)
{
  return std::floor(d * 10) / 10;
}

inline double gallons_to_liters(const double gallons)
{
  return gallons * 3.78541;
}

inline double liters_to_gallons(const double liters)
{
  return liters / 3.78541;
}

template<typename T>
inline void binary_out(std::ostream & stream, const T & obj)
{
  stream.write(
    reinterpret_cast<const char *>(&obj),
    sizeof(obj));
}

template<>
inline void binary_out<std::string>(std::ostream & stream, const std::string & obj)
{
  const size_t len = obj.size();
  stream.write(reinterpret_cast<const char *>(&len), sizeof(len));
  stream.write(obj.c_str(), len);
}

template<typename K, typename V>
inline void binary_out(std::ostream & stream, const std::unordered_map<K, V> & obj)
{
  const size_t len = obj.size();
  stream.write(reinterpret_cast<const char *>(&len), sizeof(len));
  for (const auto&[key, value] : obj) {
    binary_out(stream, key);
    binary_out(stream, value);
  }
}

template<typename T>
inline void binary_in(std::istream & stream, T & obj)
{
  stream.read(
    reinterpret_cast<char *>(&obj),
    sizeof(obj));
}

template<>
inline void binary_in<std::string>(std::istream & stream, std::string & obj)
{
  size_t len;
  stream.read(reinterpret_cast<char *>(&len), sizeof(len));
  char * str_in = reinterpret_cast<char *>(calloc(len + 1, sizeof(*str_in)));
  stream.read(str_in, len);
  obj = std::string(str_in);
  free(str_in);
}

template<typename K, typename V>
inline void binary_in(std::istream & stream, std::unordered_map<K, V> & obj)
{
  size_t len;
  binary_in(stream, len);

  K key;
  V value;
  for (; len-- > 0; ) {
    binary_in(stream, key);
    binary_in(stream, value);
    obj[key] = value;
  }
}

class ElementBase
{
public:
  ElementBase() = delete;
  /**
   * @brief Construct an element
   * @param _name Name of the element
   * @param _element_concentration Supplement concentration in micrograms per liter
   * @param _target_concentration Targeted concentration in micrograms per liter
   * @param _max_dose Maximum adjustment in micrograms per liter per day
   */
  explicit ElementBase(
    const std::string & _name, const double _element_concentration,
    const double _target_concentration, const double _max_adjustment);
  virtual ~ElementBase() = default;

  /**
   * @brief access the element name
   * @return element name
   */
  const std::string & get_name() const;

  /**
   * @brief Set the element name
   * @param _name Name of the element
   */
  void set_name(const std::string & _name);

  /**
   * @brief Returns the assumed concentration
   * @return Assumed concentration of this element, in micrograms per liter
   */
  virtual double get_current_concentration_estimate() const = 0;

  /**
   * @brief Get the dosage for a given day
   *
   * @param day Date upon which the dose will occur
   *
   * @return Dosage in milliliters
   */
  virtual double get_dose(const std::chrono::year_month_day & day) const = 0;

  /**
   * @brief Mark a dose as done for for the given date in the given ammount
   *
   * @param _dose Amount of the element dosed, in mL
   * @param _date Date the dose occured
   */
  virtual void apply_dose(const double _dose, const std::chrono::year_month_day & _date) = 0;

  /**
   * @brief set the concentration at a given time
   *
   * This is the concentration from an ICP test result.
   *
   * @param _concentration Concentration is micrograms per Liter
   * @param _date date the sample was taken
   */
  void set_concentration(const double _concentration, const std::chrono::year_month_day & _date);

  static double get_tank_size();

  double get_max_daily_dosage() const;

  double get_last_measured_concentration() const;

  const std::chrono::year_month_day & get_last_measurement_date() const;

  double get_target_concentration() const;

  double get_element_concentration() const;

  static void set_tank_size(const double _tank_size);

  /** 
   * Allow serialization for storage
   * @param stream Where to serialize
   */
  virtual void write_to(std::ostream & stream) const;

  virtual void read_from(std::istream & stream);

  friend std::istream & operator>>(std::istream & stream, ElementBase & element);

  friend std::ostream & operator<<(std::ostream & stream, const ElementBase & element);

protected:
  double _get_concentration_after_dose(
    const double _dose_ml,
    const double _prior_concentration) const;

  /**
   * @brief Access maximum dosage in liters
   * @return Maximum daily dose in Liters
   */
  double _max_daily_dosage_l(const double concentration) const;

private:
  /// name of the element
  std::string m_name;
  /// concentration in micrograms per liter
  double m_estimated_concentration = 0.0;
  /// last measurement date
  std::chrono::year_month_day m_last_measurement;
  /// last measured concentration
  double m_last_measured_concentration = 0.0;
  /// tank size (liters)
  inline static double m_tank_size = 0.0;
  /// element concentration in micrograms per liter
  double m_element_concentration;
  /// target concentration in micrograms per liter
  double m_target_concentration;
  /// maximum adjustment for this element (micrograms per liter per day)
  double m_max_adjustment;
};

}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__ELEMENT_BASE_HPP_
