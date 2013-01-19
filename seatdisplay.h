#ifndef SEATDISPLAY_H
#define SEATDISPLAY_H

#include <QMainWindow>
#include <qmath.h>
#include <vector>
#include <utility>
#include <string>
#include <QRegExp>
#include <QtSql/QtSQL>
#include <QMYSQLDriver>
#include <QMYSQLResult>
#include <QStringBuilder>
#include "ui_seatdisplay.h"
#include "ui_booking.h"
#include "ui_adminpanel.h"
#include "ui_waiting.h"
#include "ui_login.h"
#include "ui_disabilityEditor.h"

class seatdisplay : public QMainWindow, public Ui::seatdisplay
{
    //This class is for the main window of the application which contains
    //all of the seats and the primary function of the program: booking,
    //cancelling, login and admin access
    Q_OBJECT

public:
    seatdisplay(QWidget *parent = 0); //Initiator function for this class
    QList<QPushButton *> seats; //List of all buttons in the form which are seats

    std::vector<std::vector<QString> > booked; //List of seats booked as shown by the DB
    std::vector<QString> fri_clicked; //List of seats which have been clicked for the friday; Allows users to switch between days without losing selection
    std::vector<QString> sat_clicked; //Used in conjunction with `fri_clicked`

private slots:
    void seatbooking();
    void adminLogin();
    void cusLogin();
    void logout();
    void showAdminPanel();
    void on_clearButton_clicked();
    void on_checkBox_toggled(bool checked);
    void disableBooked(QString event);
    void customerBooked(QString event);
    void cancelSeats();
    void changeDay(QString event);
    //Go to "seatdisplay.cpp" for function definitions and explanation of usage
    void on_backButton_clicked();
    void on_cancelButton_clicked();
    void on_pushButton_clicked();
};

class booking_dialogue : public QDialog, public Ui::booking
{
    //This class is for the dialogue which is for users to input
    //their information, if not already logged in.
    Q_OBJECT

public:
    booking_dialogue(QWidget *parent = 0);
    QString refCode;
private slots:
    void on_okButton_clicked();
    void passChecker(QString passwd);
    //Go to "seatdisplay.cpp" for function definitions and explanation of usage
};

class adminpanel : public QDialog, public Ui::adminpanel
{
    //This class is for the dialogue which directs admins to
    //different admin options, e.g. it links to the dialogue
    //which allows admins to change which seats are disabled
    //This signposting allows for ease of use
    //This should only be accessable by admins who have
    //already logged in.
    Q_OBJECT

public:
    adminpanel(QDialog *parent = 0);
private slots:
    void disabledSeating();
    void priceChange();
    //Go to "seatdisplay.cpp" for function definitions and explanation of usage
};

class waiting : public QDialog, public Ui::waiting
{
    Q_OBJECT

public:
    waiting(QDialog *parent = 0);
};

class login : public QDialog, public Ui::Login
{
    //This class is for the dialogue which allows users and
    //admins to login. It uses its slots to customize it for
    //each use, getting rid of the need for 2 separate dialogues.
    Q_OBJECT

public:
    login(QDialog *parent = 0);
private slots:
    void adminLogin();
    void cusLogin();
    //Go to "seatdisplay.cpp" for function definitions and explanation of usage
};

class disabilityEditor : public QDialog, public Ui::disabilityEditor
{
    //This class is for the dialogue which is used to edit which
    //seats are disabled, and can only be accessed from the
    //`adminpanel` object, to prevent editing by users.
    Q_OBJECT

public:
    disabilityEditor(QDialog *parent = 0);
    QList<QPushButton *> seats;
private slots:
    void setDisab();
    void setNotDisab();
    //Go to "seatdisplay.cpp" for function definitions and explanation of usage
};

class tickets : public QDialog {
    Q_OBJECT

public:
    tickets(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // SEATDISPLAY_H
