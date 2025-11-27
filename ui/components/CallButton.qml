import QtQuick 2.15
import QtQuick.Controls

RoundButton
{
	id: button
	implicitWidth: 180
	implicitHeight: 50
	radius: 50

	background: Rectangle
	{
		radius: 50
		color: button.down?"#050":"#0F0"
	}

	contentItem: Text
	{
		text: button.text
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		font.pointSize: 20
	}
}

