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
#include "ui_seatdisplay.h"
#include "ui_booking.h"
#include "ui_adminpanel.h"
#include "ui_waiting.h"
#include "ui_login.h"
#include "ui_disabilityEditor.h"

class seatdisplay : public QMainWindow, public Ui::seatdisplay
{
    Q_OBJECT

public:
    seatdisplay(QWidget *parent = 0);
    QList<QPushButton *> seats;

    //int A_C_seats;
    //int D_F_seats;
    //int G_L_seats;

    std::vector<std::vector<QString> > booked;
    std::vector<QString> fri_clicked;
    std::vector<QString> sat_clicked;

    std::vector<std::pair<char, int> > seat_displacement;

private slots:
    void seatbooking();
    void init_vector(std::vector<std::pair<char, int> >& vec);
    void adminLogin();
    void cusLogin();
    void logout();
    void showAdminPanel();
    void on_friRadio_toggled(bool checked);
    void on_satRadio_toggled(bool checked);
    void on_pushButton_clicked();
    void on_checkBox_toggled(bool checked);
    void disableBooked(QString event);
    void customerBooked(QString event);
};

class booking_dialogue : public QDialog, public Ui::booking
{
    Q_OBJECT

public:
    booking_dialogue(QWidget *parent = 0);
    std::vector<QString> details;
    QString refCode;
private slots:
    void on_okButton_clicked();
    void passChecker(QString passwd);
};

class adminpanel : public QDialog, public Ui::adminpanel
{
    Q_OBJECT

public:
    adminpanel(QDialog *parent = 0);
private slots:
    void disabledSeating();
    void priceChange();
};

class waiting : public QDialog, public Ui::waiting
{
    Q_OBJECT

public:
    waiting(QDialog *parent = 0);
};

class login : public QDialog, public Ui::Login
{
    Q_OBJECT

public:
    login(QDialog *parent = 0);
private slots:
    void adminLogin();
    void cusLogin();
};

class disabilityEditor : public QDialog, public Ui::disabilityEditor
{
    Q_OBJECT

public:
    disabilityEditor(QDialog *parent = 0);
    QList<QPushButton *> seats;
private slots:
    void setDisab();
    void setNotDisab();
};

#endif // SEATDISPLAY_H
