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

#ifndef REEF_MOONSHINERS__UI__SETTINGS_WINDOW_HPP_
#define REEF_MOONSHINERS__UI__SETTINGS_WINDOW_HPP_

#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QWidget>

namespace reef_moonshiners::ui
{

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsWindow(QWidget * parent = nullptr);
  ~SettingsWindow() override = default;

  QLineEdit * get_tank_size_edit();

  QCheckBox * get_refugium_checkbox();

  QSpinBox * get_iodine_spinbox();
  QSpinBox * get_vanadium_spinbox();

private:
  QVBoxLayout * m_p_main_layout = nullptr;
  QHBoxLayout * m_p_refugium_layout = nullptr;
  QHBoxLayout * m_p_tank_size_layout = nullptr;
  QHBoxLayout * m_p_iodine_layout = nullptr;
  QHBoxLayout * m_p_vanadium_layout = nullptr;
  QLineEdit * m_p_tank_size_edit = nullptr;
  QSpinBox * m_p_iodine_drop_edit = nullptr;
  QSpinBox * m_p_vanadium_drop_edit = nullptr;
  QLabel * m_p_tank_size_label = nullptr;
  QLabel * m_p_refugium_label = nullptr;
  QLabel * m_p_iodine_label = nullptr;
  QLabel * m_p_vanadium_label = nullptr;
  QCheckBox * m_p_refugium_checkbox = nullptr;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__SETTINGS_WINDOW_HPP_
