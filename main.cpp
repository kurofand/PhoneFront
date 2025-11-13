#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <thread>
#include <iostream>

//#include "qmlconnector.hpp"
#include "SerialPort/Phone.cpp"


class QMLConnector: public QObject
{
    Q_OBJECT
    public:
        QMLConnector(QObject *parent=nullptr){}
        Q_INVOKABLE void answer()
        {
            auto *phone=Phone::getInstance();
            if(phone->port())
                phone->answer();
        }
        Q_INVOKABLE void reject()
        {
            auto *phone=Phone::getInstance();
            if(phone->port())
                phone->disconnect();
        };

        Q_INVOKABLE void call(QString num)
        {
            auto *phone=Phone::getInstance();
            if(phone->port())
                phone->call(std::move(num.toStdString()));
        }
    private:

    /*signals:
        static void showIncomingForm(std::string &num);*/


};

void listen(Phone *phone)
{
    const size_t bufSize=255;
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
        [url](QObject *obj, const QUrl &objUrl) {
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
        phone->requestNumber();
        sleep(1);
        phone->requestSignalStrength();
        sleep(1);
        phone->requestConnectionStatus();
        sleep(1);
        phone->requestOperatorInfo();
    }
    else
        std::cout<<"Error on open port!"<<std::endl;

    /*auto *window=engine.rootObjects().first();
    auto *wIncomingCall=window->findChild<QObject*>("oIncomingCall");
    if(wIncomingCall)
        wIncomingCall->setProperty("visible", true);*/

    return app.exec();
}

#include "main.moc"
