import QtQuick 2.15
import Qt.labs.qmlmodels
import "../components"

Item
{
	ListView
	{
		id: list
		model: mainModel
		anchors.fill: parent
		highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
		focus: true
		header: FragmentHeader{text: "Message history"}
		delegate: Item
		{
			width: list.width
			height: 45
			Row
			{
				anchors.fill: parent
				leftPadding: 5
				Text{anchors.top: parent.top; anchors.bottom: parent.bottom;verticalAlignment: Text.AlignVCenter;text: isReceived=="1"?"↓":"↑"}
				Text{anchors.top: parent.top; anchors.bottom: parent.bottom;verticalAlignment: Text.AlignVCenter;text: isRead=="1"?String.fromCodePoint(0xF32E):String.fromCodePoint(0x2709)}
				Column
				{
					padding: 5
					Text{text: datetime}
					Text{text: number}
				}
			}
		MouseArea{anchors.fill: parent;onClicked: list.currentIndex=index}
		}
	}
}
