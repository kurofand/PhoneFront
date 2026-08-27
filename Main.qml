import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels

import qmlConnector
import "./ui/components"
import "./ui/dialogs"
import "./ui/fragments"

ApplicationWindow
{
    id:window
    width: 550
    height: 450
    visible: true
    //color: root.backgroundColor
    color: activeTheme.mainBackground

    QtObject
    {
        id: activeTheme
        objectName: "activeTheme"
        property color mainBackground
        property color mainFont
        property color subFont
        property color btnBackground
        property color separatorColor
    }

    Item
    {
        id:root
        objectName: "root"
        anchors.fill: parent
        anchors.topMargin: parent.SafeArea.margins.top
        anchors.leftMargin: parent.SafeArea.margins.left
        anchors.rightMargin: parent.SafeArea.margins.right
        anchors.bottomMargin: parent.SafeArea.margins.bottom

        property bool updatingConnectionStatus: false
        property bool updatingSignalStrength: false

        function setUpdatingConnectionStatus(val){updatingConnectionStatus=val}
        function setUpdatingSignalStrength(val){updatingSignalStrength=val}
    }

    RowLayout
    {
        anchors.fill: parent
        spacing: 10
        Loader
        {
            id: lContainer
            width: window.width-cRightButtons.width
            height: window.height
            Layout.alignment: Qt.AlignLeft|Qt.AlignTop
            sourceComponent: cStatus
        }

        Column
        {
            id: cRightButtons
            Layout.alignment: Qt.AlignVCenter
            spacing: 5

            Button
            {
                width: 50
                height: 50
                onClicked: {lContainer.sourceComponent=cStatus}
                background: DefaultButtonBackground{}
            }

            Button
            {
                width: 50
                height: 50
                onClicked: {clearModel();connector.getCalls();lContainer.sourceComponent=cCalls;lContainer.width=window.width-cRightButtons.width}
                background: DefaultButtonBackground{}
            }

            Button
            {
                width: 50
                height: 50
                onClicked: {clearModel();connector.getMessages();lContainer.sourceComponent=cMessages;}
                background: DefaultButtonBackground{}
            }
            Button
            {
                width: 50
                height: 50
                onClicked: {clearModel();connector.getContacts();lContainer.sourceComponent=cContacts;}
                background: DefaultButtonBackground{}
            }
            Button
            {
                width: 50
                height: 50
                onClicked: {dSendMessage.open()}
                background: DefaultButtonBackground{}
            }

            CallButton{width: 50; height: 50; text: "📞"; onClicked: dDial.open()}
        }
    }

    DialDialog{id: dDial}
    IncomingCallDialog{id: dIncomingCall; objectName: "dIncomingCall"}
    CallDialog{id: dCall; objectName: "dCall"}
    ContactDialog{id: dContact; objectName: "dContact"}
    SendMessageDialog{id: dSendMessage; objectName: "dSendMessage"}
    AddEditContactDialog{id: dAddEditContact}
    QMLConnector{id: connector}

    Component{id: cStatus; StatusFragment{}}

    Component
    {
        id: cCalls
        CallsFragment{id:fCalls}
    }

    Component
    {
        id: cMessages
        MessagesFragment{}
    }

    Component{id: cContacts; ContactsFragment{}}

    ListModel{id: mainModel}
    ListModel{id: subModel}

    Timer
    {
        interval: 1*60*1000; running: false; repeat: true;
        onTriggered:
        {
            connector.updateStatus();
        }
    }

    function clearModel()
    {
        mainModel.clear();
    }

    function appendToList(row)
    {
        mainModel.append(row);
    }

    function appendToSubList(row)
    {
        subModel.append(row);
    }

    function getContactNumbers(id)
    {
        subModel.clear();
        connector.getNumbers(id);
    }

    function call(num)
    {
        connector.call(num);
        dCall.number=num;
        dCall.open();
    }

    function updateSignalStrengthVal(val)
    {
        if("signalStrength" in lContainer.item)
            lContainer.item.signalStrength=val
    }

    function updateConnectionStatusVal(val)
    {
        if("connectionStatus" in lContainer.item)
            lContainer.item.connectionStatus=val
    }

    function updateOperatorNameVal(val)
    {
        if("operatorName" in lContainer.item)
            lContainer.item.operatorName=val
    }
}
