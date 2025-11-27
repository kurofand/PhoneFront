import QtQuick 2.15
import Qt.labs.qmlmodels

Item
{
	property ListModel model: nil

	ListView
	{
		id: list
		model: callModel
		anchors.fill: parent
		highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
		focus: true
		delegate: Item
		{
			width: list.width
			height: 45
			Column
			{
				padding: 5
				Text{text: datetime}
				Text{text: number}
			}
		MouseArea{anchors.fill: parent;onClicked: list.currentIndex=index}
		}
	}
}
