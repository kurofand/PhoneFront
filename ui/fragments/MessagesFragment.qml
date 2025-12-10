import QtQuick 2.15
import QtQuick.Layouts
import Qt.labs.qmlmodels
import QtQuick.Controls
import "../components"

ColumnLayout
{
	property int activeMessageIndex: 0
	width: parent.width
	height: parent.height
	spacing: 10
	FragmentHeader{text: "Message history"}
	ListView
	{
		id: list
		model: mainModel
		width: window.width-cRightButtons.width-15
		height: window.height*0.6
		highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
		focus: true
		delegate: Item
		{
			width: list.width
			height: 45
			Row
			{
				anchors.fill: parent
				leftPadding: 5
				Text{anchors.top: parent.top; anchors.bottom: parent.bottom;verticalAlignment: Text.AlignVCenter;text: isReceived=="1"?"↓":"↑"}
				Text{anchors.top: parent.top; anchors.bottom: parent.bottom;verticalAlignment: Text.AlignVCenter;text: isRead=="1"?"R":"U"}
				Column
				{
					padding: 5
					Text{text: datetime}
					Text{text: number}
				}
			}
		MouseArea{anchors.fill: parent;onClicked: {list.currentIndex=index; activeMessageIndex=index; tMessage.text=mainModel.get(activeMessageIndex).msg}}
		}
	}
	Rectangle{width: parent.width; height: 1}
	Text
	{
		id: tMessage
		leftPadding: 10
		text: mainModel.get(0).msg
	}
	Row
	{
		Layout.alignment: Qt.AlignHCenter
		spacing: 10
		Button
		{
			onClicked:
			{
				let message=mainModel.get(activeMessageIndex)
				connector.setSMSStatus(message.isRead==="1"?false:true, message.id)
				mainModel.get(activeMessageIndex).isRead=message.isRead==="1"?"0":"1"
				btChangeMark.text=mainModel.get(activeMessageIndex).isRead==="1"?"Mark as unread":"Mark as read"
			}
			contentItem: Text
			{
				id: btChangeMark
				text: mainModel.get(activeMessageIndex).isRead==="1"?"Mark as unread":"Mark as read"
			}
		}
		Button
		{
			onClicked: {dSendMessage.number=mainModel.get(activeMessageIndex).number; dSendMessage.open()}
			contentItem: Text
			{
				horizontalAlignment: Text.AlignHCenter
				text: "Reply"
			}
		}
		Button
		{
			onClicked: {connector.deleteSMS(mainModel.get(activeMessageIndex).id); mainModel.remove(activeMessageIndex)}
			contentItem: Text
			{
				horizontalAlignment: Text.AlignHCenter
				text: "Delete"
			}
		}
	}
}
