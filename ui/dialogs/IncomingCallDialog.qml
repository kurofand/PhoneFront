import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

import "../components"

Dialog
{
	id:dIncomingCall
	x: (parent.width-width)/2
	y: (parent.height-height)/2
	modal: true
	padding: 20
	closePolicy: Popup.CloseOnEscape
	background: Rectangle
	{
		color: "#222222"
	}


	function answer()
	{
		connector.answer();
		dCall.open();
		dCall.number=tIncomingNumber.text;
		dCall.runTimer=true;
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
			color: "#FFF"
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

	}
}
