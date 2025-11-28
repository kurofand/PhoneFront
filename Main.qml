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
    width: 450
    height: 450
    visible: true
    color: root.backgroundColor

    Item
    {
        id:root
        objectName: "root"
        anchors.fill: parent
        anchors.topMargin: parent.SafeArea.margins.top
        anchors.leftMargin: parent.SafeArea.margins.left
        anchors.rightMargin: parent.SafeArea.margins.right
        anchors.bottomMargin: parent.SafeArea.margins.bottom

        readonly property color backgroundColor: "#222222"
        property bool updatingConnectionStatus: false
        property bool updatingSignalStrength: false
    }

    RowLayout
    {
        anchors.fill: parent
        spacing: 10
        Loader
        {
            id: lContainer
            height: parent.height
            Layout.alignment: Qt.AlignLeft|Qt.AlignTop
            sourceComponent: cStatus
        }

        Column
        {
            Layout.alignment: Qt.AlignVCenter
            spacing: 5

            Button
            {
                width: 50
                height: 50
                onClicked: {lContainer.sourceComponent=cStatus}
            }

            Button
            {
                width: 50
                height: 50
                onClicked: {lContainer.sourceComponent=cCalls;clearModel();connector.getCalls()}
            }

            Button
            {
                width: 50
                height: 50
                onClicked: {lContainer.sourceComponent=cMessages;clearModel();connector.getMessages()}
            }
            CallButton{width: 50; height: 50; text: "📞"; onClicked: dDial.open()}
        }
    }

    DialDialog{id: dDial}
    IncomingCallDialog{id: dIncomingCall; objectName: "dIncomingCall"}
    CallDialog{id: dCall; objectName: "dCall"}
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

    ListModel{id: mainModel}

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

}
