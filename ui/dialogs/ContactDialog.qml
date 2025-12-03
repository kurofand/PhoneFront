import QtQuick 2.15
import QtQuick.Controls
import Qt.labs.qmlmodels
import QtQuick.Layouts

Dialog
{
	x: (parent.width-width)/2
	y: (parent.height-height)/2
	width: parent.width-60;
	height: parent.height-60;
	modal: true
	padding: 20
	background: Rectangle
	{
		color:"#222222"
	}

	property string contactName: ""
	property int contactId: 0

	ColumnLayout
	{
		spacing: 10
		anchors.fill: parent

		Row
		{
			spacing: 10
			Text{text: "Name:"}
			Text{text: contactName}
		}

		Text{text: "Numbers:"}
		//TODO: figure out where did separator go after changed Column to Layout
		Rectangle{id: separator; width: parent.width-20; height: 1; Layout.alignment: Qt.AlignHCenter}
		ListView
		{
			id: lNumbers
			model: subModel
			Layout.fillWidth: true
			Layout.fillHeight: true

			delegate: Item
			{
				width: lNumbers.width
				height: 45
				Column
				{
					width: parent.width
					Text{padding:10; text: number}
					Rectangle{width: parent.width-20; height: 1; anchors.horizontalCenter: parent.horizontalCenter}
				}
				MouseArea{anchors.fill: parent; onPressAndHold: {numberMenu.index=index; numberMenu.popup()}}
			}
		}
		Button
		{
			id: bAdd
			width: 80
			height: 50
			Layout.alignment: Qt.AlignRight
			contentItem: Text
			{
				text: "Add number"
			}
			onClicked: dAddNumber.open()
		}
	}

	Menu
	{
		property int index
		id: numberMenu
		MenuItem{text: "Edit"; onTriggered: dEditNumber.open()}
		MenuItem{text: "Call"; onTriggered: {call(subModel.get(numberMenu.index).number);close()}}
		MenuItem{text: "Message"; onTriggered: {dSendMessage.number=subModel.get(numberMenu.index).number;dSendMessage.open();close()}}
		MenuItem{text: "Remove"; onTriggered: dRemoveNumber.open()}
	}

	Dialog
	{
		id: dEditNumber
		standardButtons: Dialog.Ok|Dialog.Cancel
		TextEdit
		{
			id: teNumber
			text: subModel.get(numberMenu.index).number
		}
		onAccepted:
		{
			const id=subModel.get(numberMenu.index).id
			const oldNumber=subModel.get(numberMenu.index).number
			const newNumber=teNumber.text
			if(oldNumber!==newNumber)
			{
				connector.editDb(id, "savedNumbers", "number", newNumber)
				subModel.set(numberMenu.index, {"id": id, "number":newNumber})
			}

		}
	}

	Dialog
	{
		id: dRemoveNumber
		standardButtons: Dialog.Ok|Dialog.Cancel
		Text
		{
			text: "Remove number?"
		}
		onAccepted:
		{
			connector.removeFromDb(subModel.get(numberMenu.index).id, "savedNumbers")
			subModel.remove(numberMenu.index)
		}
	}

	Dialog
	{
		id: dAddNumber
		standardButtons: Dialog.Ok|Dialog.Cancel
		ColumnLayout
		{
			anchors.fill: parent
			spacing: 10
			Text{text: "New number:"}
			TextEdit{id: teNewNumber; Layout.fillWidth: true}
		}
		onAccepted:
		{
			const num=teNewNumber.text
			for(var i=0;i<subModel.count;i++)
				if(parseInt(subModel.get(i).contactsId)===contactId&&subModel.get(i).number===num)
					return
			connector.addNumberToDb(contactId, num)
			subModel.append({"id": contactId, "number": num})
		}
	}
}
