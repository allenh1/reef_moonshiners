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

#include <reef_moonshiners/ui/settings_window.hpp>

namespace reef_moonshiners::ui
{

SettingsWindow::SettingsWindow(QWidget * parent)
: QWidget(parent)
{
  m_p_tank_size_edit = new QLineEdit(tr("75.0"), this);
  m_p_tank_size_label = new QLabel(tr("Tank Size (gallons):"), this);
  m_p_refugium_label = new QLabel(tr("Refugium:"), this);
  m_p_iodine_label = new QLabel(tr("Iodide drops (2-3 per 100 gallons):"));
  m_p_refugium_checkbox = new QCheckBox(this);
  m_p_iodine_drop_edit = new QSpinBox();

  m_p_iodine_drop_edit->setValue(2);

  m_p_tank_size_layout = new QHBoxLayout();
  m_p_tank_size_layout->addWidget(m_p_tank_size_label);
  m_p_tank_size_layout->addWidget(m_p_tank_size_edit);

  m_p_iodine_layout = new QHBoxLayout();
  m_p_iodine_layout->addWidget(m_p_iodine_label);
  m_p_iodine_layout->addWidget(m_p_iodine_drop_edit);

  m_p_refugium_layout = new QHBoxLayout();
  m_p_refugium_layout->addWidget(m_p_refugium_label);
  m_p_refugium_layout->addWidget(m_p_refugium_checkbox);

  m_p_main_layout = new QVBoxLayout();
  m_p_main_layout->addLayout(m_p_tank_size_layout);
  m_p_main_layout->addLayout(m_p_iodine_layout);
  m_p_main_layout->addLayout(m_p_refugium_layout);

  this->setLayout(m_p_main_layout);
}

QLineEdit * SettingsWindow::get_tank_size_edit()
{
  return m_p_tank_size_edit;
}

QCheckBox * SettingsWindow::get_refugium_checkbox()
{
  return m_p_refugium_checkbox;
}

QSpinBox * SettingsWindow::get_iodine_spinbox()
{
  return m_p_iodine_drop_edit;
}
}  // namespace reef_moonshiners::ui