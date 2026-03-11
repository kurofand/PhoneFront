#ifndef QMLCONNECTOR_HPP
#define QMLCONNECTOR_HPP

#include <QQuickItem>

class QMLConnector: public QQuickItem
{
    Q_OBJECT
    public:
        QMLConnector(QQuickItem *parent=nullptr): QQuickItem(parent){};
        Q_INVOKABLE void answer();
        Q_INVOKABLE void reject();
        Q_INVOKABLE void call(QString num);
        Q_INVOKABLE void updateStatus();
        Q_INVOKABLE void getCalls();
        Q_INVOKABLE void getMessages();
        Q_INVOKABLE void getContacts();
        Q_INVOKABLE void getNumbers(QString id);
        Q_INVOKABLE void editDb(QString id, QString table ,QString colName, QString newVal);
        Q_INVOKABLE void removeFromDb(QString id, QString table);
        Q_INVOKABLE void addNumberToDb(QString contactsId, QString number);
        Q_INVOKABLE void sendSMS(QString number, QString msg);
        Q_INVOKABLE void setSMSStatus(bool read, QString id);
        Q_INVOKABLE void deleteSMS(QString id="");
        Q_INVOKABLE void addContact(QString name);
        Q_INVOKABLE void editContact(int id, QString name);
        Q_INVOKABLE void sendVoiceMenu(char c);

    private:
        void getList(const char* query, const char* qmlFunc="appendToList");
};

#endif // QMLCONNECTOR_HPP
