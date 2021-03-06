#include <QtGui>
#include <iostream>
#include <string>
#include "seatdisplay.h"

bool admin = false;
bool cus = false; //These two `bool`s are used to check the type of user who is logged in
bool loginType; //TRUE: admin; FALSE: customer
QString cusREF;

int complexity(QString passwd) {
    bool lower = false, upper = false, numbers = false, symbols = false, longEnough = false;

    for (int i = (passwd.length() - 1); i >= 0; --i) {
        if(!lower && passwd[i].isLower()){lower = true;}
        if(!upper && passwd[i].isUpper()){upper = true;}
        if(!numbers && passwd[i].isDigit()){numbers = true;}
        if(!symbols && !(passwd[i].isLetterOrNumber())){symbols = true;}
        if(lower && upper && numbers && symbols){i = -1;} //This line allows code to break out of the `if` if all four things are found before the whole string is checked
    };

    if(passwd.length() >= 10){longEnough = true;}

    return lower + upper + numbers + symbols + longEnough;
    //`complexity()` IS NOT part of any class, so can ONLY be referred to
    //in this file. It uses a simple checking algorithm to return the
    //complexity of a string, and is used to make sure the user gives
    //a suitably complex password at sign up.
    //It returns a number which should be seen as being out of 5 [stars],
    //with a lower number indicating a weaker password.
}

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");/*"08bkrishna.no-ip.org"*/
    db.setDatabaseName("computing");/*"computing"*/
    db.setUserName("root");/*"balaji"*/
    db.setPassword("Balajikr1");
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
        db.lastError().text());
        return false;
    }
    return true;
    /*
      This function connects the program to the database hosted
      at 08bkrishna.no-ip.org called `computing`. It returns a
      boolean which is used in `seatdisplay::seatdisplay()` to
      tell the program whether it should show the dialog. This
      function also returns an error message if necessary.
    */
}

seatdisplay::seatdisplay(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    //Sets up the objects in the XML file "seatdisplay.ui"

    QRegExp rxSeats("[A-Z0-9]{3,3}");
    seats = this->findChildren<QPushButton *>(rxSeats);
    //These add all of the objects in the `seatdisplay` dialogue to a `QList`
    //which can then be used to edit the states of the buttons in other parts
    //of the program. We use `rxSeats` because in addition to the seat buttons
    //in the dialogue, there are some other buttons which we don't want in
    //this list, therefore `rxSeats` filters these out, as they do not follow
    //the naming convention used by the seat buttons.

    connect(bookButton, SIGNAL(clicked()), this, SLOT(seatbooking()));
    //Connects the `bookButton` `clicked()` signal to `seatbooking()`

    connect(actionBook_Seats, SIGNAL(triggered()), this, SLOT(seatbooking()));
    actionBook_Seats->setShortcut(tr("Ctrl+B"));
    actionBook_Seats->setToolTip(tr("Book selected seats"));
    //Sets up the action `actionBook_Seats`; sets the shortcut;
    //Sets the tooltip, to give users extra help

    connect(actionCancel_Seats, SIGNAL(triggered()), this, SLOT(cancelSeats()));
    actionCancel_Seats->setToolTip(tr("Cancel selected seats"));
    actionCancel_Seats->setShortcut(tr("Ctrl+C"));
    actionCancel_Seats->setDisabled(true);
    //insert connection for cancel seats function

    connect(actionLogin, SIGNAL(triggered()), this, SLOT(adminLogin()));
    actionLogin->setShortcut(tr("Alt+L"));

    connect(actionView_Settings, SIGNAL(triggered()), this, SLOT(showAdminPanel()));
    actionView_Settings->setShortcut(tr("Alt+S"));

    connect(actionCustomer_Login, SIGNAL(triggered()), this, SLOT(cusLogin()));
    actionCustomer_Login->setShortcut(tr("Ctrl+L"));

    connect(action_Quit, SIGNAL(triggered()), this, SLOT(close()));
    action_Quit->setShortcut(tr("Ctrl+Q"));
    action_Quit->setToolTip(tr("Quit the application"));
    //These `connect()`s do the same as the one for `actionBook_Seats`
    //and follow the same setup.

    connect(cmbEvent,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDay(QString)));

    this->setWindowTitle("Seating Plan");

    if(!createConnection())
    {
        QMessageBox error;
        error.setWindowTitle("Connection Error");
        error.setText("Can't connect to database.\nPlease check internet connection.");
        error.exec();
        this->close();
    }
    else {
        this->show();
    }
    //Referred to in the comments in "main.cpp", you can see here that
    //I have used the `QWidget::show()` function inside the initiator
    //function, as this allows me to prevent the `seatdisplay` dialogue
    //from appearing when the DB cannot be found. If I used `show()` in
    //`main()` in "main.cpp", then the dialogue would apear in either case
    //which could lead customers into think that they have booked seats
    //when they actually haven't.

    QSqlQuery query;
    query.exec("SELECT * FROM seats WHERE disabled=1");

    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if(seats[i]->objectName() == query.value(0).toString())
            {
                seats[i]->setDisabled(true);
            }
        }
    }
    //This disables the seats which have been marked as disabled by
    //the admin users in the DB, and they are only enabled if
    //the user explicitly wants disabled seats, and ticks the
    //checkbox `checkBox` which signals this intent to the program.

    query.exec("SELECT * FROM events");

    while(query.next())
    {
        cmbEvent->addItem(query.value(0).toString());
    }
    //This adds every event in `computing`.`events` to the combo box `cmbEvent`

    disableBooked(cmbEvent->itemText(0));
    //This disables any seats booked for this event, which is the default.

    cancelButton->setVisible(false);
    backButton->setVisible(false);
}

disabilityEditor::disabilityEditor(QDialog *parent) : QDialog(parent)
{
    setupUi(this);

    QRegExp rxSeats("[A-Z0-9]{0,3}");
    seats = this->findChildren<QPushButton *>(rxSeats);
    //These add all of the objects in the `disabilityEditor` dialogue to a `QList`
    //which can then be used to edit the states of the buttons from other parts
    //of the program. We use `rxSeats` because in addition to the seat buttons
    //in the dialogue, there are some other buttons which we don't want in
    //this list, therefore `rxSeats` filters these out, as they do not follow
    //the naming convention used by the seat buttons.

    setDisab();

    connect(setDis, SIGNAL(clicked()), this, SLOT(setDisab()));
    connect(setNotDis, SIGNAL(clicked()), this, SLOT(setNotDisab()));
    /*
      These `connect()`s set the functions called by each button when
      they give the signal `clicked()`, i.e. when they are clicked.
      */
}

booking_dialogue::booking_dialogue(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    QRegExp email("[a-z0-9._%+-]+@[a-z0-9.-]+[.]{1,1}[a-z]{2,4}");
    QRegExp name("[A-Za-z]{2,20}");
    QRegExp tel_no("[0-9]{11,11}");

    txt_email->setValidator(new QRegExpValidator(email, this));
    fName->setValidator(new QRegExpValidator(name, this));
    sName->setValidator(new QRegExpValidator(name, this));
    telNo->setValidator(new QRegExpValidator(tel_no, this));

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(txt_passwd, SIGNAL(textEdited(QString)), this, SLOT(passChecker(QString)));
}

adminpanel::adminpanel(QDialog *parent) : QDialog(parent)
{
    setupUi(this);
    connect(btn_disabled, SIGNAL(clicked()), this, SLOT(disabledSeating()));
    connect(btn_prices, SIGNAL(clicked()), this, SLOT(priceChange()));
}

login::login(QDialog *parent) : QDialog(parent)
{
    setupUi(this);
    if(loginType)
    {
        setWindowTitle("Admin Login");
        connect(okButton, SIGNAL(clicked()), this, SLOT(adminLogin()));
    }
    else {
        lineEdit->setPlaceholderText("Your eMail, e.g. john@example.co.uk");
        setWindowTitle("Login");
        connect(okButton, SIGNAL(clicked()), this, SLOT(cusLogin()));
    }
}

tickets::tickets(QWidget *parent) : QDialog(parent)
{

}

void tickets::paintEvent(QPaintEvent *)
{
//    QPrinter printer;
//    QPrintDialog printDialog(&printer, this);
//    if(printDialog.exec()) {
        QPainter painter(/*&printer*/this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPixmap ticket(":/resources/ticket.png");
        painter.drawPixmap(10,10,347,174,ticket);
        QFont font("SimHei", 18);
        painter.setFont(font);
        painter.drawText(130,58,200,100,Qt::AlignLeft,"A01\n�10");
        painter.setFont(QFont("SimHei", 14));
        painter.drawText(130,116,180,50,Qt::AlignLeft,"Saturday - 1900");
//    }
}

void seatdisplay::showAdminPanel()
{
    this->setEnabled(false);

    if(!admin)
    {
        QMessageBox error;
        error.setText("You are not logged in.\nPlease login to access the admin panel.");
        error.exec();
    }
    else {
        adminpanel *panel = new adminpanel;
        panel->exec();
        changeDay(cmbEvent->currentText());
    }

    this->setEnabled(true);
}

void seatdisplay::adminLogin()
{
    if(cus) {
        QMessageBox error;
        error.setText("You are already logged in as a customer.\nLogout first, then login as an administrator.");
        error.exec();
    } else {
        this->setEnabled(false);

        loginType = true;
        login *dialog = new login;
        dialog->exec();

        if(admin)
        {
            actionLogin->setText("Logout");
            connect(actionLogin, SIGNAL(triggered()), this, SLOT(logout()));
        }

        this->setEnabled(true);
    }
}

void seatdisplay::cusLogin()
{
    if(admin) {
        QMessageBox error;
        error.setText("You are already logged in as an administrator.\nLogout first, then login as a customer.");
        error.exec();
    } else {
        this->setEnabled(false);

        loginType = false;
        login *dialog = new login;
        dialog->exec();

        if(cus)
        {
            actionCustomer_Login->setText("Logout");
            connect(actionCustomer_Login, SIGNAL(triggered()), this, SLOT(logout()));
            customerBooked(cmbEvent->currentText());
            actionCancel_Seats->setEnabled(true);
        }

        this->setEnabled(true);
    }
}

void seatdisplay::logout()
{
    if(cus) {
        cus = false;
        actionCustomer_Login->setText("Customer Login");
        connect(actionCustomer_Login, SIGNAL(triggered()), this, SLOT(cusLogin()));
        actionCancel_Seats->setDisabled(true);
        changeDay(cmbEvent->currentText());
    } else if (admin) {
        admin = false;
        actionLogin->setText("Login");
        connect(actionLogin, SIGNAL(triggered()), this, SLOT(adminLogin()));
    }
}

void seatdisplay::cancelSeats() {
        for(int i = 0; i < seats.size(); ++i)
        {
            if(seats[i]->styleSheet() != "")
            {
                seats[i]->setEnabled(true);
            } else {
                seats[i]->setDisabled(true);
            }
        }
        cancelButton->setVisible(true);
        backButton->setVisible(true);
        bookButton->setEnabled(false);
        cmbEvent->setEnabled(false);
        checkBox->setEnabled(false);
}

void seatdisplay::seatbooking()
{
    if(cus){
        QList<QPushButton *> checked;
        for(int i = 0; i < seats.size(); ++i)
        {
            if(seats[i]->isChecked() && seats[i]->isCheckable())
            {
                checked.append(seats[i]);
            }
        }
        QSqlQuery booked;
        booked.exec("SELECT * FROM bookings WHERE Events_ID='" + cmbEvent->currentText() + "' AND Customer_REF='" + cusREF + "'");
        if(checked.size() + booked.size() > 10) {
            QMessageBox msgbox;
            msgbox.setText("You chosen too many seats.\nPlease go back and choose a maximum of 10 seats.");
            msgbox.exec();
        } else {
            QSqlQuery bookSeat;
            for(int i = 0; i < checked.size(); ++i) {
                bookSeat.prepare("INSERT INTO bookings VALUES (?,?,?)");
                bookSeat.addBindValue((*checked[i]).objectName());
                bookSeat.addBindValue(cusREF);
                bookSeat.addBindValue(cmbEvent->currentText());
                bookSeat.exec();
            }
        }
        customerBooked(cmbEvent->currentText());
    } else {
        this->setEnabled(false);
        booking_dialogue *dialog = new booking_dialogue;

        dialog->comboBox->clear();

        QSqlQuery query;
        QSqlQuery prices;
        QSqlRecord rec;

        for(int i = 0; i < seats.size(); ++i)
        {
            if(seats[i]->isChecked() && seats[i]->isCheckable())
            {
                dialog->comboBox->addItem(seats[i]->objectName());

                QString quer("SELECT * FROM seats WHERE seat='");
                quer.append(seats[i]->objectName()); quer.append("'");

                query.prepare(quer);
                query.exec();
                query.first();
                rec = query.record();

                int priceBand = rec.value(2).toInt();
                prices.exec("SELECT * FROM prices");

                while(prices.next())
                {
                    if(priceBand == prices.value(0).toInt())
                    {
                        dialog->lcdNumber->display(dialog->lcdNumber->value() + prices.value(1).toDouble());
                    }
                }
            }
        }

        dialog->dayLabel->setText(cmbEvent->currentText());

        if(dialog->comboBox->count() == 0) {
            QMessageBox msgbox;
            msgbox.setText("You haven't chosen any seats.\nPlease go back and choose which seats you wish to book.");
            msgbox.exec();
        }
        else if(dialog->comboBox->count() > 10)
        {
            QMessageBox msgbox;
            msgbox.setText("You chosen too many seats.\nPlease go back and choose a maximum of 10 seats.");
            msgbox.exec();
        }
        else {
          //dialog->show();
            dialog->exec();
        }

        this->setEnabled(true);

        if(cus)
        {
            actionCustomer_Login->setText("Logout");
            connect(actionCustomer_Login, SIGNAL(triggered()), this, SLOT(logout()));
            customerBooked(cmbEvent->currentText());
            actionCancel_Seats->setEnabled(true);
        }
        //`exec()` waits for it to close, meaning that once we are done with the dialogue box, here the booking dialogue,
        //we can go back to this function, and re-enable the main window which at the start of the function was set
        //to disabled. using show does not do this, therefore exec is used here.
    }
}

void seatdisplay::changeDay(QString event)
{
    for(int i = 0; i < seats.size(); ++i)
    {
        seats[i]->setEnabled(true);
        seats[i]->setStyleSheet("");
    }
    checkBox->toggle(); checkBox->toggle();
    disableBooked(event);
    if(cus) {
        customerBooked(event);
    }
}

void booking_dialogue::on_okButton_clicked()
{
    QRegExp email("[a-z0-9._%+-]+@[a-z0-9.-]+[.]{1,1}[a-z]{2,4}");
    QRegExp tel_no("[0-9]{11,11}");
    QString error;

    if(!(tel_no.exactMatch(telNo->text())))
    {
        error += "Your Telephone No. is invalid.\n";
    }
    if(!(email.exactMatch(txt_email->text())))
    {
        error += "Your eMail is invalid.\n";
    }
    if(txt_passwd->text().length() < 10)
    {
        error += "Your password is too short. It must be 10 or more characters long.\n";
    }
    if(!(error.isEmpty()))
    {
        QMessageBox error_box;
        error_box.setText(error);
        error_box.exec();
    }
    else {
        QSqlQuery siz1;
        QString siz1quer("SELECT * FROM customer WHERE eMail='");
        siz1quer.append(txt_email->text()); siz1quer.append("'");
        siz1.exec(siz1quer);
//START CREATING THE CUS-REF
        int cus1;
        for (int i = 0; i < (txt_email->text().length()/3); ++i) {
            if(qPrintable(txt_email->text())[i] == '@') {}
            else {
                cus1 *= qPrintable(txt_email->text())[i];
            }
        }
        int cus2;
        for (int i = (txt_email->text().length()/3); i < 2*(txt_email->text().length()/3); ++i) {
            if(qPrintable(txt_email->text())[i] == '@') {}
            else {
                cus2 *= qPrintable(txt_email->text())[i];
            }
        }
        int cus3;
        for (int i = 2*(txt_email->text().length()/3); i < (3*(txt_email->text().length()/3) - 2); ++i) {
            if(qPrintable(txt_email->text())[i] == '@') {}
            else {
                cus3 *= qPrintable(txt_email->text())[i];
            }
        }
        char hex[2560]; //cus-ref var
        sprintf(hex, "%x%x%x", cus1, cus2, cus3); //cus-ref var filled
//END CREATING THE CUS-REF
        if(siz1.size() != 0)
        {
            QMessageBox error_box;
            error_box.setText("eMail in use.");
            error_box.exec();
        } else {
            QSqlQuery insertCus;
            insertCus.prepare("INSERT INTO customer VALUES (?,?,?,?,?,?)");

            insertCus.addBindValue(hex);
            insertCus.addBindValue(fName->text());
            insertCus.addBindValue(sName->text());
            insertCus.addBindValue(telNo->text());
            insertCus.addBindValue(txt_email->text());
            insertCus.addBindValue(txt_passwd->text());

            insertCus.exec();
            cus = true;
            cusREF = hex; std::cerr << "cusREF: " << qPrintable(cusREF) << "\ncus: " << cus << '\n';

            QSqlQuery bookSeat;
            for(int i = 0; i < comboBox->count(); ++i) {
                bookSeat.prepare("INSERT INTO bookings VALUES (?,?,?)");
                bookSeat.addBindValue(comboBox->itemText(i));
                bookSeat.addBindValue(cusREF);
                bookSeat.addBindValue(dayLabel->text());
                bookSeat.exec();
            }
            this->close();
        }
    }
}

/*
  on_okButton_clicked should be redefined in last section to give user more information, i.e. use multiple 'if'
  statements to give the user an idea of what data has been duplicated (REF no. or email).
  */

void seatdisplay::on_clearButton_clicked()
{
    for(int i = 0; i < seats.size(); ++i)
    {
        seats[i]->setChecked(false);
    }
}

void adminpanel::disabledSeating()
{
    this->setEnabled(false);

    disabilityEditor *dialog = new disabilityEditor;
    dialog->exec();

    this->setEnabled(true);
}

void adminpanel::priceChange()
{

}

void login::adminLogin()
{
    if(lineEdit->text().length() == 0 || lineEdit_2->text().length() == 0)
    {
        QMessageBox error;
        error.setText("Please enter your login details.");
        error.exec();
    }
    else {
        QSqlQuery query;

        QString quer("SELECT * FROM administrators WHERE (username='");
        quer.append(lineEdit->text());
        quer.append("')");

        query.exec(quer);
        query.first();

        QSqlRecord rec = query.record();
        if(query.size() == 0)
        {
            QMessageBox error;
            error.setText("Username does not exist.");
            error.exec();
        } else {
            if(rec.value(1).toString() == lineEdit_2->text())
            {
                admin = true;
                this->close();
            } else {
                QMessageBox error;
                error.setText("Incorrect Password.");
                error.exec();
            }
        }
    }
}

void login::cusLogin()
{
    if(lineEdit->text().length() == 0 || lineEdit_2->text().length() == 0)
    {
        QMessageBox error;
        error.setText("Please enter your login details.");
        error.exec();
    }
    else {
        QSqlQuery query;

        QString quer("SELECT * FROM customer WHERE (email='");
        quer.append(lineEdit->text());
        quer.append("')");

        query.exec(quer);
        query.first();

        QSqlRecord rec = query.record();
        if(query.size() == 0)
        {
            QMessageBox error;
            error.setText("User does not exist.");
            error.exec();
        } else {
            if(rec.value(5).toString() == lineEdit_2->text())
            {
                cusREF = rec.value(0).toString();
                cus = true;
                this->close();
            } else {
                QMessageBox error;
                error.setText("Incorrect Password");
                error.exec();
            }
        }
    }
}

void seatdisplay::on_checkBox_toggled(bool checked)
{
    QSqlQuery query;
    query.exec("SELECT * FROM seats WHERE disabled=1");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if(seats[i]->objectName() == query.value(0).toString())
            {
                seats[i]->setDisabled(!checked);
            }
        }
    }
    disableBooked(cmbEvent->currentText());
}

void booking_dialogue::passChecker(QString passwd) {
    if(complexity(passwd) < 3) {
        passStrength->setText("Weak");
        passStrength->setStyleSheet("background-color: red; color: white;");
    } else if(complexity(passwd) == 3) {
        passStrength->setText("OK");
        passStrength->setStyleSheet("background-color: yellow;");
    } else {
        passStrength->setText("Strong");
        passStrength->setStyleSheet("background-color: green;");
    }
}

void seatdisplay::disableBooked(QString event)
{
    QSqlQuery query;
    query.exec("SELECT * FROM bookings WHERE Events_ID='" + event + "'");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if(seats[i]->objectName() == query.value(0).toString())
            {
                seats[i]->setDisabled(true);
            }
        }
    }
}

void seatdisplay::customerBooked(QString event)
{
    QSqlQuery query;
    query.exec("SELECT * FROM bookings WHERE Events_ID='" + event + "' AND Customer_REF='" + cusREF + "'");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if(seats[i]->objectName() == query.value(0).toString())
            {
                seats[i]->setDisabled(true);
                seats[i]->setStyleSheet("QPushButton {color: #333;border: 1px solid #555;border-radius: 4px;margin:1px;"
                                        "background: qradialgradient(cx: 0.3, cy: -0.4, fx: 0.3, fy: -0.4, radius: 1.35, stop: 0 #fff, stop: 1 #888);}"
                                        "QPushButton:hover {background: qradialgradient(cx: 0.3, cy: -0.4, fx: 0.3, fy: -0.4, radius: 1.35, stop: 0 #888, stop: 1 #bbb);}"
                                        "QPushButton:checked {background: qradialgradient(cx: 0.3, cy: -0.4, fx: 0.3, fy: -0.4, radius: 1.35, stop: 0 #999, stop: 1 #fff);}");
                seats[i]->setChecked(false);
            }
        }
    }
}

void disabilityEditor::setDisab()
{
    QSqlQuery *query1 = new QSqlQuery;
    for(int i = 0; i < seats.size(); ++i)
    {
        if(seats[i]->isChecked() && seats[i]->isCheckable())
        {
            query1->exec("UPDATE `computing`.`seats` SET disabled=1 WHERE seat='" + seats[i]->objectName() + "'");
            seats[i]->setChecked(false);
        }
    }
    QSqlQuery query;
    query.exec("SELECT * FROM seats WHERE disabled=1");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if(seats[i]->objectName() == query.value(0).toString())
            {
                seats[i]->setStyleSheet("QPushButton {color: #333;border: 1px solid #555;border-radius: 4px;margin:1px;"
                                        "background: qradialgradient(cx: 0.3, cy: -0.4, fx: 0.3, fy: -0.4, radius: 1.35, stop: 0 #fff, stop: 1 #888);"
                                        "image: url(:/resources/wheelchair-icon.png);}"
                                        "QPushButton:hover{background: qradialgradient(cx: 0.3, cy: -0.4, fx: 0.3, fy: -0.4, radius: 1.35, stop: 0 #888, stop: 1 #bbb);}"
                                        "QPushButton:checked{background: qradialgradient(cx: 0.3, cy: -0.4, fx: 0.3, fy: -0.4, radius: 1.35, stop: 0 #999, stop: 1 #fff);}");
            }
        }
    }
}

void disabilityEditor::setNotDisab()
{
    QSqlQuery *query1 = new QSqlQuery;
    for(int i = 0; i < seats.size(); ++i)
    {
        if(seats[i]->isChecked() && seats[i]->isCheckable())
        {
            query1->exec("UPDATE `computing`.`seats` SET disabled=0 WHERE seat='" + seats[i]->objectName() + "'");
            seats[i]->setChecked(false);
        }
    }
    QSqlQuery query;
    query.exec("SELECT * FROM seats WHERE disabled=0");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if(seats[i]->objectName() == query.value(0).toString())
            {
                seats[i]->setStyleSheet("");
            }
        }
    }
}

void seatdisplay::on_backButton_clicked()
{
    backButton->setVisible(false);
    cancelButton->setVisible(false);
    bookButton->setEnabled(true);
    cmbEvent->setEnabled(true);
    checkBox->setEnabled(true);
    changeDay(cmbEvent->currentText());
}

void seatdisplay::on_cancelButton_clicked()
{
    QSqlQuery deleteSeat;
    for(int i = 0; i < seats.size(); ++i)
    {
        if(seats[i]->isChecked() && seats[i]->isCheckable() && seats[i]->isEnabled())
        {
            deleteSeat.exec("DELETE FROM bookings WHERE Seats_seat='" + seats[i]->objectName() + "' AND Events_ID='" + cmbEvent->currentText() + "'");
            seats[i]->setStyleSheet("");
            seats[i]->setEnabled(false);
            seats[i]->setChecked(false);
        }
    }
}

void seatdisplay::on_pushButton_clicked()
{
    tickets *test = new tickets;
    test->exec();
}
