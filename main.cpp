#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>

#include <thread>

#include "qmlconnector.hpp"
#include "SerialPort/Phone.hpp"
#include "sqliteconnector/sqliteclient.cpp"
#include "Log/log.hpp"

#include "cli/cli.hpp"

void listen(Phone *phone)
{
    const size_t bufSize=1024;
    for(;;)
    {
        char buf[bufSize];
        int read=phone->port()->readFromPort(buf, bufSize);
        if(read>0)
        {
            //new read logic:
            //get chunk of data
            //split it by new line
            //send to parser
            std::string chunk(std::string(buf, read)), resp, line;
            Log(LogLevel::Debug)<<"Chunk of data from modem: ["<<chunk<<"]";
            std::stringstream sChunk(chunk);
            bool readNextLine=false;
            while(getline(sChunk, line, '\n'))
            {
                //most responses start with + and contain only a single line
                if(line[0]=='+'||
                    line.find("RING")!=std::string::npos||
                    line.find("VOICE CALL")!=std::string::npos||
                    line.find("MISSED_CALL")!=std::string::npos||
                    (readNextLine&&!line.empty()))
                {
                    resp+=line;
                    //CMGRD is the only exception for now - it has information in first line and pdu in second
                    //so parser should receive both lines
                    if(line.find("CMGRD")!=std::string::npos)
                    {
                        readNextLine=true;
                        resp.push_back('\n');
                        continue;
                    }
                    phone->parseResponse(resp);
                    resp="";
                    readNextLine=false;
                }

            }
            //potentional bug with cut response on response larger than buf size.
            //let's hope that does not happen
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
        phone->requestNumber();
        phone->requestSignalStrength();
        phone->requestConnectionStatus();
        phone->requestOperatorInfo();
        phone->setIdentification();
        auto *dbClient=SqliteClient::instance();
        dbClient->connect();
        auto *queryRes=new std::vector<std::unordered_map<std::string, std::string>>();
        auto *contacts=new std::unordered_map<std::string, std::string>();
        dbClient->executeQuery("SELECT number, name FROM savedNumbers INNER JOIN contacts ON contactsId=contacts.id", queryRes);
        for(const auto &row: *queryRes)
            contacts->insert(std::pair<std::string, std::string>{row.at("number"), row.at("name")});

        queryRes->clear();
        auto *window=engine.rootObjects().first();
        auto *themeObj=window->findChild<QObject*>("activeTheme");
        dbClient->executeQuery("SELECT mainBackground, btnBackground, mainFont, subFont, separatorColor FROM themes INNER JOIN settings ON settings.val=themes.id WHERE settings.name=\"Theme\"", queryRes);
        for(auto const& [key, val]: queryRes->at(0))
        {
            std::string colorCode="#"+val;
            themeObj->setProperty(key.c_str(), colorCode.c_str());
        }
        delete queryRes;
        phone->contacts(contacts);

    }
    else
        Log(LogLevel::Error)<<"Error on opening port";
//TODO: for ui tests, remove later

    Cli cli(&s);
    cli.getNumInfo();
    return app.exec();
}

#include "main.moc"
