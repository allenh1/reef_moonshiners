^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package reef_moonshiners
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.2.0 (2022-09-17)
------------------
* Disable nano dose checkbox on Windows
* Fix nano dose saving on android
* Add Nano Doses (`#26 <https://github.com/allenh1/reef_moonshiners/issues/26>`_)
* Add Okay Button to Settings Window (`#25 <https://github.com/allenh1/reef_moonshiners/issues/25>`_)
* Increase CheckBox Size (`#24 <https://github.com/allenh1/reef_moonshiners/issues/24>`_)
  * Change ElementDisplay to inherit from QListWidgetItem
  * Fix save file path
  * Increase checkbox size
* Implement Rubidium Element (`#23 <https://github.com/allenh1/reef_moonshiners/issues/23>`_)
* Fix "App Freezes with Invalid ATI ID" (`#22 <https://github.com/allenh1/reef_moonshiners/issues/22>`_)
* Fix Barium calculation (`#20 <https://github.com/allenh1/reef_moonshiners/issues/20>`_)
* Implement Iodine and Vanadium (`#17 <https://github.com/allenh1/reef_moonshiners/issues/17>`_)
  * Implement Iodine
  * Implement Vanadium element
* Contributors: Hunter L. Allen

0.1.0 (2022-09-11)
------------------
* Update README
* Persistent Settings (`#12 <https://github.com/allenh1/reef_moonshiners/issues/12>`_)
  * Add main window icon
  * Implement persistent storage
* Generate qmake From CMake (`#10 <https://github.com/allenh1/reef_moonshiners/issues/10>`_)
  * Generate .pro file from CMake
  * Fix clang build error
* Import ATI ICP (`#5 <https://github.com/allenh1/reef_moonshiners/issues/5>`_)
  * Add ICP test selection window
  * Fix bug altering dose after changing tank size
  * Fetch corrections from ATI and populate calendar
  * Show dosage from selected calendar date
* Add Main UI (`#3 <https://github.com/allenh1/reef_moonshiners/issues/3>`_)
  * Implemented basic interface
  * Allow Qt6 build
  * Add Settings window with refugium toggle and tank size input
* Add readme (`#2 <https://github.com/allenh1/reef_moonshiners/issues/2>`_)
* Initial Implementation (`#1 <https://github.com/allenh1/reef_moonshiners/issues/1>`_)
  * Implement daily elements
  * Add basic testing
  * Add copyright headers
  * Implement correction elements
  * Fill in element concentrations with values from Andre
* init
* Contributors: Hunter L. Allen
