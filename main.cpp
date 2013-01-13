#include <QApplication>
#include "seatdisplay.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    seatdisplay *dialog = new seatdisplay;
    //dialog->show();
    return app.exec();
}
/*
  Here the main function is defined, and when this is called
  a new `seatdisplay` pointer is created. LINE 8 is redundant
  due to the addition of a `show()` call in LINE 108 of
  "seatdisplay.cpp", which allows the object to choose
  whether it should open (Fully explained in "seatdisplay.ccp").
*/

