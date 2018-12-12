import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import MaterialIcons 2.2
import Utils 1.0

/**
  A component to display and edit a Node's attributes.
*/
ColumnLayout {
    id: root

    property variant node: null  // the node to edit
    property bool readOnly: false
    readonly property bool isCompatibilityNode: node.hasOwnProperty("compatibilityIssue")

    signal upgradeRequest()

    spacing: 0

    Pane {
        Layout.fillWidth: true
        background: Rectangle { color: Qt.darker(parent.palette.window, 1.15) }
        padding: 2

        RowLayout {
            width: parent.width

            Label {
                Layout.fillWidth: true
                elide: Text.ElideMiddle
                text: node.nodeType
                horizontalAlignment: Text.AlignHCenter
                padding: 6
            }

            ToolButton {
                text: MaterialIcons.settings
                font.family: MaterialIcons.fontFamily
                onClicked: settingsMenu.popup()
                checkable: true
                checked: settingsMenu.visible
            }
        }
        Menu {
            id: settingsMenu
            MenuItem {
                text: "Open Cache Folder"
                onClicked: Qt.openUrlExternally(Filepath.stringToUrl(node.internalFolder))
                ToolTip.text: node.internalFolder
                ToolTip.visible: hovered
                ToolTip.delay: 500
            }
            MenuSeparator {}
            MenuItem {
                text: "Clear Submitted Status"
                onClicked: node.clearSubmittedChunks()
            }
        }
    }

    // CompatibilityBadge banner for CompatibilityNode
    Loader {
        active: isCompatibilityNode
        Layout.fillWidth: true
        visible: active  // for layout update

        sourceComponent: CompatibilityBadge {
            canUpgrade: root.node.canUpgrade
            issueDetails: root.node.issueDetails
            onUpgradeRequest: root.upgradeRequest()
            sourceComponent: bannerDelegate
        }
    }

    StackLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true

        currentIndex: tabBar.currentIndex

        Item {

            ListView {
                id: attributesListView

                anchors.fill: parent
                anchors.margins: 4

                clip: true
                spacing: 1
                ScrollBar.vertical: ScrollBar { id: scrollBar }

                model: node ? node.attributes : undefined

                delegate: AttributeItemDelegate {
                    readOnly: root.isCompatibilityNode
                    labelWidth: 180
                    width: attributesListView.width
                    attribute: object
                }
                // Helper MouseArea to lose edit/activeFocus
                // when clicking on the background
                MouseArea {
                    anchors.fill: parent
                    onClicked: root.forceActiveFocus()
                    z: -1
                }
            }
        }

        NodeLog {
            id: nodeLog

            Layout.fillHeight: true
            Layout.fillWidth: true
            node: root.node

        }
    }
    TabBar {
        id: tabBar

        Layout.fillWidth: true
        width: childrenRect.width
        position: TabBar.Footer
        TabButton {
            text: "Attributes"
            width: implicitWidth
            padding: 4
            leftPadding: 8
            rightPadding: leftPadding
        }
        TabButton {
            text: "Log"
            width: implicitWidth
            leftPadding: 8
            rightPadding: leftPadding
        }
    }
}
