import QtQuick 2.15

Item
{
	id: dialForm

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
			dialForm.addNumber(text)
		}
	}

	Grid
	{
		rows: 3
		spacing: 10
		horizontalItemAlignment: Grid.AlignHCenter

		Text
		{
			id: tNumber
			anchors.horizontalCenter: parent
			color: "#FFF"
			text: qsTr(" ")
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

		CallButton{id: bCall; text: "📞";}
	}
}
