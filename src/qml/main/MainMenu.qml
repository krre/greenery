import QtQuick 2.5
import QtQuick.Controls 1.4
import Osg 1.0 as Osg
import "../../js/utils.js" as Utils

MenuBar {
    property alias recentFilesModel: recentFilesModel

    Menu {
        title: qsTr("File")

        MenuItem {
            text: qsTr("New...")
            shortcut: "Ctrl+N"
            onTriggered: Utils.createDynamicObject(mainRoot, "qrc:/qml/components/dialog/NewProjectDialog.qml")
        }

        MenuItem {
            text: qsTr("Open...")
            shortcut: "Ctrl+O"
            onTriggered: {
                var fileDialog = Utils.createDynamicObject(mainRoot, "qrc:/qml/components/filedialog/FileDialogOpen.qml")
                fileDialog.accepted.connect(function() {
                    Utils.openFile(UTILS.urlToPath(fileDialog.fileUrl))
                })
            }
        }

        Menu {
            id: recentFilesMenu
            title: qsTr("Recent Files")
            enabled: recentFilesModel.count > 0

            Instantiator {
                model: recentFilesModel

                MenuItem {
                    text: model.filePath
                    onTriggered: Utils.openFile(text)
                }

                onObjectAdded: recentFilesMenu.insertItem(index, object)
                onObjectRemoved: recentFilesMenu.removeItem(object)
            }

            MenuSeparator {
                visible: recentFilesModel.count > 0
            }

            MenuItem {
                text: qsTr("Clear Menu")
                onTriggered: recentFilesModel.clear()
            }

            ListModel {
                id: recentFilesModel
            }
        }

        MenuSeparator {}

        MenuItem {
            text: qsTr("Close")
            shortcut: "Ctrl+W"
            onTriggered: tabView.removeTab(tabView.currentIndex)
            enabled: tabView.count > 0
        }

        MenuItem {
            text: qsTr("Close All")
            shortcut: "Ctrl+Shift+W"
            onTriggered: {
                while (tabView.count > 0) {
                    tabView.removeTab(0)
                }
            }
            enabled: tabView.count > 0
        }

        MenuItem {
            text: qsTr("Close Other")
            enabled: tabView.count > 1
            onTriggered: {
                var i = 0
                while (tabView.count > 1) {
                    if (i !== tabView.currentIndex) {
                        tabView.removeTab(i)
                    } else {
                        i++
                    }
                }
            }
        }

        MenuSeparator {}

        MenuItem {
            text: qsTr("Exit")
            shortcut: "Ctrl+Q"
            onTriggered: Qt.quit()
        }
    }

    Menu {
        title: qsTr("Edit")
        visible: currentTab

        MenuItem {
            text: qsTr("Cancel")
            shortcut: "Tab"
            onTriggered: {
                currentTab.cancel()
            }
        }
    }

    Menu {
        title: qsTr("Run")
        visible: currentTab

        MenuItem {
            text: qsTr("Run")
            shortcut: "F9"
            enabled: currentTab && currentTab.filePath
            onTriggered: currentTab.process.run(Settings.value("Path", "sprout"), currentTab.filePath)
        }
    }

    Menu {
        title: qsTr("Camera")
        visible: currentTab

        MenuItem {
            text: qsTr("Home")
            onTriggered: currentTab.home()
            shortcut: "F12"
        }
    }

    Menu {
        title: qsTr("Tools")

        MenuItem {
            text: qsTr("Options...")
            onTriggered: Utils.createDynamicObject(mainRoot, "qrc:/qml/main/Options.qml")
        }
    }

    Menu {
        title: qsTr("Window")
        visible: currentTab

        MenuItem {
            text: qsTr("Clear Output")
            shortcut: "Shift+Del"
            onTriggered: currentTab.output.textEdit.text = ""
        }

        MenuItem {
            text: qsTr("Output")
            checkable: true
            checked: true
            onTriggered: currentTab.output.visible = !currentTab.output.visible
        }

        MenuItem {
            text: qsTr("Command Sheet")
            checkable: true
            checked: true
            onTriggered: currentTab.commandSheet.visible = !currentTab.commandSheet.visible
        }
    }

    Menu {
        title: qsTr("Debug")
        visible: isDebug && currentTab

        MenuItem {
            text: qsTr("Write Current Node to File")
            onTriggered: Osg.OsgDb.writeNodeFile(currentTab.currentUnit, UTILS.homePath + "/node.osg")
        }

        MenuItem {
            text: qsTr("Write Scene Node to File")
            onTriggered: Osg.OsgDb.writeNodeFile(currentTab.sceneNode, UTILS.homePath + "/scene.osg")
        }
    }

    Menu {
        title: qsTr("Help")

        MenuItem {
            text: qsTr("About Greenery...")
            onTriggered: Utils.createDynamicObject(mainRoot, "qrc:/qml/main/About.qml")
        }
    }
}