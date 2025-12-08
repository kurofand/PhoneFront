#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>

#include <thread>
#include <iostream>

//#include <stdio.h>

//#include "qmlconnector.hpp"
#include "SerialPort/Phone.cpp"
#include "sqliteconnector/sqliteclient.cpp"
#include "SerialPort/sms.hpp"


class QMLConnector: public QQuickItem
{
    Q_OBJECT
    public:
        QMLConnector(QQuickItem *parent=nullptr): QQuickItem(parent){}
        Q_INVOKABLE void answer()
        {
            auto *phone=Phone::getInstance();
            if(phone->connectionAlive())
                phone->answer();
        }
        Q_INVOKABLE void reject()
        {
            auto *phone=Phone::getInstance();
            if(phone->connectionAlive())
                phone->disconnect();
        };

        Q_INVOKABLE void call(QString num)
        {
            auto *phone=Phone::getInstance();
            if(phone->connectionAlive())
                phone->call(std::move(num.toStdString()));
        }

        Q_INVOKABLE void updateStatus()
        {
            auto *phone=Phone::getInstance();
            if(phone->port())
            {
                //Not sure that it is necessary to update operator info
                /*phone->requestOperatorInfo();
                sleep(1);*/
                phone->requestConnectionStatus();
                sleep(1);
                phone->requestSignalStrength();
            }
        }

        Q_INVOKABLE void getCalls()
        {
            getList("SELECT * FROM calls");
        }

        Q_INVOKABLE void getMessages()
        {
            getList("SELECT * FROM messages");
        }

        Q_INVOKABLE void getContacts()
        {
            getList("SELECT id, name FROM contacts");
        }

        Q_INVOKABLE void getNumbers(QString id)
        {
            std::string query;
            query+="SELECT id, contactsId, number FROM savedNumbers WHERE contactsId=";
            query+=id.toStdString();
            getList(query.c_str(), "appendToSubList");
        }

        Q_INVOKABLE void editDb(QString id, QString table, QString colName, QString newVal)
        {
            std::string query="UPDATE "+table.toStdString()+" SET "+colName.toStdString()+"='"+newVal.toStdString()+"' WHERE id="+id.toStdString();
            auto *db=SqliteClient::instance();
            db->executeQuery(query.c_str());
        }

        Q_INVOKABLE void removeFromDb(QString id, QString table)
        {
            std::string query="DELETE FROM "+table.toStdString()+" WHERE id="+id.toStdString();
            auto *db=SqliteClient::instance();
            db->executeQuery(query.c_str());
        }

        Q_INVOKABLE void addNumberToDb(QString contactsId, QString number)
        {
            std::string query="INSERT INTO savedNumbers(contactsId, number) VALUES("+contactsId.toStdString()+",'"+number.toStdString()+"')";
            auto *db=SqliteClient::instance();
            db->executeQuery(query.c_str());
        }

        Q_INVOKABLE void sendSMS(QString number, QString msg)
        {
            if(Phone::getInstance()->port())
            {
                auto *sms=new Sms();
                sms->number(std::move(number.toStdString()));
                sms->message(std::move(msg.toStdString()));
                sms->toPdu();
                Phone::getInstance()->sendSms(sms);
            //sms instance will be deleted after send, no need to free here
            }

        }

    private:
        void getList(const char* query, const char* qmlFunc="appendToList")
        {
            auto *db=SqliteClient::instance();
            auto *queryRes=new std::vector<std::unordered_map<std::string, std::string>>();
            db->executeQuery(query, queryRes);
            if(!queryRes->empty())
            {
                auto *root=window();
                for(const auto &row: *queryRes)
                {
                    QVariantMap e;
                    for(const auto &[key, val]: row)
                        e.insert(QString::fromStdString(key), QString::fromStdString(val));
                    QMetaObject::invokeMethod(root, qmlFunc, Q_ARG(QVariant, QVariant::fromValue(e)));
                }
            }
            delete queryRes;
        }

    /*signals:
        static void showIncomingForm(std::string &num);*/


};

void listen(Phone *phone)
{
    const size_t bufSize=1024;
    for(;;)
    {
        char buf[bufSize];
        int read=phone->port()->readFromPort(buf, bufSize);
        if(read>0)
        {
            std::string resp(buf, read);
            phone->parseResponse(resp);
            memset(buf, 0, bufSize);
        }
        else if(read<0)
        {
            int e=errno;
            std::cout<<"An error occured on read: "<<strerror(e)<<std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<QMLConnector>("qmlConnector", 1, 0, "QMLConnector");

    const QUrl url(QStringLiteral("qrc:/PhoneFront/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url, &engine](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);

        },
        Qt::QueuedConnection);
    engine.load(url);
    SerialPort port{"/dev/ttyUSB2", 9600};
    if(port.open())
    {
        //Phone phone(&port, &engine);
        //Phone phone(&port);
        //because c++ object can not linked with qml(or I do not how),
        //I made a Phone class a singleton
        //There is initialization
        Phone *phone=Phone::getInstance();
        phone->port(&port);
        phone->engine(&engine);
        std::thread tListen(listen, phone);
        tListen.detach();
        phone->setVoiceHangupControl();
        sleep(1);
        phone->requestNumber();
        sleep(1);
        phone->requestSignalStrength();
        sleep(1);
        phone->requestConnectionStatus();
        sleep(1);
        phone->requestOperatorInfo();
        auto *dbClient=SqliteClient::instance();
        dbClient->connect();

        auto *queryRes=new std::vector<std::unordered_map<std::string, std::string>>();
        auto *contacts=new std::unordered_map<std::string, std::string>();
        dbClient->executeQuery("SELECT number, name FROM savedNumbers INNER JOIN contacts ON contactsId=contacts.id", queryRes);
        for(const auto &row: *queryRes)
            for(const auto &[key, val]: row)
                contacts->at(key)=val;
        delete queryRes;
        phone->contacts(contacts);

    }
    else
        std::cout<<"Error on open port!"<<std::endl;
//TODO: for ui tests, remove later
    auto *dbClient=SqliteClient::instance();
    dbClient->connect();

    return app.exec();
}

#include "main.moc"
