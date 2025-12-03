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
	//closePolicy: Popup.CloseOnEscape
	onClosed: tNumber.text=" "
	background: Rectangle
	{
		color: "#222222"
	}

	function addNumber(text)
	{
		if(tNumber.text.length<20)
			tNumber.text+=text;
	}

	component NumberButton: DialFormButton
	{
		onClicked:
		{
			addNumber(text)
		}
	}

	ColumnLayout
	{
		anchors.fill: parent
		spacing: 10

		RowLayout
		{
			width: parent.width
			Text
			{
				id: tNumber
				Layout.fillWidth: true
				color: "#FFF"
				text: qsTr("")
			}
			Button
			{
				Layout.preferredWidth: 30
				Layout.preferredHeight: 30
				onClicked:
				{
					tNumber.text=tNumber.text.slice(0, -1);
				}
				contentItem: Text
				{
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
					text:"⌫"
				}
			}
		}
		Grid
		{
			id: gButtons
			rows: 4
			columns: 3
			spacing: 10
			NumberButton{text: "1"}
			NumberButton{text: "2"}
			NumberButton{text: "3"}
			NumberButton{text: "4"}
			NumberButton{text: "5"}
			NumberButton{text: "6"}
			NumberButton{text: "7"}
			NumberButton{text: "8"}
			NumberButton{text: "9"}
			NumberButton{text: "*"}
			NumberButton{text: "0"}
			NumberButton{text: "#"}
		}

		CallButton{id: bCall; text: "📞"; onClicked: {call(tNumber.text);close()}}
	}
}
