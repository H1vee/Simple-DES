import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    id: mainWindow
    width: 800
    height: 900
    visible: true
    title: "S-DES Encryption System"
    color: "#f5f5f5"

    property bool keyIsValid: false
    property string primaryColor: "#3498db"
    property string accentColor: "#2980b9"
    property string errorColor: "#e74c3c"
    property string successColor: "#2ecc71"

    // Connect to the bridge signals
    Connections {
        target: cryptoBridge

        function onKeyChanged(valid) {
            keyIsValid = valid
            if (valid) {
                keyStatusText.text = "Key status: Valid"
                keyStatusText.color = successColor
            } else {
                keyStatusText.text = "Key status: Invalid"
                keyStatusText.color = errorColor
            }
        }

        function onEncryptionCompleted(result) {
            console.log("Encryption completed with result: " + result);
            cipherTextArea.text = result
        }

        function onDecryptionCompleted(result) {
            decryptedTextArea.text = result
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#f5f5f5"

        Rectangle {
            id: header
            width: parent.width
            height: 80
            color: primaryColor

            Text {
                anchors.centerIn: parent
                text: "S-DES Encryption System"
                font.pixelSize: 28
                font.bold: true
                color: "white"
            }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            anchors.topMargin: 100
            spacing: 20

            // Key Input Section
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                radius: 8
                color: "white"
                border.color: "#e0e0e0"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "Encryption Key"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    Text {
                        text: "Enter 10-bit key using only 0s and 1s"
                        font.pixelSize: 14
                        color: "#666666"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 5
                        spacing: 10

                        TextField {
                            id: keyInput
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40
                            placeholderText: "e.g., 1010000010"
                            font.pixelSize: 16
                            maximumLength: 10
                            validator: RegularExpressionValidator { regularExpression: /[01]{10}/ }
                            background: Rectangle {
                                radius: 4
                                border.color: keyInput.activeFocus ? primaryColor : "#e0e0e0"
                                border.width: keyInput.activeFocus ? 2 : 1
                            }
                        }

                        Button {
                            text: "Set Key"
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 40
                            background: Rectangle {
                                color: parent.down ? accentColor : primaryColor
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 14
                                font.bold: true
                            }
                            onClicked: {
                                console.log("Set Key clicked with value: " + keyInput.text)
                                cryptoBridge.setKey(keyInput.text)
                            }
                        }
                    }

                    Text {
                        id: keyStatusText
                        text: "Key status: Not set"
                        color: "#666666"
                        font.pixelSize: 14
                    }
                }
            }

            // Plaintext Input
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                radius: 8
                color: "white"
                border.color: "#e0e0e0"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "Plaintext"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true

                        TextArea {
                            id: plainTextArea
                            placeholderText: "Enter text to encrypt"
                            wrapMode: TextArea.Wrap
                            font.pixelSize: 14
                            background: Rectangle {
                                color: "#f9f9f9"
                                radius: 4
                                border.color: "#e0e0e0"
                            }
                        }
                    }

                    Button {
                        text: "Encrypt"
                        Layout.alignment: Qt.AlignRight
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 40
                        enabled: keyIsValid
                        background: Rectangle {
                            color: parent.enabled ? (parent.down ? accentColor : primaryColor) : "#cccccc"
                            radius: 4
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 14
                            font.bold: true
                        }
                        onClicked: {
                            console.log("Encrypt button clicked with text: " + plainTextArea.text)
                            if (plainTextArea.text.length > 0) {
                                cryptoBridge.encrypt(plainTextArea.text)
                            }
                        }
                    }
                }
            }

            // Ciphertext Display
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                radius: 8
                color: "white"
                border.color: "#e0e0e0"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "Ciphertext"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true

                        TextArea {
                            id: cipherTextArea
                            readOnly: false
                            wrapMode: TextArea.Wrap
                            placeholderText: "Encrypted text will appear here"
                            font.pixelSize: 14
                            background: Rectangle {
                                color: "#f9f9f9"
                                radius: 4
                                border.color: "#e0e0e0"
                            }
                        }
                    }

                    Button {
                        text: "Decrypt"
                        Layout.alignment: Qt.AlignRight
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 40
                        enabled: keyIsValid
                        background: Rectangle {
                            color: parent.enabled ? (parent.down ? accentColor : primaryColor) : "#cccccc"
                            radius: 4
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 14
                            font.bold: true
                        }
                        onClicked: {
                            if (cipherTextArea.text.length > 0) {
                                cryptoBridge.decrypt(cipherTextArea.text)
                            }
                        }
                    }
                }
            }

            // Decrypted Text Display
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                radius: 8
                color: "white"
                border.color: "#e0e0e0"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text {
                        text: "Decrypted Text"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true

                        TextArea {
                            id: decryptedTextArea
                            readOnly: true
                            wrapMode: TextArea.Wrap
                            placeholderText: "Decrypted text will appear here"
                            font.pixelSize: 14
                            background: Rectangle {
                                color: "#f9f9f9"
                                radius: 4
                                border.color: "#e0e0e0"
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Button {
                            text: "Clear All"
                            Layout.preferredWidth: 120
                            Layout.preferredHeight: 40
                            background: Rectangle {
                                color: "#95a5a6"
                                radius: 4
                                opacity: parent.down ? 0.8 : 1
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 14
                                font.bold: true
                            }
                            onClicked: {
                                plainTextArea.text = ""
                                cipherTextArea.text = ""
                                decryptedTextArea.text = ""
                            }
                        }

                        Item { Layout.fillWidth: true }

                        Text {
                            text: "Results should match original plaintext"
                            color: "#666666"
                            font.italic: true
                        }
                    }
                }
            }
        }
    }
}