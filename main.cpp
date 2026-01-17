#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>

#include <thread>

#include "qmlconnector.hpp"
#include "SerialPort/Phone.hpp"
#include "sqliteconnector/sqliteclient.cpp"
#include "Log/log.hpp"

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
            Log(LogLevel::Error)<<"Error on read from port: "<<strerror(e);
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
    Log(LogLevel::Info)<<"Start application";
    SerialPort port{"/dev/ttyUSB2", 9600};
    if(port.open())
    {
        Log(LogLevel::Info)<<"Port opened";
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
            contacts->insert(std::pair<std::string, std::string>{row.at("number"), row.at("name")});
        delete queryRes;
        phone->contacts(contacts);

    }
    else
        Log(LogLevel::Error)<<"Error on opening port";
//TODO: for ui tests, remove later
    /*auto *dbClient=SqliteClient::instance();
    dbClient->connect();*/

    return app.exec();
}

#include "main.moc"
