// Copyright 2023 Hunter L. Allen
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

#include <reef_moonshiners/ui/element_settings.hpp>

namespace reef_moonshiners::ui
{

ElementSettings::ElementSettings(
  reef_moonshiners::ElementContainer * const _p_element_container,
  QWidget * _parent)
: QWidget(_parent),
  m_p_element_container(_p_element_container),
  m_p_parent(_parent)
{
  m_p_vbox_layout = new QVBoxLayout(this);
  /* connect each element to a multiplier */
  for (auto * const element : m_p_element_container->get_dailies()) {
    if (nullptr != dynamic_cast<reef_moonshiners::DropperElement *>(element)) {
      continue;
    }
    auto * row = m_rows.emplace_back(new QHBoxLayout);
    auto * label = m_labels.emplace_back(new QLabel(tr(element->get_name().c_str())));
    auto * spin_box = m_multipliers.emplace_back(new QSpinBox);
    spin_box->setValue((int) element->get_multiplier());
    std::string element_name = element->get_name();
    QObject::connect(
      spin_box, &QSpinBox::valueChanged,
      [&, name = std::move(element_name)](const int multiplier){
	auto * daily = dynamic_cast<reef_moonshiners::DailyElement *>((*m_p_element_container)[name]);
	daily->set_multiplier(multiplier);
      });
    row->addWidget(label);
    row->addWidget(spin_box);
    m_p_vbox_layout->addLayout(row);
  }

  m_p_okay_button = new QPushButton(tr("&Okay"));
  m_p_okay_button_layout = new QHBoxLayout();
  m_p_okay_button_layout->addSpacing(this->size().width() / 2);
  m_p_okay_button_layout->addWidget(m_p_okay_button);

  m_p_vbox_layout->addLayout(m_p_okay_button_layout);
}

QPushButton * ElementSettings::get_okay_button() const
{
  return m_p_okay_button;
}

void ElementSettings::refresh_values()
{
  const auto dailies = m_p_element_container->get_dailies();
  for (size_t x = 0; x < m_multipliers.size(); ++x) {
    reef_moonshiners::DailyElement * const element = dailies[x];
    m_multipliers[x]->setValue((int) element->get_multiplier());
  }
}

void ElementSettings::resizeEvent(QResizeEvent * event)
{
  this->QWidget::resizeEvent(event);
  QLayoutItem * item = m_p_okay_button_layout->itemAt(0);  /* spacer */
  m_p_okay_button_layout->removeItem(item);
  m_p_okay_button_layout->insertSpacing(0, this->size().width() / 2);
  m_p_okay_button_layout->addWidget(m_p_okay_button);
}

}  // namespace reef_moonshiners::ui
