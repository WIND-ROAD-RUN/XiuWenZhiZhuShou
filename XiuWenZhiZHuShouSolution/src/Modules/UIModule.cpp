#include "UIModule.hpp"
#include "DlgProductSet.h"
#include "rqw_LabelWarning.h"
#include "ui_DlgProductScore.h"
#include "ui_DlgProductSet.h"

void UIModule::build()
{
	_dlgProductSet = new DlgProductSet();
	labelWarning = new rw::rqw::LabelWarning();
}

void UIModule::destroy()
{
	delete _dlgProductSet;
	delete labelWarning;
}

void UIModule::start()
{

}

void UIModule::stop()
{

}