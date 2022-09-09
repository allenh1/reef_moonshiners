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

#include <reef_moonshiners/ui/icp_import_dialog/ati_entry_window.hpp>

namespace reef_moonshiners::ui::icp_import_dialog
{

ATIEntryWindow::ATIEntryWindow(QWidget * parent)
: QWidget(parent)
{
  m_p_analysis_id_label = new QLabel(tr("Analysis ID:"));
  m_p_next_button = new QPushButton(tr("&Next"));
  m_p_back_button = new QPushButton(tr("&Back"));
  m_p_ati_id_entry = new QLineEdit();

  m_p_ati_entry_layout = new QHBoxLayout();
  m_p_ati_entry_layout->addWidget(m_p_analysis_id_label);
  m_p_ati_entry_layout->addWidget(m_p_ati_id_entry);

  m_p_button_layout = new QHBoxLayout();
  m_p_button_layout->addWidget(m_p_back_button);
  m_p_button_layout->addWidget(m_p_next_button);

  m_p_calendar_widget = new QCalendarWidget();
  m_p_collection_label = new QLabel(tr("Select Collection Date:"));

  m_p_main_layout = new QVBoxLayout(this);
  m_p_main_layout->addLayout(m_p_ati_entry_layout);
  m_p_main_layout->addWidget(m_p_collection_label);
  m_p_main_layout->addWidget(m_p_calendar_widget);
  m_p_main_layout->addLayout(m_p_button_layout);

  QObject::connect(
    m_p_next_button, &QPushButton::clicked,
    [this]() {
      Q_EMIT (next_button_pressed(m_p_ati_id_entry->text(), m_p_calendar_widget->selectedDate()));
    });
}

QPushButton * ATIEntryWindow::get_back_button() const
{
  return m_p_back_button;
}

}  // namespace reef_moonshiners::ui::icp_import_dialog
