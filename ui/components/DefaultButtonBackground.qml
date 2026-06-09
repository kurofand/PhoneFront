import QtQuick 2.15

Rectangle
{
	color: parent.down? Qt.darker(activeTheme.btnBackground, 1.1):activeTheme.btnBackground
}
