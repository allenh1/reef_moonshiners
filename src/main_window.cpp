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
  m_p_about_window = new AboutWindow(this);

  m_p_calendar = new QCalendarWidget(this);
  m_p_calendar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_p_central_widget = new QWidget(this);
  m_p_main_layout = new QVBoxLayout(m_p_central_widget);
  m_p_main_layout->addWidget(m_p_calendar);
  m_p_main_layout->addWidget(m_p_dose_label);

  m_p_list_widget = new QListWidget();
  m_p_main_layout->addWidget(m_p_list_widget);

  m_p_toolbar = new QToolBar(this);
  m_p_toolbar->addAction(m_p_import_action);
  m_p_toolbar->addAction(m_p_settings_action);
  m_p_toolbar->addAction(m_p_calendar_action);
  m_p_toolbar->addAction(m_p_about_action);

  m_p_calendar_action->setDisabled(true);

  m_p_active_window = m_p_central_widget;
  m_p_active_action = m_p_calendar_action;

  /* list connections */
  QObject::connect(
    m_p_list_widget, &QListWidget::itemClicked,
    this, &MainWindow::_handle_item_clicked);

  /* action mappings */
  QObject::connect(
    m_p_settings_action, &QAction::triggered, this, &MainWindow::_activate_settings_window);
  QObject::connect(
    m_p_calendar_action, &QAction::triggered, this, &MainWindow::_activate_calendar_window);
  QObject::connect(
    m_p_import_action, &QAction::triggered, this, &MainWindow::_activate_icp_import_dialog);
  QObject::connect(
    m_p_about_action, &QAction::triggered, this, &MainWindow::_activate_about_window);
  QObject::connect(
    m_p_calendar, &QCalendarWidget::selectionChanged, this, &MainWindow::_refresh_elements);

  /* connections from settings window */
  QObject::connect(
    m_p_settings_window->get_refugium_checkbox(), &QCheckBox::stateChanged, this,
    &MainWindow::_update_refugium_state);
  QObject::connect(
    m_p_settings_window->get_tank_size_edit(), &QLineEdit::textChanged, this,
    &MainWindow::_update_tank_size);
  QObject::connect(
    m_p_settings_window->get_iodine_spinbox(), &QSpinBox::valueChanged,
    this, &MainWindow::_update_iodine_drops);
  QObject::connect(
    m_p_settings_window->get_vanadium_spinbox(), &QSpinBox::valueChanged,
    this, &MainWindow::_update_vanadium_drops);
  QObject::connect(
    m_p_settings_window, &SettingsWindow::rubidium_selection_changed,
    this, &MainWindow::_update_rubidium_selection);
  QObject::connect(
    m_p_settings_window->get_rubidium_start_dateedit(), &QDateEdit::dateChanged,
    this, &MainWindow::_update_rubidium_start_date);
  QObject::connect(
    m_p_settings_window->get_okay_button(), &QPushButton::clicked,
    this, &MainWindow::_activate_calendar_window);
  QObject::connect(
    m_p_settings_window->get_nano_dose_checkbox(), &QCheckBox::stateChanged,
    this, &MainWindow::_update_nano_dose_state);

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
  QObject::connect(
    m_p_ati_correction_start_window, &icp_import_dialog::ATICorrectionStartWindow::increase_iodine,
    this, &MainWindow::_handle_increase_iodine);
  QObject::connect(
    m_p_ati_correction_start_window, &icp_import_dialog::ATICorrectionStartWindow::decrease_iodine,
    this, &MainWindow::_handle_decrease_iodine);
  QObject::connect(
    m_p_ati_correction_start_window,
    &icp_import_dialog::ATICorrectionStartWindow::increase_vanadium,
    this, &MainWindow::_handle_increase_vanadium);
  QObject::connect(
    m_p_ati_correction_start_window,
    &icp_import_dialog::ATICorrectionStartWindow::decrease_vanadium,
    this, &MainWindow::_handle_decrease_vanadium);

  this->addToolBar(m_p_toolbar);

  this->setCentralWidget(m_p_central_widget);
  this->setWindowTitle(tr("Reef Moonshiners"));
  this->setWindowIcon(QIcon(":/icon.png"));

  this->_fill_element_list();
  this->_load();
  this->_refresh_elements();
}

void MainWindow::_fill_element_list()
{
  /* daily elements */

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

  /* rubidium */

  m_p_rubidium_element = std::make_unique<reef_moonshiners::Rubidium>();
  m_p_rubidium_display = new ElementDisplay(m_p_rubidium_element.get(), m_p_list_widget);

  /* dropper elements */

  auto iodine = std::make_unique<reef_moonshiners::Iodine>();
  m_p_iodine_element = iodine.get();
  m_dropper_elements.emplace(std::move(iodine), new ElementDisplay(iodine.get(), m_p_list_widget));
  auto vanadium = std::make_unique<reef_moonshiners::Vanadium>();
  m_p_vanadium_element = vanadium.get();
  m_dropper_elements.emplace(
    std::move(vanadium), new ElementDisplay(
      vanadium.get(), m_p_list_widget));

  /* corrections */
  auto fluorine = std::make_unique<reef_moonshiners::Fluorine>();
  m_correction_elements.emplace(
    std::move(fluorine),
    new ElementDisplay(fluorine.get(), m_p_list_widget));
  auto bromine = std::make_unique<reef_moonshiners::Bromine>();
  m_correction_elements.emplace(
    std::move(bromine),
    new ElementDisplay(bromine.get(), m_p_list_widget));
  auto nickel = std::make_unique<reef_moonshiners::Nickel>();
  m_correction_elements.emplace(
    std::move(nickel),
    new ElementDisplay(nickel.get(), m_p_list_widget));
  auto zinc = std::make_unique<reef_moonshiners::Zinc>();
  m_correction_elements.emplace(std::move(zinc), new ElementDisplay(zinc.get(), m_p_list_widget));
  auto barium = std::make_unique<reef_moonshiners::Barium>();
  m_correction_elements.emplace(
    std::move(barium),
    new ElementDisplay(barium.get(), m_p_list_widget));
  auto boron = std::make_unique<reef_moonshiners::Boron>();
  m_correction_elements.emplace(std::move(boron), new ElementDisplay(boron.get(), m_p_list_widget));
  auto strontium = std::make_unique<reef_moonshiners::Strontium>();
  m_correction_elements.emplace(
    std::move(strontium),
    new ElementDisplay(strontium.get(), m_p_list_widget));
  auto potassium = std::make_unique<reef_moonshiners::Potassium>();
  m_correction_elements.emplace(
    std::move(potassium),
    new ElementDisplay(potassium.get(), m_p_list_widget));
}

void MainWindow::_handle_item_clicked(QListWidgetItem * p_item)
{
  if (nullptr == p_item) {
    return;
  }

  if (p_item->data(Qt::CheckStateRole) != Qt::Checked) {
    dynamic_cast<ElementDisplay *>(p_item)->set_checked(true);
  } else {
    dynamic_cast<ElementDisplay *>(p_item)->set_checked(false);
  }
}

void MainWindow::_save()
{
  fs::path out{
    QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString()};
  fs::create_directories(out);  /* create if not exists */
  out = out / "reef_moonshiners.dat";
  std::ofstream file{out, std::ios::binary};
  binary_out(file, m_save_file_version);
  binary_out(file, reef_moonshiners::ElementBase::get_tank_size());
  binary_out(file, m_refugium_state);
  binary_out(file, m_nano_dose_state);
  for (const auto & [daily, display] : m_elements) {
    (void)display;
    file << *daily;
  }
  for (const auto & [daily, display] : m_dropper_elements) {
    (void)display;
    file << *daily;
  }
  file << *m_p_rubidium_element;
  for (const auto & [correction, display] : m_correction_elements) {
    (void)display;
    file << *correction;
  }
}

bool MainWindow::_load()
{
  fs::path dir{QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString()};
  fs::path in = dir / "reef_moonshiners.dat";
  if (!fs::exists(in)) {
    m_p_vanadium_element->set_drops(1);
    m_p_iodine_element->set_drops(2);
    return false;  /* this will be created after we save */
  }
  std::ifstream file{in, std::ios::binary};
  /* read save_file_version */
  size_t save_file_version = 0;
  binary_in(file, save_file_version);
  if (save_file_version > m_save_file_version) {
    /* version zero assumed */
    save_file_version = 0;
    /* rewind to beginning of file */
    file.seekg(0, file.beg);
  }
  reef_moonshiners::ElementBase::set_load_version(save_file_version);
  double tank_size;
  binary_in(file, tank_size);
  reef_moonshiners::ElementBase::set_tank_size(tank_size);
  const double gallons = reef_moonshiners::liters_to_gallons(tank_size);
  m_p_settings_window->get_tank_size_edit()->setText(QString().setNum(gallons));
  binary_in(file, m_refugium_state);
  m_p_settings_window->get_refugium_checkbox()->setCheckState(Qt::CheckState(m_refugium_state));
  if (save_file_version >= 3) {
    binary_in(file, m_nano_dose_state);
  }
  m_p_settings_window->get_nano_dose_checkbox()->setCheckState(Qt::CheckState(m_nano_dose_state));
  for (auto & [daily, display] : m_elements) {
    (void)display;
    file >> *daily;
  }
  /* load dropper elements */
  if (save_file_version >= 1) {
    for (auto & [daily, display] : m_dropper_elements) {
      (void)display;
      file >> *daily;
    }
  } else {
    m_p_vanadium_element->set_drops(1);
    m_p_iodine_element->set_drops(2);
  }
  /* load rubidium */
  if (save_file_version >= 2) {
    file >> *m_p_rubidium_element;
  }
  m_p_settings_window->get_iodine_spinbox()->setValue(
    (int)m_p_iodine_element->get_dose(std::chrono::year_month_day{}));
  m_p_settings_window->get_vanadium_spinbox()->setValue(
    (int)m_p_vanadium_element->get_dose(std::chrono::year_month_day{}));
  const auto date = m_p_rubidium_element->get_initial_dose_date();
  m_p_settings_window->get_rubidium_start_dateedit()->setDate(
    QDate((int)date.year(), (unsigned)date.month(), (unsigned)date.day()));
  m_p_settings_window->get_rubidium_combobox()->setCurrentIndex(
    (uint8_t)m_p_rubidium_element->get_dosing_frequency());
  for (auto & [correction, display] : m_correction_elements) {
    (void)display;
    file >> *correction;
  }
  return true;
}

void MainWindow::_refresh_elements()
{
  for (auto &[element, display] : m_correction_elements) {
    display->update_dosage(m_p_calendar->selectedDate());
  }
  for (auto &[element, display] : m_elements) {
    display->update_dosage(m_p_calendar->selectedDate());
  }
  for (auto &[element, display] : m_dropper_elements) {
    display->update_dosage(m_p_calendar->selectedDate());
  }
  m_p_rubidium_display->update_dosage(m_p_calendar->selectedDate());
  _save();
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

void MainWindow::_activate_about_window()
{
  /* change view to about window */
  m_p_active_window = this->takeCentralWidget();
  this->setCentralWidget(m_p_about_window);
  /* grey out settings action */
  m_p_about_action->setDisabled(true);
  /* un-grey out the calendar widget */
  m_p_active_action->setEnabled(true);
  m_p_active_action = m_p_about_action;
  m_p_active_window = m_p_about_window;
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

void MainWindow::_update_nano_dose_state(int state)
{
  if (Qt::Checked == state) {
    for (const auto & [element, display] : m_elements) {
      element->set_use_nano_dose(true);  /* this doubles the daily dose */
    }
  } else if (Qt::Unchecked == state) {
    for (const auto & [element, display] : m_elements) {
      element->set_use_nano_dose(false);
    }
  }
  m_nano_dose_state = state;
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

void MainWindow::_update_iodine_drops(int drops)
{
  m_p_iodine_element->set_drops(drops);
  this->_refresh_elements();
}

void MainWindow::_update_vanadium_drops(int drops)
{
  m_p_vanadium_element->set_drops(drops);
  this->_refresh_elements();
}

void MainWindow::_update_rubidium_selection(reef_moonshiners::RubidiumSelection rubidium_selection)
{
  m_p_rubidium_element->set_dosing_frequency(rubidium_selection);
}

void MainWindow::_update_rubidium_start_date(QDate rubidium_start_date)
{
  int year, month, day;
  rubidium_start_date.getDate(&year, &month, &day);
  const std::chrono::year_month_day date{
    std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
  m_p_rubidium_element->set_initial_dose_date(date);
}

void MainWindow::_handle_next_icp_selection_window(
  reef_moonshiners::ui::icp_import_dialog::IcpSelection icp_selection)
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
  manager.setTransferTimeout(10E3);  /* ten second timeout */
  QNetworkReply * response = manager.get(QNetworkRequest(QUrl(url)));
  QEventLoop event;
  connect(response, SIGNAL(finished()), &event, SLOT(quit()));
  event.exec();
  if (QNetworkReply::NetworkError::NoError != response->error()) {
    /* error in transfer */
    this->setEnabled(true);
    m_p_ati_entry_window->show_input_error_message();
    return;
  }
  m_p_ati_entry_window->hide_input_error_message();
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
  for (auto &[element, display] : m_dropper_elements) {
    /* set concentration */
    element->set_concentration(values[element->get_name()], date_of_sample);
  }
  for (auto &[element, display] : m_elements) {
    /* set concentration */
    element->set_concentration(values[element->get_name()], date_of_sample);
  }
  /* handle iodine */
  m_p_active_icp_selection_window = m_p_ati_correction_start_window;
  m_p_ati_correction_start_window->set_iodine_increase(m_p_iodine_element->is_low());
  m_p_ati_correction_start_window->set_iodine_decrease(m_p_iodine_element->is_high());
  m_p_ati_correction_start_window->set_vanadium_increase(m_p_vanadium_element->is_low());
  m_p_ati_correction_start_window->set_vanadium_decrease(m_p_vanadium_element->is_high());
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

void MainWindow::_handle_increase_iodine()
{
  m_p_iodine_element->set_drops(m_p_iodine_element->get_dose(std::chrono::year_month_day{}) + 1);
  m_p_settings_window->get_iodine_spinbox()->setValue(
    m_p_iodine_element->get_dose(
      std::chrono::
      year_month_day{}));
  this->_refresh_elements();
}

void MainWindow::_handle_decrease_iodine()
{
  m_p_iodine_element->set_drops(m_p_iodine_element->get_dose(std::chrono::year_month_day{}) - 1);
  m_p_settings_window->get_iodine_spinbox()->setValue(
    m_p_iodine_element->get_dose(
      std::chrono::
      year_month_day{}));
  this->_refresh_elements();
}

void MainWindow::_handle_increase_vanadium()
{
  m_p_vanadium_element->set_drops(
    m_p_vanadium_element->get_dose(std::chrono::year_month_day{}) +
    1);
  m_p_settings_window->get_vanadium_spinbox()->setValue(
    m_p_vanadium_element->get_dose(std::chrono::year_month_day{}));
  this->_refresh_elements();
}

void MainWindow::_handle_decrease_vanadium()
{
  m_p_vanadium_element->set_drops(
    m_p_vanadium_element->get_dose(std::chrono::year_month_day{}) -
    1);
  m_p_settings_window->get_vanadium_spinbox()->setValue(
    m_p_vanadium_element->get_dose(std::chrono::year_month_day{}));
  this->_refresh_elements();
}

}  // namespace reef_moonshiners::ui
