import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qmlConnector

Window
{
    width: 350
    height: 350
    visible: true
    color: root.backgroundColor

    Item
    {
        id:root
        anchors.fill: parent
        anchors.topMargin: parent.SafeArea.margins.top
        anchors.leftMargin: parent.SafeArea.margins.left
        anchors.rightMargin: parent.SafeArea.margins.right
        anchors.bottomMargin: parent.SafeArea.margins.bottom

        readonly property color backgroundColor: "#222222"
    }

    ColumnLayout
    {
        spacing: 10

        Row
        {
            spacing: 10

            StatusText{text: "Provider:"}
            StatusText{id: tProvider; text:"PROVIDER_NAME"}
        }

        Row
        {
            spacing: 10

            StatusText{text:"Connection status:"}
            StatusText{id: tConnectionStatus; text:"CONNECTION_STATUS"}
        }

        Row
        {
            spacing: 10

            StatusText{text:"Signal strength:"}
            StatusText{id: tSignalStrength; text:"SIGNAL_STRENGTH"}
        }

        CallButton{text: "📞"; onClicked: dDial.open()}

    Row
    {
    Button
    {
        width: 50
        height: 50
        onClicked: dIncomingCall.open();
    }
    Button
    {
        width: 50
        height: 50
        onClicked: dDial.open();
    }
    }
    }

    DialDialog{id: dDial}
    IncomingCallDialog{id: dIncomingCall; objectName: "dIncomingCall"}
    CallDialog{id: dCall}
    QMLConnector{id: connector}

    /*function open()
    {
        dIncomingCall.open();
    }*/

}
