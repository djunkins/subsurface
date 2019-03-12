// SPDX-License-Identifier: GPL-2.0
#include "TabDiveSite.h"
#include "qt-models/divelocationmodel.h"

#include <qt-models/divecomputerextradatamodel.h>

TabDiveSite::TabDiveSite(QWidget *parent) : TabBase(parent)
{
	ui.setupUi(this);
	ui.diveSites->setTitle(tr("Dive sites"));
	ui.diveSites->setModel(&model);
	// Default: sort by name
	ui.diveSites->view()->sortByColumn(LocationInformationModel::NAME, Qt::AscendingOrder);
	ui.diveSites->view()->setSortingEnabled(true);

	// Show only the first few columns
	for (int i = LocationInformationModel::COORDS; i < LocationInformationModel::COLUMNS; ++i)
		ui.diveSites->view()->setColumnHidden(i, true);
}

void TabDiveSite::updateData()
{
}

void TabDiveSite::clear()
{
}
