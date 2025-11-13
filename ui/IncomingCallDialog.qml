import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

//import qmlConnector

Dialog
{
	id:dIncomingCall
	x: (parent.width-width)/2
	y: (parent.height-height)/2
	modal: true
	padding: 20
	closePolicy: Popup.CloseOnEscape


	function answer()
	{
		connector.answer();
		dCall.open();
		dCall.number=tIncomingNumber.text;
		close();
	}

	function reject()
	{
		connector.reject();
		close();
	}

	contentItem:
		ColumnLayout
	{
		spacing: 10
		anchors.fill: parent
		Text
		{
			Layout.alignment: Qt.AlignHCenter
			font.pointSize: 20
			text: "Incoming call"
		}
		Text
		{
			id: tIncomingNumber
			objectName: "tIncomingNumber"
			Layout.alignment: Qt.AlignHCenter
			font.pointSize: 20
			color: "#FFF"
			text: "080xxxx6256"
		}

		Row
		{
			Layout.alignment: Qt.AlignHCenter
			spacing: 30
			IncomingCallButton{id: bAnswer; text: "📞"; color: this.down?"#050":"#0F0"; onClicked: answer()}
			IncomingCallButton{id: bHangUp;text: "🕿"; color: this.down?"#500":"#F00"; onClicked: reject()}
		}

		//QMLConnector{id: connector}
	}
}
