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

#ifndef REEF_MOONSHINERS__UI__ELEMENT_SETTINGS_HPP_
#define REEF_MOONSHINERS__UI__ELEMENT_SETTINGS_HPP_

#include <reef_moonshiners/element_container.hpp>

#include <memory>
#include <vector>

#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>

namespace reef_moonshiners::ui
{

class ElementSettings final : public QWidget
{
  Q_OBJECT

public:
  ElementSettings() = delete;

  explicit ElementSettings(
    reef_moonshiners::ElementContainer * const _p_element_container,
    QWidget * _parent = nullptr);

  ~ElementSettings() final = default;

  QPushButton * get_okay_button() const;

  void refresh_values();

protected:
  void resizeEvent(QResizeEvent * event) final;
  
private:
  reef_moonshiners::ElementContainer * m_p_element_container = nullptr;

  std::vector<QLabel *> m_labels;
  std::vector<QSpinBox *> m_multipliers;
  std::vector<QHBoxLayout *> m_rows;

  QHBoxLayout * m_p_okay_button_layout = nullptr;
  QPushButton * m_p_okay_button = nullptr;
  QVBoxLayout * m_p_vbox_layout = nullptr;

  QWidget * m_p_parent;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__ELEMENT_SETTINGS_HPP_
