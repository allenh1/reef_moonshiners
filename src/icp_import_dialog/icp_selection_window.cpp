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

#include <reef_moonshiners/ui/icp_import_dialog/icp_selection_window.hpp>

namespace reef_moonshiners::ui::icp_import_dialog
{

IcpSelectionWindow::IcpSelectionWindow(QWidget * parent)
: QWidget(parent)
{
  m_p_combo_box = new QComboBox();
  m_p_next_button = new QPushButton("&Next");
  // m_p_button_spacer = new QSpacerItem();
  m_p_selection_label = new QLabel("ICP Test:");

  m_p_selection_layout = new QHBoxLayout();
  m_p_selection_layout->addWidget(m_p_selection_label);
  m_p_selection_layout->addWidget(m_p_combo_box);

  m_p_button_layout = new QHBoxLayout();
  // m_p_button_layout->addWidget(m_p_button_spacer);
  m_p_button_layout->addWidget(m_p_next_button);

  m_p_main_layout = new QVBoxLayout(this);
  m_p_main_layout->addLayout(m_p_selection_layout);
  m_p_main_layout->addLayout(m_p_button_layout);

  /* Add ATI ICP as an option, more to come :) */
  m_p_combo_box->insertItem(0, "ATI ICP-OES");

  this->setLayout(m_p_main_layout);

  QObject::connect(
    m_p_next_button, &QPushButton::clicked,
    [this]() {
      Q_EMIT (next_button_pressed(
        IcpSelection{static_cast<uint8_t>(m_p_combo_box->currentIndex())}));
    });
}

}  // namespace reef_moonshiners::ui::icp_import_dialog
