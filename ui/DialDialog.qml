import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

//import qmlConnector

Dialog
{
	x: (parent.width-width)/2
	y: (parent.height-height)/2
	modal: true
	padding: 20
	//closePolicy: Popup.CloseOnEscape
	onClosed: tNumber.text=" "

	function addNumber(text)
	{
		if(tNumber.text==" ")
			tNumber.text=text;
		else
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
		spacing: 10

		Row
		{
			Text
			{
				id: tNumber
				anchors.horizontalCenter: parent
				color: "#FFF"
				text: qsTr(" ")
			}
			Button
			{
				width: 20
				height: 20
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

		CallButton{id: bCall; text: "📞"; onClicked: call(tNumber.text)}
	}

	//QMLConnector{id: connector}

	function call(num)
	{
		connector.call(num);
		dCall.number=num;
		dCall.open();
		close();
	}
}
