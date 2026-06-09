import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.qmlmodels
import "../components"

ColumnLayout
{
	width: parent.width
	height: parent.height
	spacing: 10
	FragmentHeader{text: "Call history"}
	ListView
	{
		id: list
		width: window.width-cRightButtons.width-15
		height: parent.height
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
				Text{anchors.top: parent.top; anchors.bottom: parent.bottom;verticalAlignment: Text.AlignVCenter;text: isReceived=="1"?"↓":"↑"; color: activeTheme.subFont}
				Column
				{
					padding: 5
					Text{text: datetime; color: activeTheme.subFont}
					Text{text: typeof name!="undefined"&&name!=""?name:number; color: activeTheme.subFont}
				}
				Text{anchors.top: parent.top;anchors.bottom: parent.bottom;verticalAlignment: Text.AlignVCenter;leftPadding: 5;text: "Call length:"; color: activeTheme.subFont}
				Text{anchors.top: parent.top;anchors.bottom: parent.bottom;verticalAlignment: Text.AlignVCenter;text: length; color: activeTheme.subFont}
			}
		MouseArea{anchors.fill: parent;onClicked: list.currentIndex=index;
			onPressAndHold:{callMenu.number=mainModel.get(index).number;callMenu.popup()}}
		}
	}
	Menu
	{
		property string number
		id: callMenu
		MenuItem{text: "Call"; onTriggered: call(callMenu.number)}
		MenuItem{text: "Message"; onTriggered: {dSendMessage.number=callMenu.number;dSendMessage.open()}}
	}
}
