import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import qmlConnector
import "./ui/components"
import "./ui/dialogs"

Window
{
    width: 400
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

    ColumnLayout
    {

        spacing: 10

        MarginRow
        {
            Layout.topMargin: 5

            StatusText{text: "Operator:"}
            StatusText{id: tOperator; objectName:"tOperator"; text:"Failed to get an operator name"}
        }

        MarginRow
        {
            StatusText{text:"Connection status:"}
            StatusText{id: tConnectionStatus; objectName:"tConnectionStatus"; text:"CONNECTION_STATUS"}
        }

        MarginRow
        {
            StatusText{text:"Signal strength:"}
            StatusText{id: tSignalStrength; objectName:"tSignalStrength"; text:"SIGNAL_STRENGTH"}
        }

        MarginRow
        {
            Button
            {
                width: 50
                height: 50
                enabled: !root.updatingConnectionStatus&&!root.updatingSignalStrength
                onClicked: connector.updateStatus()
                contentItem: Text
                {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    text: "⟳"
                }
            }
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
    CallDialog{id: dCall; objectName: "dCall"}
    QMLConnector{id: connector}

    Timer
    {
        interval: 1*60*1000; running: false; repeat: true;
        onTriggered:
        {
            connector.updateStatus();
        }
    }

}
