#include <QApplication>
#include "seatdisplay.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    seatdisplay *dialog = new seatdisplay;
    //dialog->show();
    return app.exec();
}
