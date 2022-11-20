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

#include <reef_moonshiners/ui/about_window.hpp>

namespace reef_moonshiners::ui
{

AboutWindow::AboutWindow(QWidget * parent)
: QWidget(parent),
  m_moonshiner_logo(":/icon.png")
{
  m_p_about_label = new QLabel("About Reef Moonshiners");
  m_p_about_label->setAlignment(Qt::AlignHCenter);

  m_p_image_label = new QLabel();
  m_p_image_label->setPixmap(m_moonshiner_logo);
  m_p_image_label->setAlignment(Qt::AlignHCenter);

  m_p_handbook_link_label = new QLabel();
  m_p_handbook_link_label->setText(
    "<a href=\"https://www.reefmoonshiners.com/_files/ugd/e30d85_bacea6453df24b5ca30d92d64279d228.pdf\">"
    "Reef Moonshiner's Handbook</a>");
  m_p_handbook_link_label->setTextFormat(Qt::RichText);
  m_p_handbook_link_label->setTextInteractionFlags(Qt::TextBrowserInteraction);
  m_p_handbook_link_label->setOpenExternalLinks(true);
  m_p_handbook_link_label->setAlignment(Qt::AlignHCenter);

  m_p_faq_link_label = new QLabel();
  m_p_faq_link_label->setText(
    "<a href=\"https://www.reefmoonshiners.com/faq\">"
    "Reef Moonshiner's FAQ</a>");
  m_p_faq_link_label->setTextFormat(Qt::RichText);
  m_p_faq_link_label->setTextInteractionFlags(Qt::TextBrowserInteraction);
  m_p_faq_link_label->setOpenExternalLinks(true);
  m_p_faq_link_label->setAlignment(Qt::AlignHCenter);

  m_p_main_layout = new QVBoxLayout(this);
  m_p_main_layout->addWidget(m_p_about_label);
  m_p_main_layout->addWidget(m_p_image_label);
  m_p_main_layout->addWidget(m_p_handbook_link_label);
  m_p_main_layout->addWidget(m_p_faq_link_label);

  m_p_main_layout->setAlignment(Qt::AlignHCenter);
}

void AboutWindow::resizeEvent(QResizeEvent * event)
{
  static_cast<void>(event);  /* unused */
  m_p_image_label->setPixmap(
    m_moonshiner_logo.scaled(
      m_p_image_label->size(),
      Qt::KeepAspectRatio,
      Qt::SmoothTransformation));
}

}  // namespace reef_moonshiners::ui
