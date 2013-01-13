#include <QtGui>
#include <iostream>
#include <string>
#include "seatdisplay.h"

bool admin = false;
bool cus = false;
bool loginType; //TRUE: admin; FALSE: customer
QString cusREF;

int complexity(QString passwd) {
    bool lower = false, upper = false, numbers = false, symbols = false, longEnough = false;

    for (int i = (passwd.length() - 1); i >= 0; --i) {
        if(!lower && passwd[i].isLower()){lower = true;}
        if(!upper && passwd[i].isUpper()){upper = true;}
        if(!numbers && passwd[i].isDigit()){numbers = true;}
        if(!symbols && !(passwd[i].isLetterOrNumber())){symbols = true;}
        if(lower && upper && numbers && symbols){i = -1;}
    };

    if(passwd.length() >= 10){longEnough = true;}

    return 5 - lower - upper - numbers - symbols - longEnough;
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
}

seatdisplay::seatdisplay(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    init_vector(seat_displacement);

    QRegExp rxSeats("[A-Z0-9]{0,3}");
    seats = this->findChildren<QPushButton *>(rxSeats);

    connect(bookButton, SIGNAL(clicked()), this, SLOT(seatbooking()));

    connect(actionBook_Seats, SIGNAL(triggered()), this, SLOT(seatbooking()));
    actionBook_Seats->setShortcut(tr("Ctrl+B"));
    actionBook_Seats->setToolTip(tr("Book selected seats"));
    //insert connection for cancel seats function
    actionCancel_Seats->setShortcut(tr("Ctrl+C"));

    connect(actionLogin, SIGNAL(triggered()), this, SLOT(adminLogin()));
    actionLogin->setShortcut(tr("Alt+L"));
    connect(actionView_Settings, SIGNAL(triggered()), this, SLOT(showAdminPanel()));
    actionView_Settings->setShortcut(tr("Alt+S"));

    connect(actionCustomer_Login, SIGNAL(triggered()), this, SLOT(cusLogin()));
    actionCustomer_Login->setShortcut(tr("Ctrl+L"));

    connect(action_Quit, SIGNAL(triggered()), this, SLOT(close()));
    action_Quit->setShortcut(tr("Ctrl+Q"));
    action_Quit->setToolTip(tr("Quit the application"));

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

    QSqlQuery query;
    query.exec("SELECT * FROM seats WHERE disabled=1");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if((*seats[i]).objectName() == query.value(0).toString())
            {
                (*seats[i]).setDisabled(true);
            }
        }
    }
//    QSqlQuery query2;
//    query2.exec("SELECT * FROM `computing`.`events`");
//    while(query2.next())
//    {
//        booked.push_back(std::vector<QString>());

//        QString quer("SELECT * FROM bookings WHERE events_id=");
//        quer.append(query2.value(0).toString());

//        QSqlQuery query3;
//        query3.exec(quer);

//        while(query3.next())
//        {
//            int pos = query2.value(0).toInt() - 1;
//            booked[pos].push_back(query3.value(0).toString());
//        }
//    }
    disableBooked("Friday - 1900");
}

disabilityEditor::disabilityEditor(QDialog *parent) : QDialog(parent)
{
    setupUi(this);

    QRegExp rxSeats("[A-Z0-9]{0,3}");
    seats = this->findChildren<QPushButton *>(rxSeats);

    QSqlQuery query;
    query.exec("SELECT * FROM seats WHERE disabled=1");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if((*seats[i]).objectName() == query.value(0).toString())
            {
                (*seats[i]).setStyleSheet("QPushButton {background-color: qlineargradient(spread:pad, x1:0.266, y1:0, x2:0.532327, y2:1, stop:0 rgba(170, 255, 127, 255), stop:1 rgba(170, 255, 0, 255));border-radius: 4px;border-color: rgb(0, 255, 0);image: url(:/resources/wheelchair-icon.png);}QPushButton:checked {background-color: rgb(255, 170, 127);}");
            }
        }
    }

    connect(setDis, SIGNAL(clicked()), this, SLOT(setDisab()));
    connect(setNotDis, SIGNAL(clicked()), this, SLOT(setNotDisab()));
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
            customerBooked("Friday - 1900");
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
    } else if (admin) {
        admin = false;
        actionLogin->setText("Login");
        connect(actionLogin, SIGNAL(triggered()), this, SLOT(adminLogin()));
    }


}

void seatdisplay::seatbooking()
{
    this->setEnabled(false);
    booking_dialogue *dialog = new booking_dialogue;

    dialog->comboBox->clear();

    QSqlQuery query;
    QSqlQuery prices;
    QSqlRecord rec;

    for(int i = 0; i < seats.size(); ++i)
    {
        if((*seats[i]).isChecked() && (*seats[i]).isCheckable())
        {
            dialog->comboBox->addItem((*seats[i]).objectName());

            QString quer("SELECT * FROM seats WHERE seat='");
            quer.append((*seats[i]).objectName()); quer.append("'");

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

    QString day = (friRadio->isChecked())?"on Friday":"on Saturday";

    dialog->dayLabel->setText(day);

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
//        dialog->show();
        dialog->exec();
    }
    this->setEnabled(true);
    //exec waits for it to close, meaning that once we are done with the dialogue box, here the booking dialogue,
    //we can go back to this function, and re-enable the main window which at the start of the function was set
    //to disabled. using show does not do this, therefore exec is used here.
}

void seatdisplay::init_vector(std::vector<std::pair<char, int> > &vec)
{
    vec.push_back(std::make_pair('A', 4));
    vec.push_back(std::make_pair('B', 3));
    vec.push_back(std::make_pair('C', 2));
    vec.push_back(std::make_pair('D', 1));
    vec.push_back(std::make_pair('E', 0));
    vec.push_back(std::make_pair('F', 0));
    vec.push_back(std::make_pair('G', 1));
    vec.push_back(std::make_pair('H', 1));
    vec.push_back(std::make_pair('J', 1));
    vec.push_back(std::make_pair('K', 0));
    vec.push_back(std::make_pair('L', 0));
    /*
      Think this is meant to be used to tell how far two seats are from each other
      in order to show customers the closest seat with suitable properties.

      Is uneccessary, so may be removed at end.
      */
}

void seatdisplay::on_friRadio_toggled(bool checked)
{
    if(checked)
    {
        sat_clicked.clear();
        for(int i = 0; i < seats.size(); ++i)
        {
            if((*seats[i]).isChecked() && (*seats[i]).isCheckable())
            {
                sat_clicked.push_back((*seats[i]).objectName());
                (*seats[i]).setChecked(false);
            }
            for(int j = 0; j < fri_clicked.size(); ++j)
            {
                if((*seats[i]).objectName() == fri_clicked[j])
                {
                    (*seats[i]).setChecked(true);
                }
            }
        }
    }
}

void seatdisplay::on_satRadio_toggled(bool checked)
{
    if(checked)
    {
        fri_clicked.clear();
        for(int i = 0; i < seats.size(); ++i)
        {
            if((*seats[i]).isChecked() && (*seats[i]).isCheckable())
            {
                fri_clicked.push_back((*seats[i]).objectName());
                (*seats[i]).setChecked(false);
            }
            for(int j = 0; j < sat_clicked.size(); ++j)
            {
                if((*seats[i]).objectName() == sat_clicked[j])
                {
                    (*seats[i]).setChecked(true);
                }
            }
        }
    }
}

void booking_dialogue::on_okButton_clicked()
{
    QRegExp email("[a-z0-9._%+-]+@[a-z0-9.-]+[.]{1,1}[a-z]{2,4}");
    QRegExp tel_no("[0-9]{11,11}");
    QRegExp passwd("[A-Za-z0-9,./<>?;#:@~[]{}'!£$%^&*()]{10, 1000}");
    QString error;

    if(!(tel_no.exactMatch(telNo->text())))
    {
        error += "Your Telephone No. is invalid.\n";
    }
    if(!(email.exactMatch(txt_email->text())))
    {
        error += "Your eMail is invalid.\n";
    }
    if(!(passwd.exactMatch(txt_passwd->text())))
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
        int cus;
        for (int i = 0; i < (txt_email->text().length()/3); ++i) {
            if(qPrintable(txt_email->text())[i] == '@') {}
            else {
                cus *= qPrintable(txt_email->text())[i];
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
        sprintf(hex, "%x%x%x", cus, cus2, cus3); //cus-ref var filled
//END CREATING THE CUS-REF
        if(siz1.size() != 0)
        {
            QMessageBox error_box;
            error_box.setText("eMail in use.");
            error_box.exec();
        } else {
            QSqlQuery insertCus;

            QString quer("INSERT INTO customer VALUES (");
            quer.append("'"); quer.append(hex); quer.append("'"); quer.append(", ");
            quer.append("'"); quer.append(fName->text()); quer.append("'"); quer.append(", ");
            quer.append("'"); quer.append(sName->text()); quer.append("'"); quer.append(", ");
            quer.append("'"); quer.append(telNo->text()); quer.append("'"); quer.append(", ");
            quer.append("'"); quer.append(txt_email->text()); quer.append("'"); quer.append(", ");
            quer.append("'"); quer.append(txt_passwd->text()); quer.append("'"); quer.append(")");

            insertCus.exec(quer);
            std::cerr << qPrintable(quer) << "\n";
        }
    }
}

/*
  on_okButton_clicked should be redefined in last section to give user more information, i.e. use multiple 'if'
  statements to give the user an idea of what data has been duplicated (REF no. or email).
  */

void seatdisplay::on_pushButton_clicked()
{
    for(int i = 0; i < seats.size(); ++i)
    {
        (*seats[i]).setChecked(false);
    }
    fri_clicked.erase(fri_clicked.begin(), fri_clicked.end());
    sat_clicked.erase(sat_clicked.begin(), sat_clicked.end());
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
            if((*seats[i]).objectName() == query.value(0).toString())
            {
                (*seats[i]).setDisabled(!checked);
            }
        }
    }
    query.exec("SELECT * FROM seats WHERE disabled=0");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if((*seats[i]).objectName() == query.value(0).toString())
            {
                (*seats[i]).setDisabled(false);
            }
        }
    }
}

void booking_dialogue::passChecker(QString passwd) {
    float start = passStrength->value();
    float end = complexity(passwd) * 10;
    std::cerr << end << std::endl;
    float dir = (start > end)?(-1):(1);
    while (passStrength->value() != end)
    {
        passStrength->setValue(passStrength->value() + dir);
        for(int i = 0; i < 1000000; ++i){}
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
            if((*seats[i]).objectName() == query.value(0).toString())
            {
                (*seats[i]).setDisabled(true);
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
            if((*seats[i]).objectName() == query.value(0).toString())
            {
                (*seats[i]).setDisabled(true);
                (*seats[i]).setChecked(true);
                (*seats[i]).setStyleSheet("border-radius: 4; border-color: rgb(138, 138, 138); background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.448864 rgba(0, 85, 0, 255), stop:0.522727 rgba(0, 100, 0, 255));");
            }
        }
    }
}

void disabilityEditor::setDisab()
{
    QSqlQuery *query1 = new QSqlQuery;
    for(int i = 0; i < seats.size(); ++i)
    {
        if((*seats[i]).isChecked() && (*seats[i]).isCheckable())
        {
            query1->exec("UPDATE `computing`.`seats` SET disabled=1 WHERE seat='" + (*seats[i]).objectName() + "'");
        }
    }
    QSqlQuery query;
    query.exec("SELECT * FROM seats WHERE disabled=1");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if((*seats[i]).objectName() == query.value(0).toString())
            {
                (*seats[i]).setStyleSheet("QPushButton {background-color: qlineargradient(spread:pad, x1:0.266, y1:0, x2:0.532327, y2:1, stop:0 rgba(170, 255, 127, 255), stop:1 rgba(170, 255, 0, 255));border-radius: 4px;border-color: rgb(0, 255, 0);image: url(:/resources/wheelchair-icon.png);}QPushButton:checked {background-color: rgb(255, 170, 127);}");
            }
        }
    }
}

void disabilityEditor::setNotDisab()
{
    QSqlQuery *query1 = new QSqlQuery;
    for(int i = 0; i < seats.size(); ++i)
    {
        if((*seats[i]).isChecked() && (*seats[i]).isCheckable())
        {
            query1->exec("UPDATE `computing`.`seats` SET disabled=0 WHERE seat='" + (*seats[i]).objectName() + "'");
        }
    }
    QSqlQuery query;
    query.exec("SELECT * FROM seats WHERE disabled=0");
    while(query.next())
    {
        for(int i = 0; i < seats.size(); ++i)
        {
            if((*seats[i]).objectName() == query.value(0).toString())
            {
                (*seats[i]).setStyleSheet("");
            }
        }
    }
}
