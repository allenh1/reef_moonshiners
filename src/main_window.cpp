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

  /* construct windows */
  m_p_settings_window = new SettingsWindow(this);
  m_p_icp_selection_window = new icp_import_dialog::IcpSelectionWindow(this);

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

  m_p_calendar_action->setDisabled(true);

  m_p_active_window = m_p_central_widget;
  m_p_active_action = m_p_calendar_action;

  /* action mappings */
  QObject::connect(
    m_p_settings_action, &QAction::triggered, this, &MainWindow::_activate_settings_window);
  QObject::connect(
    m_p_calendar_action, &QAction::triggered, this, &MainWindow::_activate_calendar_window);
  QObject::connect(
    m_p_import_action, &QAction::triggered, this, &MainWindow::_activate_icp_import_dialog);

  /* connections from settings window */
  QObject::connect(
    m_p_settings_window->get_refugium_checkbox(), &QCheckBox::stateChanged, this,
    &MainWindow::_update_refugium_state);
  QObject::connect(
    m_p_settings_window->get_tank_size_edit(), &QLineEdit::textChanged, this,
    &MainWindow::_update_tank_size);

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

void MainWindow::_activate_settings_window()
{
  /* change view to settings window */
  m_p_active_window = this->takeCentralWidget();
  this->setCentralWidget(m_p_settings_window);
  /* grey out settings action */
  m_p_settings_action->setDisabled(true);
  /* un-grey out the calendar widget */
  m_p_active_action->setEnabled(true);
  m_p_active_action = m_p_settings_action;
  m_p_active_window = m_p_settings_window;
}

void MainWindow::_activate_calendar_window()
{
  /* change view to settings window */
  m_p_active_window = this->takeCentralWidget();
  this->setCentralWidget(m_p_central_widget);
  /* grey out calendar action */
  m_p_calendar_action->setDisabled(true);
  /* un-grey out the settings widget */
  m_p_active_action->setEnabled(true);
  m_p_active_window = m_p_central_widget;
  m_p_active_action = m_p_calendar_action;
}

void MainWindow::_activate_icp_import_dialog()
{
  /* change view to ICP window */
  m_p_active_window = this->takeCentralWidget();
  if (nullptr == m_p_active_icp_selection_window) {
    m_p_active_icp_selection_window = m_p_icp_selection_window;
  }
  this->setCentralWidget(m_p_active_icp_selection_window);
  /* grey out icp action action */
  m_p_import_action->setDisabled(true);
  /* un-grey out the settings widget */
  m_p_active_action->setEnabled(true);
  m_p_active_window = m_p_active_icp_selection_window;
  m_p_active_action = m_p_import_action;
}

void MainWindow::_update_refugium_state(int state)
{
  if (Qt::Checked == state) {
    for (const auto & [element, display] : m_elements) {
      element->set_multiplier(2.0);  /* this doubles the daily dose */
      display->update_dosage(element.get());
    }
  } else if (Qt::Unchecked == state) {
    for (const auto & [element, display] : m_elements) {
      element->set_multiplier(1.0);
      display->update_dosage(element.get());
    }
  }
}

void MainWindow::_update_tank_size(const QString & text)
{
  bool ok = false;
  const double tank_size_gallons = text.toDouble(&ok);
  if (!ok) {
    /* TODO(allenh1): Add error message popup */
    return;
  }
  reef_moonshiners::ElementBase::set_tank_size(
    reef_moonshiners::gallons_to_liters(tank_size_gallons));
  /* update dailies */
  for (const auto & [element, display] : m_elements) {
    display->update_dosage(element.get());
  }
}

}  // namespace reef_moonshiners::ui
