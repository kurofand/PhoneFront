import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Dialog
{
	x: (parent.width-width)/2
	y: (parent.height-height)/2
	modal: true
	padding: 20
	closePolicy: Popup.CloseOnEscape
	background: Rectangle
	{
		color: "#222222"
	}

	property string number: "080xxxxxx56"
	property string sec: "00"
	property string min: "00"
	property bool runTimer: false
	property bool connected: false

	ColumnLayout
	{
		anchors.fill: parent
		spacing: 10

		RowLayout
		{
			Text
			{
				id: tCallStatus
				Layout.alignment: Qt.AlignHCenter
				font.pointSize: 15
				text: connected?"Connected":"Calling..."
			}
			Text
			{
				id: tCallTime
				Layout.alignment: Qt.AlignHCenter
				font.pointSize: 15
				text: min+":"+sec
			}
		}
		Text
		{
			id: tNumber
			Layout.alignment: Qt.AlignHCenter
			font.pointSize: 20
			text: number
		}
		RoundButton
		{
			id: button
			implicitWidth: 180
			implicitHeight: 50
			radius: 50

			background: Rectangle
			{
				radius: 50
				color: button.down?"#500":"#F00"
			}

			contentItem: Text
			{
				text: "🕿"
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				font.pointSize: 20
			}

			onClicked:
			{
				connector.reject();
				runTimer=false;
				connected=false;
				tiCallTime.stop();
				sec="00";
				min="00";
				close();
			}
		}
	}

	Timer
	{
		id: tiCallTime
		running: runTimer; repeat: true
		onTriggered:
		{
			sec=prepareVal(++sec);
			if(sec==60)
			{
				sec="00";
				min=prepareVal(++min);
			}

		}
	}

	function prepareVal(val)
	{
		if(val<10)
			val="0"+val;
		return val;
	}
}
