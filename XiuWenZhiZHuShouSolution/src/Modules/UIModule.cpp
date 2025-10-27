#include "UIModule.hpp"
#include "rqw_LabelWarning.h"

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