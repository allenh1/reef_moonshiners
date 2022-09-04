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

#include <reef_moonshiners/ui/main_window.hpp>

namespace reef_moonshiners::ui
{

MainWindow::MainWindow(QWidget * parent)
: QMainWindow(parent),
  m_dose_label(tr("Dosing Summary")),
  m_import_action(tr("Import ICP Data")),
  m_settings_action(tr("Settings")),
  m_calendar_action(tr("Calendar")),
  m_about_action(tr("About"))
{
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(75));
  this->_populate_list_layout();

  m_main_layout.addWidget(&m_calendar);
  m_main_layout.addWidget(&m_dose_label);
  m_main_layout.addLayout(&m_list_layout);

  m_toolbar.addAction(&m_import_action);
  m_toolbar.addAction(&m_settings_action);
  m_toolbar.addAction(&m_calendar_action);
  m_toolbar.addAction(&m_about_action);

  this->addToolBar(&m_toolbar);

  m_central_widget.setLayout(&m_main_layout);
  this->setCentralWidget(&m_central_widget);
  this->setWindowTitle(tr("Reef Moonshiners"));
}

void MainWindow::_fill_element_list()
{
  /* for now, these are all just hard-coded */
  m_elements.emplace_back(std::make_unique<reef_moonshiners::Manganese>());
  m_elements.emplace_back(std::make_unique<reef_moonshiners::Chromium>());
  m_elements.emplace_back(std::make_unique<reef_moonshiners::Selenium>());
  m_elements.emplace_back(std::make_unique<reef_moonshiners::Cobalt>());
  m_elements.emplace_back(std::make_unique<reef_moonshiners::Iron>());
}

void MainWindow::_populate_list_layout()
{
  const std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  this->_fill_element_list();
  for (const auto & element : m_elements) {
    element->set_concentration(0.0, now);
    QHBoxLayout * layout = new QHBoxLayout();
    QCheckBox * check = new QCheckBox();
    QSizePolicy current = check->sizePolicy();
    current.setHorizontalStretch(0);
    check->setSizePolicy(current);
    layout->addWidget(check);
    layout->addWidget(new QLabel(tr(element->get_name().c_str())));
    layout->addWidget(new QLabel(QString().setNum((double)element->get_dose(now)) + " mL"));
    m_list_layout.addLayout(layout);
  }
}

}  // namespace reef_moonshiners::ui
