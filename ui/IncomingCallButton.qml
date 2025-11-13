import QtQuick 2.15
import QtQuick.Controls

RoundButton
{
	id: button
	implicitWidth: 60
	implicitHeight: 60
	radius: 50

	property color color

	background: Rectangle
	{
		radius: 50
		color: button.color
	}

	contentItem: Text
	{
		text: button.text
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		font.pointSize: 20
	}
}
