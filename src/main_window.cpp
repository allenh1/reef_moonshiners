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
  m_p_element_settings = new ElementSettings(&m_element_container, this);
  m_p_icp_selection_window = new icp_import_dialog::IcpSelectionWindow(this);
  m_p_ati_entry_window = new icp_import_dialog::ATIEntryWindow(this);
  m_p_ati_correction_start_window = new icp_import_dialog::ATICorrectionStartWindow(this);
  m_p_oceamo_ms_entry_window = new icp_import_dialog::OceamoMSEntryWindow(this);
  m_p_about_window = new AboutWindow(this);
  m_p_oceamo_file_browser = new QFileDialog(this);

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
    m_p_settings_window->get_element_settings_button(), &QPushButton::clicked,
    this, &MainWindow::_activate_element_settings);
  QObject::connect(
    m_p_settings_window->get_nano_dose_checkbox(), &QCheckBox::stateChanged,
    this, &MainWindow::_update_nano_dose_state);

  /* connections from element settings window */
  QObject::connect(
   m_p_element_settings->get_okay_button(), &QPushButton::clicked,
   this, &MainWindow::_activate_settings_window);

  /* connections from ICP Dialog */
  QObject::connect(
    m_p_icp_selection_window, &icp_import_dialog::IcpSelectionWindow::next_button_pressed,
    this, &MainWindow::_handle_next_icp_selection_window);
  QObject::connect(
    m_p_ati_entry_window->get_back_button(), &QPushButton::clicked,
    this, &MainWindow::_handle_back_ati_entry_window);
  QObject::connect(
    m_p_oceamo_ms_entry_window->get_back_button(), &QPushButton::clicked,
    this, &MainWindow::_handle_back_oceamo_ms_entry_window);
  QObject::connect(
    m_p_ati_entry_window, &icp_import_dialog::ATIEntryWindow::next_button_pressed,
    this, &MainWindow::_handle_next_ati_entry_window);
  QObject::connect(
    m_p_oceamo_ms_entry_window, &icp_import_dialog::OceamoMSEntryWindow::next_button_pressed,
    this, &MainWindow::_handle_next_oceamo_ms_entry_window);
  QObject::connect(
    m_p_oceamo_file_browser, &QFileDialog::fileSelected,
    this, &MainWindow::_handle_oceamo_ms_analysis_selected);
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

  m_elements.emplace(
    new ElementDisplay(m_element_container["Manganese"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Chromium"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Selenium"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Cobalt"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Iron"], m_p_list_widget));

  /* rubidium */

  m_elements.emplace(
    new ElementDisplay(m_element_container["Rubidium"], m_p_list_widget));

  /* dropper elements */

  m_elements.emplace(new ElementDisplay(m_element_container["Iodine"], m_p_list_widget));
  m_elements.emplace(new ElementDisplay(m_element_container["Vanadium"], m_p_list_widget));

  /* corrections */
  m_elements.emplace(
    new ElementDisplay(m_element_container["Fluorine"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Bromine"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Nickel"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Zinc"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Boron"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Barium"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Potassium"], m_p_list_widget));
  m_elements.emplace(
    new ElementDisplay(m_element_container["Strontium"], m_p_list_widget));
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
  binary_out(file, 0);  /* previously refugium state */
  binary_out(file, m_nano_dose_state);
  file << m_element_container;
}

bool MainWindow::_load()
{
  fs::path dir{QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString()};
  fs::path in = dir / "reef_moonshiners.dat";
  if (!fs::exists(in)) {
    m_element_container.vanadium()->set_drops(1);
    m_element_container.iodine()->set_drops(2);
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
  int refugium_state;
  binary_in(file, refugium_state);  /* not used */
  if (save_file_version < 5) {
    /* TODO(allenh1): report as unsupported, warn for data loss */
    return false;
  }
  binary_in(file, m_nano_dose_state);
  m_p_settings_window->get_nano_dose_checkbox()->setCheckState(Qt::CheckState(m_nano_dose_state));
  file >> m_element_container;

  m_p_settings_window->get_iodine_spinbox()->setValue(
    (int)m_element_container.iodine()->get_dose(std::chrono::year_month_day{}));
  m_p_settings_window->get_vanadium_spinbox()->setValue(
    (int)m_element_container.vanadium()->get_dose(std::chrono::year_month_day{}));
  const auto date =
    m_element_container.rubidium()->get_initial_dose_date();
  m_p_settings_window->get_rubidium_start_dateedit()->setDate(
    QDate((int)date.year(), (unsigned)date.month(), (unsigned)date.day()));
  m_p_settings_window->get_rubidium_combobox()->setCurrentIndex(
    (uint8_t)m_element_container.rubidium()->get_dosing_frequency());
  return true;
}

void MainWindow::_refresh_elements()
{
  for (auto & display : m_elements) {
    display->update_dosage(m_p_calendar->selectedDate());
  }
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

void MainWindow::_activate_element_settings()
{
  /* change view to element settings window */
  m_p_active_window = this->takeCentralWidget();
  m_p_element_settings->refresh_values();
  this->setCentralWidget(m_p_element_settings);
  /* grey out settings action */
  m_p_settings_action->setDisabled(true);
  /* un-grey out the calendar widget */
  m_p_active_action->setEnabled(true);
  m_p_active_action = m_p_settings_action;
  m_p_active_window = m_p_element_settings;
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

void MainWindow::_activate_oceamo_file_browser()
{
  /* change view to file dialog */
  m_p_active_window = this->takeCentralWidget();
  this->setCentralWidget(m_p_oceamo_file_browser);
  m_p_active_window = m_p_oceamo_file_browser;
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

void MainWindow::_update_nano_dose_state(int state)
{
  if (Qt::Checked == state) {
    for (auto & element : m_element_container.get_dailies()) {
      element->set_use_nano_dose(true);  /* this doubles the daily dose */
    }
  } else if (Qt::Unchecked == state) {
    for (auto & element : m_element_container.get_dailies()) {
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
  m_element_container.iodine()->set_drops(drops);
  this->_refresh_elements();
}

void MainWindow::_update_vanadium_drops(int drops)
{
  m_element_container.vanadium()->set_drops(drops);
  this->_refresh_elements();
}

void MainWindow::_update_rubidium_selection(reef_moonshiners::RubidiumSelection rubidium_selection)
{
  m_element_container.rubidium()->set_dosing_frequency(rubidium_selection);
}

void MainWindow::_update_rubidium_start_date(QDate rubidium_start_date)
{
  int year, month, day;
  rubidium_start_date.getDate(&year, &month, &day);
  const std::chrono::year_month_day date{
    std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
  m_element_container.rubidium()->set_initial_dose_date(date);
}

void MainWindow::_handle_next_icp_selection_window(
  reef_moonshiners::ui::icp_import_dialog::IcpSelection icp_selection)
{
  switch (icp_selection) {
    case IcpSelection::ATI_ICP_OES:
      m_p_active_icp_selection_window = m_p_ati_entry_window;
      this->_activate_icp_import_dialog();
      break;
    case IcpSelection::OCEAMO_ICP_MS:
      m_p_active_icp_selection_window = m_p_oceamo_ms_entry_window;
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

void MainWindow::_handle_back_oceamo_ms_entry_window()
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
  for (auto & element : m_element_container.get_corrections()) {
    /* set concentration */
    element->set_concentration(values[element->get_name()], date_of_sample);
  }
  for (auto & element : m_element_container.get_dailies()) {
    /* set concentration */
    element->set_use_ms_mode(false);
    element->set_concentration(values[element->get_name()], date_of_sample);
  }
  /* handle iodine */
  m_p_active_icp_selection_window = m_p_ati_correction_start_window;
  m_p_ati_correction_start_window->set_iodine_increase(m_element_container.iodine()->is_low());
  m_p_ati_correction_start_window->set_iodine_decrease(m_element_container.iodine()->is_high());
  m_p_ati_correction_start_window->set_vanadium_increase(m_element_container.vanadium()->is_low());
  m_p_ati_correction_start_window->set_vanadium_decrease(m_element_container.vanadium()->is_high());
  this->setEnabled(true);
  this->_activate_icp_import_dialog();
}

void MainWindow::_handle_okay_ati_correction_start_window(const QDate & date)
{
  int year, month, day;
  date.getDate(&year, &month, &day);
  const std::chrono::year_month_day start_date{
    std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
  for (auto & element : m_element_container.get_corrections()) {
    /* set concentration */
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

void MainWindow::_handle_next_oceamo_ms_entry_window(const QDate & date)
{
  m_oceamo_sample_date = date;
  this->_activate_oceamo_file_browser();
}

void MainWindow::_handle_oceamo_ms_analysis_selected(const QString & file)
{
  this->setDisabled(true);
  QPdfDocument ms_results;
  QPdfDocument::Error ret = ms_results.load(file);
  if (QPdfDocument::Error::None != ret) {
    /* error loading PDF document */
    this->setEnabled(true);
    m_p_oceamo_ms_entry_window->show_pdf_load_error_message(tr("failed to load pdf"), ret);
    return;
  } else if (ms_results.pageCount() != 3) {
    this->setEnabled(true);
    m_p_oceamo_ms_entry_window->show_pdf_load_error_message(tr("Unexpected page count"));
    return;
  }
  QString page_1 = ms_results.getAllText(0).text();
  QString page_2 = ms_results.getAllText(1).text();
  QRegularExpression re_ug_l{"(\\w+) (\\d+,?\\d*).*µg/l"};
  QRegularExpression re_mg_l{"(\\w+) (\\d+,?\\d*).*mg/l"};
  QRegularExpression re_date_sample{"Date of Sampling: (\\d+)\.(\\d+)\.(\\d+)"};
  int day = -1;
  int month;
  int year;
  std::unordered_map<std::string, double> element_concentration_map;
  for (const auto & line : page_1.split('\n')) {
    if (auto match = re_ug_l.match(line); match.hasMatch()) {
      QString element = match.captured(1);
      double concentration = match.captured(2).replace(',', '.').toDouble();
      element_concentration_map[element.toStdString()] = concentration;
    } else if (auto match = re_mg_l.match(line); match.hasMatch()) {
      QString element = match.captured(1);
      double concentration = match.captured(2).replace(',', '.').toDouble();
      element_concentration_map[element.toStdString()] = concentration * 1E3;
    } else if (auto match = re_date_sample.match(line); match.hasMatch()) {
      day = match.captured(1).toInt();
      month = match.captured(2).toInt();
      year = match.captured(3).toInt();
    }
  }
  for (const auto & line : page_2.split('\n')) {
    if (auto match = re_ug_l.match(line); match.hasMatch()) {
      QString element = match.captured(1);
      double concentration = match.captured(2).replace(',', '.').toDouble();
      element_concentration_map[element.toStdString()] = concentration;
    } else if (auto match = re_mg_l.match(line); match.hasMatch()) {
      QString element = match.captured(1);
      double concentration = match.captured(2).replace(',', '.').toDouble();
      element_concentration_map[element.toStdString()] = concentration * 1E3;
    }
  }
  /* Bromide / Fluoride is a special case for Oceamo, where others say bromine */
  element_concentration_map["Bromine"] = element_concentration_map["Bromide"];
  element_concentration_map["Fluorine"] = element_concentration_map["Fluoride"];
  const std::chrono::year_month_day date_of_sample{
    std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
  for (auto & element : m_element_container.get_corrections()) {
    /* set concentration */
    element->set_concentration(element_concentration_map[element->get_name()], date_of_sample);
  }
  for (auto & element : m_element_container.get_dailies()) {
    /* set concentration */
    element->set_use_ms_mode(true);
    element->set_concentration(element_concentration_map[element->get_name()], date_of_sample);
    element->update_multiplier_from_ms_results();
  }
  /* handle iodine */
  m_p_active_icp_selection_window = m_p_ati_correction_start_window;
  m_p_ati_correction_start_window->set_iodine_increase(m_element_container.iodine()->is_low());
  m_p_ati_correction_start_window->set_iodine_decrease(m_element_container.iodine()->is_high());
  m_p_ati_correction_start_window->set_vanadium_increase(m_element_container.vanadium()->is_low());
  m_p_ati_correction_start_window->set_vanadium_decrease(m_element_container.vanadium()->is_high());
  this->setEnabled(true);
  this->_activate_icp_import_dialog();
}

void MainWindow::_handle_increase_iodine()
{
  m_element_container.iodine()->set_drops(
    m_element_container.iodine()->get_dose(
      std::chrono::
      year_month_day{}) + 1);
  m_p_settings_window->get_iodine_spinbox()->setValue(
    m_element_container.iodine()->get_dose(
      std::chrono::
      year_month_day{}));
  this->_refresh_elements();
}

void MainWindow::_handle_decrease_iodine()
{
  m_element_container.iodine()->set_drops(
    m_element_container.iodine()->get_dose(
      std::chrono::
      year_month_day{}) - 1);
  m_p_settings_window->get_iodine_spinbox()->setValue(
    m_element_container.iodine()->get_dose(
      std::chrono::
      year_month_day{}));
  this->_refresh_elements();
}

void MainWindow::_handle_increase_vanadium()
{
  m_element_container.vanadium()->set_drops(
    m_element_container.vanadium()->get_dose(std::chrono::year_month_day{}) +
    1);
  m_p_settings_window->get_vanadium_spinbox()->setValue(
    m_element_container.vanadium()->get_dose(std::chrono::year_month_day{}));
  this->_refresh_elements();
}

void MainWindow::_handle_decrease_vanadium()
{
  m_element_container.vanadium()->set_drops(
    m_element_container.vanadium()->get_dose(std::chrono::year_month_day{}) -
    1);
  m_p_settings_window->get_vanadium_spinbox()->setValue(
    m_element_container.vanadium()->get_dose(std::chrono::year_month_day{}));
  this->_refresh_elements();
}

}  // namespace reef_moonshiners::ui
