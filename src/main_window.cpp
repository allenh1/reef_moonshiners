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
: QMainWindow(parent)
{
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(75));

  m_p_dose_label = new QLabel(tr("Dosing Summary"), this);
  m_p_import_action = new QAction(tr("Import ICP Data"), this);
  m_p_settings_action = new QAction(tr("Settings"), this);
  m_p_calendar_action = new QAction(tr("Calendar"), this);
  m_p_about_action = new QAction(tr("About"), this);

  m_p_calendar = new QCalendarWidget(this);
  m_p_central_widget = new QWidget(this);
  m_p_list_widget = new QWidget(m_p_central_widget);
  m_p_main_layout = new QVBoxLayout(m_p_central_widget);
  m_p_main_layout->addWidget(m_p_calendar);
  m_p_main_layout->addWidget(m_p_dose_label);

  m_p_list_layout = new QVBoxLayout(m_p_list_widget);
  m_p_main_layout->addWidget(m_p_list_widget);

  m_p_toolbar = new QToolBar(this);
  m_p_toolbar->addAction(m_p_import_action);
  m_p_toolbar->addAction(m_p_settings_action);
  m_p_toolbar->addAction(m_p_calendar_action);
  m_p_toolbar->addAction(m_p_about_action);

  this->addToolBar(m_p_toolbar);

  this->setCentralWidget(m_p_central_widget);
  this->setWindowTitle(tr("Reef Moonshiners"));

  this->_populate_list_layout();
}

void MainWindow::_fill_element_list()
{
  /* for now, these are all just hard-coded */
  auto manganese = std::make_unique<reef_moonshiners::Manganese>();
  m_elements.emplace(std::move(manganese), new ElementDisplay(manganese.get(), m_p_list_widget));
  auto chromium = std::make_unique<reef_moonshiners::Chromium>();
  m_elements.emplace(std::move(chromium), new ElementDisplay(chromium.get(), m_p_list_widget));
  auto selenium = std::make_unique<reef_moonshiners::Selenium>();
  m_elements.emplace(std::move(selenium), new ElementDisplay(selenium.get(), m_p_list_widget));
  auto cobalt = std::make_unique<reef_moonshiners::Cobalt>();
  m_elements.emplace(std::move(cobalt), new ElementDisplay(cobalt.get(), m_p_list_widget));
  auto iron = std::make_unique<reef_moonshiners::Iron>();
  m_elements.emplace(std::move(iron), new ElementDisplay(iron.get(), m_p_list_widget));
}

void MainWindow::_populate_list_layout()
{
  this->_fill_element_list();
  for (auto &[element, display] : m_elements) {
    m_p_list_layout->addWidget(display);
  }
}

}  // namespace reef_moonshiners::ui
