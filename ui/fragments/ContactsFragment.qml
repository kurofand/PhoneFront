import QtQuick 2.15
import Qt.labs.qmlmodels
import QtQuick.Layouts
import "../components"

ColumnLayout
{
	width: parent.width
	height: parent.height
	spacing: 10
	FragmentHeader{text: "Contacts"}
	ListView
	{
		id: list
		width: window.width-cRightButtons.width-15
		model: mainModel
		delegate: Item
		{
			width: list.width
			height: 45
			Column
			{
				width: parent.width
				Rectangle{width: parent.width-20; height: 1; anchors.horizontalCenter: parent.horizontalCenter}
				Text{padding:10; text: name}
				Rectangle{width: parent.width-20; height: 1; anchors.horizontalCenter: parent.horizontalCenter}
			}
			MouseArea{anchors.fill: parent; onClicked:{dContact.contactName=name;dContact.contactId=id;getContactNumbers(id);dContact.open()}}
		}
	}



}
