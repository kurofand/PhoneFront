import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Dialog
{
	property string name: ""
	property int id: 0
	x: (parent.width-width)/2
	y: (parent.height-height)/2
	width: 250
	modal: true
	padding: 20
	onClosed: {id=0;name=""}
	background: Rectangle
	{
		color: "#222222"
	}

	ColumnLayout
	{
		anchors.fill: parent
		spacing: 10

		RowLayout
		{
			spacing: 5
			Layout.fillWidth: true
			Text
			{
				text: "Name:"
			}
			TextInput
			{
				id: tiName
				Layout.fillWidth: true
				text: name
			}
		}

		Button
		{
			Layout.alignment: Qt.AlignHCenter
			onClicked:
			{
				mainModel.clear()
				if(id==0)
					connector.addContact(tiName.text)
				else
					connector.editContact(id, tiName.text)
				close()
			}
			enabled: tiName.text!==name
			contentItem: Text
			{
				horizontalAlignment: Text.AlignHCenter
				text: "Save"
			}
		}
	}
}
