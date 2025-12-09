import QtQuick 2.15
import QtQuick.Layouts
import "../components"
import Qt.labs.qmlmodels

ColumnLayout
{
	spacing: 10
	FragmentHeader{text: "Call history"}
	ListView
	{
		id: list
		width: window.width-cRightButtons.width-15
		model: mainModel
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
