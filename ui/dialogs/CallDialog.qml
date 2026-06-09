import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Dialog
{
	x: (parent.width-width)/2
	y: (parent.height-height)/2
	modal: true
	padding: 20
	closePolicy: Popup.CloseOnEscape
	background: Rectangle
	{
		color: activeTheme.mainBackground
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
				color: activeTheme.mainFont
			}
			Text
			{
				id: tCallTime
				Layout.alignment: Qt.AlignHCenter
				font.pointSize: 15
				text: min+":"+sec
				color: activeTheme.mainFont
			}
		}
		Text
		{
			id: tNumber
			Layout.alignment: Qt.AlignHCenter
			font.pointSize: 20
			text: number
			color: activeTheme.mainFont
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

		RoundButton
		{
			implicitWidth: 140
			implicitHeight: 30
			radius: 50
			Layout.alignment: Qt.AlignHCenter
			//in some cases i got VOICE CALL: BEGIN only after AT+VTS so buttons should be visible all the time
			//visible: connected
			background: Rectangle
			{
				radius: 50
				color: "#333"
			}
			contentItem: Text
			{
				text: gVoiceMenuButtons.visible?"△":"▽"
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				font.pointSize: 20
			}
			onClicked: gVoiceMenuButtons.visible=!gVoiceMenuButtons.visible
		}

		component VoiceMenuButton: DialFormButton
		{
			onClicked: connector.sendVoiceMenu(text)
		}

		Grid
		{
			id: gVoiceMenuButtons
			Layout.alignment: Qt.AlignHCenter
			rows: 4
			columns: 3
			spacing: 10
			visible: false
			VoiceMenuButton{text: "1"}
			VoiceMenuButton{text: "2"}
			VoiceMenuButton{text: "3"}
			VoiceMenuButton{text: "4"}
			VoiceMenuButton{text: "5"}
			VoiceMenuButton{text: "6"}
			VoiceMenuButton{text: "7"}
			VoiceMenuButton{text: "8"}
			VoiceMenuButton{text: "9"}
			VoiceMenuButton{text: "*"}
			VoiceMenuButton{text: "0"}
			VoiceMenuButton{text: "#"}
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
