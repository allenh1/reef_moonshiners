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

#include <fstream>
#include <filesystem>
#include <iostream>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QXmlStreamReader>
#include <QJsonDocument>
#include <QJsonParseError>

namespace
{
namespace fs = std::filesystem;
}  // namespace

namespace reef_moonshiners::ui
{

MainWindow::MainWindow(QWidget * parent)
: QMainWindow(parent)
{
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(75));

  m_p_dose_label = new QLabel(tr("Dosing Summary"), this);
  m_p_dose_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_p_import_action = new QAction(tr("Import ICP Data"), this);
  m_p_settings_action = new QAction(tr("Settings"), this);
  m_p_calendar_action = new QAction(tr("Calendar"), this);
  m_p_about_action = new QAction(tr("About"), this);

  /* construct windows */
  m_p_settings_window = new SettingsWindow(this);
  m_p_icp_selection_window = new icp_import_dialog::IcpSelectionWindow(this);
  m_p_ati_entry_window = new icp_import_dialog::ATIEntryWindow(this);
  m_p_ati_correction_start_window = new icp_import_dialog::ATICorrectionStartWindow(this);

  m_p_calendar = new QCalendarWidget(this);
  m_p_calendar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_p_central_widget = new QWidget(this);
  // m_p_list_widget = new QWidget(m_p_central_widget);
  m_p_main_layout = new QVBoxLayout(m_p_central_widget);
  m_p_main_layout->addWidget(m_p_calendar);
  m_p_main_layout->addWidget(m_p_dose_label);

  m_p_list_layout = new QVBoxLayout();
  m_p_main_layout->addLayout(m_p_list_layout);

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
  QObject::connect(
    m_p_calendar, &QCalendarWidget::selectionChanged, this, &MainWindow::_refresh_elements);

  /* connections from settings window */
  QObject::connect(
    m_p_settings_window->get_refugium_checkbox(), &QCheckBox::stateChanged, this,
    &MainWindow::_update_refugium_state);
  QObject::connect(
    m_p_settings_window->get_tank_size_edit(), &QLineEdit::textChanged, this,
    &MainWindow::_update_tank_size);

  /* connections from ICP Dialog */
  QObject::connect(
    m_p_icp_selection_window, &icp_import_dialog::IcpSelectionWindow::next_button_pressed,
    this, &MainWindow::_handle_next_icp_selection_window);
  QObject::connect(
    m_p_ati_entry_window->get_back_button(), &QPushButton::clicked,
    this, &MainWindow::_handle_back_ati_entry_window);
  QObject::connect(
    m_p_ati_entry_window, &icp_import_dialog::ATIEntryWindow::next_button_pressed,
    this, &MainWindow::_handle_next_ati_entry_window);
  QObject::connect(
    m_p_ati_correction_start_window,
    &icp_import_dialog::ATICorrectionStartWindow::okay_button_pressed,
    this, &MainWindow::_handle_okay_ati_correction_start_window);
  QObject::connect(
    m_p_ati_correction_start_window->get_back_button(), &QPushButton::clicked,
    this, &MainWindow::_handle_back_ati_correction_start_window);

  this->addToolBar(m_p_toolbar);

  this->setCentralWidget(m_p_central_widget);
  this->setWindowTitle(tr("Reef Moonshiners"));
  this->setWindowIcon(QIcon(":/icon.png"));

  this->_fill_element_list();
  this->_populate_list_layout();
  this->_load();
  this->_refresh_elements();
}

void MainWindow::_fill_element_list()
{
  /* for now, these are all just hard-coded */
  auto manganese = std::make_unique<reef_moonshiners::Manganese>();
  m_elements.emplace(std::move(manganese), new ElementDisplay(manganese.get(), m_p_list_layout));
  auto chromium = std::make_unique<reef_moonshiners::Chromium>();
  m_elements.emplace(std::move(chromium), new ElementDisplay(chromium.get(), m_p_list_layout));
  auto selenium = std::make_unique<reef_moonshiners::Selenium>();
  m_elements.emplace(std::move(selenium), new ElementDisplay(selenium.get(), m_p_list_layout));
  auto cobalt = std::make_unique<reef_moonshiners::Cobalt>();
  m_elements.emplace(std::move(cobalt), new ElementDisplay(cobalt.get(), m_p_list_layout));
  auto iron = std::make_unique<reef_moonshiners::Iron>();
  m_elements.emplace(std::move(iron), new ElementDisplay(iron.get(), m_p_list_layout));

  /* corrections */
  auto fluorine = std::make_unique<reef_moonshiners::Fluorine>();
  m_correction_elements.emplace(
    std::move(fluorine),
    new ElementDisplay(fluorine.get(), m_p_list_layout));
  auto bromine = std::make_unique<reef_moonshiners::Bromine>();
  m_correction_elements.emplace(
    std::move(bromine),
    new ElementDisplay(bromine.get(), m_p_list_layout));
  auto nickel = std::make_unique<reef_moonshiners::Nickel>();
  m_correction_elements.emplace(
    std::move(nickel),
    new ElementDisplay(nickel.get(), m_p_list_layout));
  auto zinc = std::make_unique<reef_moonshiners::Zinc>();
  m_correction_elements.emplace(std::move(zinc), new ElementDisplay(zinc.get(), m_p_list_layout));
  auto barium = std::make_unique<reef_moonshiners::Barium>();
  m_correction_elements.emplace(
    std::move(barium),
    new ElementDisplay(barium.get(), m_p_list_layout));
  auto boron = std::make_unique<reef_moonshiners::Boron>();
  m_correction_elements.emplace(std::move(boron), new ElementDisplay(boron.get(), m_p_list_layout));
  auto strontium = std::make_unique<reef_moonshiners::Strontium>();
  m_correction_elements.emplace(
    std::move(strontium),
    new ElementDisplay(strontium.get(), m_p_list_layout));
  auto potassium = std::make_unique<reef_moonshiners::Potassium>();
  m_correction_elements.emplace(
    std::move(potassium),
    new ElementDisplay(potassium.get(), m_p_list_layout));
}

void MainWindow::_save()
{
  fs::path out{
    QStandardPaths::displayName(
      QStandardPaths::StandardLocation::AppDataLocation).toStdString()};
  fs::create_directory(out);  /* create if not exists */
  out = out / "reef_moonshiners.dat";
  // fprintf(stderr, "saving file '%s'\n", std::string(out).c_str());
  std::ofstream file{out, std::ios::binary};
  binary_out(file, reef_moonshiners::ElementBase::get_tank_size());
  binary_out(file, m_refugium_state);
  for (const auto & [daily, display] : m_elements) {
    (void)display;
    file << *daily;
  }
  for (const auto & [correction, display] : m_correction_elements) {
    (void)display;
    file << *correction;
  }
}

bool MainWindow::_load()
{
  fs::path dir{
    QStandardPaths::displayName(
      QStandardPaths::StandardLocation::AppDataLocation).toStdString()};
  fs::path in = dir / "reef_moonshiners.dat";
  if (!fs::exists(in)) {
    return false;  /* this will be created after we save */
  }
  std::ifstream file{in, std::ios::binary};
  double tank_size;
  binary_in(file, tank_size);
  reef_moonshiners::ElementBase::set_tank_size(tank_size);
  const double gallons = reef_moonshiners::liters_to_gallons(tank_size);
  m_p_settings_window->get_tank_size_edit()->setText(QString().setNum(gallons));
  binary_in(file, m_refugium_state);
  m_p_settings_window->get_refugium_checkbox()->setCheckState(Qt::CheckState(m_refugium_state));
  for (auto & [daily, display] : m_elements) {
    (void)display;
    file >> *daily;
  }
  for (auto & [correction, display] : m_correction_elements) {
    (void)display;
    file >> *correction;
  }
  return true;
}

void MainWindow::_refresh_elements()
{
  for (auto &[element, display] : m_correction_elements) {
    display->update_dosage(element.get(), m_p_calendar->selectedDate());
  }
  for (auto &[element, display] : m_elements) {
    display->update_dosage(element.get(), m_p_calendar->selectedDate());
  }
  m_p_list_layout->update();
  _save();
}

void MainWindow::_populate_list_layout()
{
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
  _refresh_elements();
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
  if (m_p_active_action != m_p_import_action) {
    m_p_active_action->setEnabled(true);
  }
  m_p_active_window = m_p_active_icp_selection_window;
  m_p_active_action = m_p_import_action;
}

void MainWindow::_update_refugium_state(int state)
{
  if (Qt::Checked == state) {
    for (const auto & [element, display] : m_elements) {
      element->set_multiplier(2.0);  /* this doubles the daily dose */
    }
  } else if (Qt::Unchecked == state) {
    for (const auto & [element, display] : m_elements) {
      element->set_multiplier(1.0);
    }
  }
  m_refugium_state = state;
  this->_refresh_elements();
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
  /* update elements */
  this->_refresh_elements();
}

void MainWindow::_handle_next_icp_selection_window(IcpSelection icp_selection)
{
  switch (icp_selection) {
    case IcpSelection::ATI_ICP_OES:
      m_p_active_icp_selection_window = m_p_ati_entry_window;
      this->_activate_icp_import_dialog();
      break;
    default:
      exit(1);
  }
}

void MainWindow::_handle_back_ati_entry_window()
{
  m_p_active_icp_selection_window = m_p_icp_selection_window;
  this->_activate_icp_import_dialog();
}

void MainWindow::_handle_next_ati_entry_window(const QString & text, const QDate & date)
{
  QString url{"https://lab.atiaquaristik.com/publicAnalysis/"};
  url += text;
  /* connect to the host */
  this->setDisabled(true);
  QNetworkAccessManager manager;
  QNetworkReply * response = manager.get(QNetworkRequest(QUrl(url)));
  QEventLoop event;
  connect(response, SIGNAL(finished()), &event, SLOT(quit()));
  event.exec();

  // Source should be stored here
  QString html{response->readAll()};
  /* find the data table */
  auto idx = html.indexOf("var dataTable");
  if (idx < 0) {
    fprintf(stderr, "data table not found\n");
    return;
  }
  /* truncate */
  html = html.sliced(idx);
  /* find start of relevant data */
  idx = html.indexOf("\"0\":");
  if (idx < 0) {
    fprintf(stderr, "zero section not found\n");
    return;
  }
  html = QString("{") + html.sliced(idx);
  /* find end of the table */
  idx = html.indexOf("tank:");
  /* truncate */
  html = html.sliced(0, idx);
  html = html.sliced(0, html.lastIndexOf(','));
  /* parse as JSON */
  QJsonParseError err{QJsonParseError::NoError};
  QJsonDocument parsed = QJsonDocument::fromJson(html.toUtf8(), &err);
  if (QJsonParseError::ParseError::NoError != err.error) {
    fprintf(stderr, "Error parsing JSON: '%s'\n", err.errorString().toStdString().c_str());
    fprintf(stderr, "text: '%s'\n", html.toStdString().c_str());
    return;
  }
  std::unordered_map<std::string, double> values;
  for (size_t x = 0; x < 43; ++x) {
    double scale = 1.0;
    if (parsed[QString().setNum(x)]["element"]["units_id"].toInt() == 2) {
      scale = 1E3;  /* mg / L -> ug / L */
    }
    values[parsed[QString().setNum(x)]["element"]["description_en"].toString().toStdString()] =
      parsed[QString().setNum(x)]["elements_value"].toDouble() * scale;
  }
  int year, month, day;
  date.getDate(&year, &month, &day);
  const std::chrono::year_month_day date_of_sample{
    std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
  for (auto &[element, display] : m_correction_elements) {
    /* set concentration */
    element->set_concentration(values[element->get_name()], date_of_sample);
  }
  m_p_active_icp_selection_window = m_p_ati_correction_start_window;
  this->setEnabled(true);
  this->_activate_icp_import_dialog();
}

void MainWindow::_handle_okay_ati_correction_start_window(const QDate & date)
{
  int year, month, day;
  date.getDate(&year, &month, &day);
  const std::chrono::year_month_day start_date{
    std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
  for (auto &[element, display] : m_correction_elements) {
    /* set concentration */
    (void)display;
    element->set_correction_start_date(start_date);
  }
  m_p_active_icp_selection_window = nullptr;
  this->_activate_calendar_window();
}

void MainWindow::_handle_back_ati_correction_start_window()
{
  m_p_active_icp_selection_window = m_p_ati_entry_window;
  this->_activate_icp_import_dialog();
}

}  // namespace reef_moonshiners::ui
