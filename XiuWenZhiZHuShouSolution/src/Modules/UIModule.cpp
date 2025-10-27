#include "UIModule.hpp"
#include "rqw_LabelWarning.h"
#include "ui_DlgProductSet.h"

void UIModule::build()
{
	labelWarning = new rw::rqw::LabelWarning();
}

void UIModule::destroy()
{
	delete labelWarning;
}

void UIModule::start()
{

}

void UIModule::stop()
{

}