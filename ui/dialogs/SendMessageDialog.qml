import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Dialog
{
	property string number: ""
	id: dialogroot
	x: (parent.width-width)/2
	y: (parent.height-height)/2
	width: parent.width-100
	height: parent.height-100
	modal: true
	padding: 20
	onClosed:
	{
		tiNumber.text=""
		teMessage.text=""
		number=""
	}
	onOpened: tiNumber.text=number

	ColumnLayout
	{
		anchors.fill: parent
		spacing: 10

		FragmentHeader{text: "Send SMS"}

		RowLayout
		{
			spacing: 5
			Layout.fillWidth: true
			Text
			{
				id: tNumber
				text:"To:"
			}
			TextInput
			{
				id: tiNumber
				text: number
				maximumLength: 20
				Layout.fillWidth: true
			}
		}
		Text
		{
			text: "Message:"
		}
		TextEdit
		{
			id: teMessage
			Layout.fillWidth: true
			Layout.fillHeight: true
			wrapMode: TextEdit.Wrap
			//limit message length to 255. if not ui goes wild
			//Alarm! max message length for PDU UCS2 is 140 bytes or 70 symbols, so
			//TODO: set max message length to 70 or split message to several on send event
			onTextChanged:
			{
				if(teMessage.text.length>255)
					teMessage.text=teMessage.text.slice(0, -1)
			}
		}
		Button
		{
			id: bSend
			width: 100
			height: 75
			Layout.alignment: Qt.AlignHCenter
			contentItem: Text
			{
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				text: "send"
			}
			onClicked: connector.sendSMS(tiNumber.text, teMessage.text)
		}
	}
}
