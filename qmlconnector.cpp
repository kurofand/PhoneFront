#include "qmlconnector.hpp"

#include <QQuickWindow>

#include "SerialPort/Phone.hpp"
#include "sqliteconnector/sqliteclient.hpp"

void QMLConnector::answer()
{
    auto *phone=Phone::getInstance();
    if(phone->connectionAlive())
        phone->answer();
}

void QMLConnector::reject()
{
    auto *phone=Phone::getInstance();
    if(phone->connectionAlive())
        phone->disconnect();
}

void QMLConnector::call(QString num)
{
    auto *phone=Phone::getInstance();
    if(phone->connectionAlive())
        phone->call(std::move(num.toStdString()));
}

void QMLConnector::updateStatus()
{
    auto *phone=Phone::getInstance();
    if(phone->port())
    {
        phone->requestConnectionStatus();
        sleep(1);
        phone->requestSignalStrength();
    }
}

void QMLConnector::getCalls()
{
    getList("SELECT * FROM calls");
}

void QMLConnector::getMessages()
{
    getList("SELECT * FROM sms");
}

void QMLConnector::getContacts()
{
    getList("SELECT id, name FROM contacts");
}

void QMLConnector::getNumbers(QString id)
{
    std::string query;
    query+="SELECT id, contactsId, number FROM savedNumbers WHERE contactsId=";
    query+=id.toStdString();
    getList(query.c_str(), "appendToSubList");
}

void QMLConnector::editDb(QString id, QString table, QString colName, QString newVal)
{
    std::string query="UPDATE "+table.toStdString()+" SET "+colName.toStdString()+"='"+newVal.toStdString()+"' WHERE id="+id.toStdString();
    auto *db=SqliteClient::instance();
    db->executeQuery(query.c_str());
}

void QMLConnector::removeFromDb(QString id, QString table)
{
    std::string query="DELETE FROM "+table.toStdString()+" WHERE id="+id.toStdString();
    auto *db=SqliteClient::instance();
    db->executeQuery(query.c_str());
}

void QMLConnector::addNumberToDb(QString contactsId, QString number)
{
    std::string query="INSERT INTO savedNumbers(contactsId, number) VALUES("+contactsId.toStdString()+",'"+number.toStdString()+"')";
    auto *db=SqliteClient::instance();
    db->executeQuery(query.c_str());
}

void QMLConnector::sendSMS(QString number, QString msg)
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

void QMLConnector::setSMSStatus(bool read, QString id)
{
    std::string query="UPDATE sms SET isRead=";
    query+=read+'0';
    query+=" WHERE id="+id.toStdString();
    SqliteClient::instance()->executeQuery(query.c_str());
}

void QMLConnector::deleteSMS(QString id)
{
    std::string query="DELETE FROM sms";
    if(id!="")
        query+=" WHERE id="+id.toStdString();
    SqliteClient::instance()->executeQuery(query.c_str());
}

void QMLConnector::addContact(QString name)
{
    std::string query="INSERT INTO contacts(name) VALUES(\""+name.toStdString()+"\")";
    SqliteClient::instance()->executeQuery(query.c_str());
    getContacts();
}

void QMLConnector::editContact(int id, QString name)
{
    std::string query="UPDATE contacts SET name=\""+name.toStdString()+"\" WHERE id="+std::to_string(id);
    SqliteClient::instance()->executeQuery(query.c_str());
    getContacts();
}

void QMLConnector::getList(const char* query, const char* qmlFunc)
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
            if(row.find("number")!=row.end()&&Phone::getInstance()->contacts()!=nullptr)
                e.insert("name", QString::fromStdString(Phone::getInstance()->contacts()->at(row.at("number"))));
            QMetaObject::invokeMethod(root, qmlFunc, Q_ARG(QVariant, QVariant::fromValue(e)));
        }
    }
    delete queryRes;
}

