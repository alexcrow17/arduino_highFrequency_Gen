from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
from PyQt5.QtGui import QIcon
import numpy as np

# ------------------------------------------------------------
app = QtWidgets.QApplication([])
uiMain = uic.loadUi("qt\main.ui")  # load UI file
uiMain.setWindowTitle("UHF Generator")  # name Tab
uiMain.setWindowIcon(QIcon('dashboard.ico'))
uiMain.show()
# ------------------------------------------------------------
serial_1 = QSerialPort()


CONNECT = False


# ------------------------------------------------------------
def voidS_1():
    global CONNECT
    rx = serial_1.readLine()
    rxs = str(rx, 'utf-8').strip()
    data = rxs.split(',')
    print(data)
    password = '17,35,14,76,'
    if data[0] == 'UHF_GEN_REV1.1':
        serial_1.write(password.encode())
        block_element(False)
        CONNECT = True
    else:
        if data[0] != '':
            dataProcessing(data)


def dataProcessing(data):
    # обработка входных данных
    if data[0] == '1':
        uiMain.qt_stay_ask.setText("ASK  ON")
        uiMain.qt_start_ask.setText("STOP")
    else:
        uiMain.qt_stay_ask.setText("ASK OFF")
        uiMain.qt_start_ask.setText("START")

    uiMain.qt_ask_freq.setValue(int(data[1]))

    vco_freq = float(data[3])
#    vco_freq = ((vco_freq - 279) / (3750 - 280)) * (12 - 7) + 7
    vco_freq = -0.0000001704 * (float(vco_freq) * float(vco_freq)) + 0.0018342965 * float(vco_freq) + 7.5157006048
    uiMain.qt_vco_freq_g.setValue(int(vco_freq / 1))
    uiMain.qt_vco_freq_m.setValue(int((vco_freq * 1000) % 1000))

    mask_10 = int(data[2])
    mask_10 = (mask_10 / 2) - 26.5
    uiMain.qt_out_power.setText(str(mask_10))
    upd_bar()
    # uiMain.qt_temp_dvs.setText(data[3])


def UpdatePortList():
    portList = []
    ports = QSerialPortInfo().availablePorts()
    for port in ports:
        portList.append(port.portName())
    uiMain.qt_comport_list.clear()
    uiMain.qt_comport_list.addItems(portList)


def onOpen_1():
    global CONNECT
    serial_1.setPortName(uiMain.qt_comport_list.currentText())
    serial_1.setBaudRate(int(uiMain.qt_speed_list.currentText()))
    if serial_1.open(QIODevice.ReadWrite):
        uiMain.qt_open_comport.setText("CLOSE")
        uiMain.qt_comport_list.setEnabled(False)
        uiMain.qt_speed_list.setEnabled(False)

    else:
        serial_1.close()
        uiMain.qt_open_comport.setText("OPEN")
        uiMain.qt_start.setText("START")
        uiMain.qt_comport_list.setEnabled(True)
        uiMain.qt_speed_list.setEnabled(True)
        block_element(True)
        CONNECT = False


def out_power_dec():
    level = float(uiMain.qt_out_power.text())
    if level > -26.5:
        level = level - 0.5
    uiMain.qt_out_power.setText(str(level))


def out_power_inc():
    level = float(uiMain.qt_out_power.text())
    if level < 5:
        level = level + 0.5
    uiMain.qt_out_power.setText(str(level))


def out_power_0():
    uiMain.qt_out_power.setText('0.0')


def out_power_10():
    uiMain.qt_out_power.setText('-10.0')


def out_power_20():
    uiMain.qt_out_power.setText('-20.0')


def start_ask():
    if uiMain.qt_start_ask.text() == "START":
        uiMain.qt_start_ask.setText("STOP")
        uiMain.qt_stay_ask.setText("ASK  ON")
    else:
        uiMain.qt_start_ask.setText("START")
        uiMain.qt_stay_ask.setText("ASK OFF")


def block_element(flag):
    flag = not flag
    uiMain.qt_ask_freq.setEnabled(flag)
    uiMain.qt_start_ask.setEnabled(flag)
    uiMain.qt_vco_freq_g.setEnabled(flag)
    uiMain.qt_vco_freq_m.setEnabled(flag)
    uiMain.qt_out_power_dec.setEnabled(flag)
    uiMain.qt_out_power.setEnabled(flag)
    uiMain.qt_out_power_inc.setEnabled(flag)
    uiMain.qt_out_power_10.setEnabled(flag)
    uiMain.qt_out_power_20.setEnabled(flag)
    uiMain.qt_out_power_30.setEnabled(flag)
    uiMain.label_5.setEnabled(flag)
    uiMain.label_6.setEnabled(flag)
    uiMain.label_4.setEnabled(flag)
    uiMain.qt_stay_ask.setEnabled(flag)
    uiMain.label_3.setEnabled(flag)
    uiMain.label_2.setEnabled(flag)
    uiMain.label.setEnabled(flag)
    uiMain.label_7.setEnabled(flag)
    uiMain.label_8.setEnabled(flag)
    uiMain.label_10.setEnabled(flag)
    uiMain.qt_send_data.setEnabled(flag)
    uiMain.qt_start.setEnabled(flag)
    uiMain.label_10.setText("OUT GEN:     OFF")


def send_data():

    if uiMain.qt_start_ask.text() == "START":
        message = 0
    else:
        message = 1

    serialSend([0, message])

    message = uiMain.qt_ask_freq.value()
    serialSend([1, message])

    message = mask_32_to_64(uiMain.qt_out_power.text())
    serialSend([2, 63 - message])

    vco_freq = float(uiMain.qt_vco_freq_g.value()) + (float(uiMain.qt_vco_freq_m.value()) / 1000)
    vco_freq = 27.1649740437 * (float(vco_freq) * float(vco_freq) * float(vco_freq)) - 703.3297714879 * (float(vco_freq) * float(vco_freq)) + 6678.2828258624 * float(vco_freq) - 22047.5792314074
    print(vco_freq)
    message = int(vco_freq)
    serialSend([3, message])

    upd_bar()


def serialSend(data):
    txs = ""
    for val in data:
        txs += str(val)
        txs += ','
    txs = txs[:-1]
    txs += ';'
    serial_1.write(txs.encode())
    print(txs)


def upd_bar():
    buff = "VCO: " + str(uiMain.qt_vco_freq_g.value()) + "."
    if uiMain.qt_vco_freq_m.value() < 100:
        buff += "0"
    if uiMain.qt_vco_freq_m.value() < 10:
        buff += "0"
    buff += str(uiMain.qt_vco_freq_m.value()) + " GHz "

    buff += uiMain.qt_out_power.text() + " dBm"
    uiMain.label_5.setText(buff)

    buff = "ASK: " + str(uiMain.qt_ask_freq.value())
    if uiMain.qt_start_ask.text() == "START":
        buff += " kHz OFF"
    else:
        buff += " kHz ON"
    uiMain.label_6.setText(buff)


def mask_32_to_64(power):
    power = float(power) + 26.5
    power = power * 2
    return int(power)


def start():
    if uiMain.qt_start.text() == "START":
        uiMain.qt_start.setText("STOP")
        uiMain.label_10.setText("OUT GEN:      ON")
        serialSend([4, 1])

    else:
        uiMain.qt_start.setText("START")
        uiMain.label_10.setText("OUT GEN:      OFF")
        serialSend([4, 0])


serial_1.readyRead.connect(voidS_1)

# -----------Main Window-------------
uiMain.qt_upd_comport_list.clicked.connect(UpdatePortList)
uiMain.qt_open_comport.clicked.connect(onOpen_1)

uiMain.qt_start_ask.clicked.connect(start_ask)
uiMain.qt_out_power_dec.clicked.connect(out_power_dec)
uiMain.qt_out_power_inc.clicked.connect(out_power_inc)
uiMain.qt_out_power_10.clicked.connect(out_power_20)
uiMain.qt_out_power_20.clicked.connect(out_power_10)
uiMain.qt_out_power_30.clicked.connect(out_power_0)

uiMain.qt_send_data.clicked.connect(send_data)
uiMain.qt_start.clicked.connect(start)
# -----------------------------------

app.exec()
