import QtQuick 2.15
import QtQuick.Controls
import Qt.labs.qmlmodels

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

	Column
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
		Rectangle{width: parent.width-20; height: 1; anchors.horizontalCenter: parent.horizontalCenter}
		ListView
		{
			id: lNumbers
			model: subModel
			width: parent.width
			height: parent.height-50;
			/*anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: parent.bottom7*/

			delegate: Item
			{
				width: lNumbers.width
				height: 45
				Column
				{
					width: parent.width
					//Rectangle{width: parent.width-20; height: 1; anchors.horizontalCenter: parent.horizontalCenter}
					Text{padding:10; text: number}
					Rectangle{width: parent.width-20; height: 1; anchors.horizontalCenter: parent.horizontalCenter}
				}
			}
		}
	}
}
