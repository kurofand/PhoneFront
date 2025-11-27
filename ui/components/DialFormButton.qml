import QtQuick 2.15
import QtQuick.Controls

RoundButton
{
	id: button
	implicitWidth: 50
	implicitHeight: 50
	radius: 8

	background: Rectangle
	{
		radius: 8
		border.color: "#A9A9A9"
		color: button.down?"#DDD":"#FFF"
	}

	contentItem: Text
	{
		text: button.text
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		font.pointSize: 16
		font.bold: true
	}
}
