import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import "../components"

ColumnLayout
{
	spacing: 10

	FragmentHeader{text: "Connection info"}

	MarginRow
	{
		Layout.topMargin: 5

		StatusText{text: "Operator:"}
		StatusText{id: tOperator; objectName:"tOperator"; text:"Failed to get an operator name"}
	}

	MarginRow
	{
		StatusText{text:"Connection status:"}
		StatusText{id: tConnectionStatus; objectName:"tConnectionStatus"; text:"CONNECTION_STATUS"}
	}

	MarginRow
	{
		StatusText{text:"Signal strength:"}
		StatusText{id: tSignalStrength; objectName:"tSignalStrength"; text:"SIGNAL_STRENGTH"}
	}

	MarginRow
	{
		Button
		{
			width: 50
			height: 50
			enabled: !root.updatingConnectionStatus&&!root.updatingSignalStrength
			onClicked: connector.updateStatus()
			contentItem: Text
			{
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				font.pointSize: 15
				text: "⟳"
			}
		}
	}
}
