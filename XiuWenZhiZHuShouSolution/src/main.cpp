#include <QMessageBox>
#include <QtWidgets/QApplication>
#include "rqw_RunEnvCheck.hpp"
#include "Utilty.hpp"
#include "HandleScanner.h"
#include "Modules.hpp"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

	if (!Modules::check())
	{
		return 1;
	}

	Modules::getInstance().build();

    HandleScanner w;
	Modules::getInstance().uiModule._handleScanner = &w;

	Modules::getInstance().connect();
	Modules::getInstance().start();
	
#ifdef NDEBUG
	w.showFullScreen();
#else
	w.show();
#endif
    return a.exec();
}
