#include "widget.h"
#include "ui_widget.h"
#include <QAndroidJniEnvironment>
#include <QtAndroidExtras>
#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    _discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    _localDevice = new QBluetoothLocalDevice(this);

    connect(_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Widget::onAddDevice);
    connect(_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &Widget::onScanFinished);
    connect(_discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error), this, &Widget::onBluetoothError);

    connect(_localDevice, &QBluetoothLocalDevice::pairingFinished, this, &Widget::onPairingFinished);

    connect(&_checkTimer, &QTimer::timeout, this, &Widget::onCheckBtScoTimeout);
    _checkTimer.start(1000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_scanButton_released()
{
    ui->listWidget->clear();
    ui->scanButton->setEnabled(false);
    _discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
}

void Widget::onAddDevice(const QBluetoothDeviceInfo& info)
{
    QString infoStr = QString("%1 %2").arg(info.address().toString()).arg(info.name());
    qDebug() << infoStr;
    ui->listWidget->addItem(infoStr);
}

void Widget::onScanFinished()
{
    ui->scanButton->setEnabled(true);
    ui->pairButton->setEnabled(true);
}

void Widget::onBluetoothError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    ui->listWidget->addItem(QString("Bluetooth error: %1").arg(error));
    ui->scanButton->setEnabled(true);
    ui->pairButton->setEnabled(true);
}

void Widget::on_onButton_released()
{
    bool res = QAndroidJniObject::callStaticMethod<jboolean>("BluetoothClass", "enableBluetooth", "(Landroid/content/Context;)Z", QtAndroid::androidContext().object());
}

void Widget::on_offButton_released()
{
    bool res = QAndroidJniObject::callStaticMethod<jboolean>("BluetoothClass", "disableBluetooth", "(Landroid/content/Context;)Z", QtAndroid::androidContext().object());
}

void Widget::on_pairButton_released()
{
    if (ui->listWidget->currentRow() == -1) {
        return;
    }
    QString itemStr = ui->listWidget->currentItem()->text();
    QString address = itemStr.split(" ").first();
    qDebug() << "ADDRESS: " << address;
    _localDevice->requestPairing(QBluetoothAddress(address), QBluetoothLocalDevice::Paired);
}

void Widget::onPairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing)
{
    qDebug() << "Pairing with: " << address;
}

void Widget::on_speakerOnButton_released()
{
    QAndroidJniObject::callStaticMethod<void>("BluetoothClass", "enableSpeakerphone", "(Landroid/content/Context;)V", QtAndroid::androidContext().object());
}

void Widget::on_speakerOffButton_released()
{
    QAndroidJniObject::callStaticMethod<void>("BluetoothClass", "disableSpeakerphone", "(Landroid/content/Context;)V", QtAndroid::androidContext().object());
}

void Widget::on_btAudioOnButton_released()
{
    QAndroidJniObject::callStaticMethod<void>("BluetoothClass", "enableBluetoothAudio", "(Landroid/content/Context;)V", QtAndroid::androidContext().object());
}

void Widget::on_btAudioOffButton_released()
{
    QAndroidJniObject::callStaticMethod<void>("BluetoothClass", "disableBluetoothAudio", "(Landroid/content/Context;)V", QtAndroid::androidContext().object());
}

void Widget::onCheckBtScoTimeout()
{
    bool res = QAndroidJniObject::callStaticMethod<jboolean>("BluetoothClass", "isBluetoothScoOn", "(Landroid/content/Context;)Z", QtAndroid::androidContext().object());
}
