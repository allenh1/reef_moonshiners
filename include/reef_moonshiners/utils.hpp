// Copyright 2023 Hunter L. Allen
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

#ifndef REEF_MOONSHINERS__UTILS_HPP_
#define REEF_MOONSHINERS__UTILS_HPP_

#include <ostream>
#include <istream>
#include <cmath>

namespace reef_moonshiners
{

template<size_t places>
constexpr double round_places(const double d)
{
  return round_places<places - 1>(d * 10) / 10;
}

template<>
constexpr double round_places<1>(const double d)
{
  return std::ceil(d * 10) / 10;
}

template<>
constexpr double round_places<0>(const double d)
{
  return std::round(d);
}

template<size_t places>
constexpr double truncate_places(const double d)
{
  return truncate_places<places - 1>(d * 10) / 10;
}

template<>
constexpr double truncate_places<1>(const double d)
{
  return std::floor(d * 10) / 10;
}

constexpr double gallons_to_liters(const double gallons)
{
  return gallons * 3.78541;
}

constexpr double liters_to_gallons(const double liters)
{
  return liters / 3.78541;
}

template<typename T>
concept trivial_type = std::is_trivial_v<T>;

template<typename T>
concept string_type = requires(T & t) {
  t.c_str();
  t.size();
  T("foo");
};

template<trivial_type T>
constexpr void binary_out(std::ostream & stream, const T & obj)
{
  stream.write(
    reinterpret_cast<const char *>(&obj),
    sizeof(obj));
}

template<string_type S>
constexpr void binary_out(std::ostream & stream, const S & obj)
{
  const size_t len = obj.size();
  stream.write(reinterpret_cast<const char *>(&len), sizeof(len));
  stream.write(obj.c_str(), len);
}

template<typename K, typename V>
constexpr void binary_out(std::ostream & stream, const std::unordered_map<K, V> & obj)
{
  const size_t len = obj.size();
  stream.write(reinterpret_cast<const char *>(&len), sizeof(len));
  for (const auto &[key, value] : obj) {
    binary_out(stream, key);
    binary_out(stream, value);
  }
}

template<trivial_type T>
constexpr void binary_in(std::istream & stream, T & obj)
{
  stream.read(
    reinterpret_cast<char *>(&obj),
    sizeof(obj));
}

template<string_type S>
constexpr void binary_in(std::istream & stream, S & obj)
{
  size_t len;
  stream.read(reinterpret_cast<char *>(&len), sizeof(len));
  char * str_in = reinterpret_cast<char *>(calloc(len + 1, sizeof(*str_in)));
  stream.read(str_in, len);
  obj = S(str_in);
  free(str_in);
}

template<typename K, typename V>
constexpr void binary_in(std::istream & stream, std::unordered_map<K, V> & obj)
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

}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__UTILS_HPP_
