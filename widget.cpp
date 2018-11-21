#include "widget.h"
#include "ui_widget.h"
#include <QAndroidJniEnvironment>
#include <QtAndroidExtras>
#include <QDebug>


Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , _mediaPlayer(new QMediaPlayer(this))
{
    ui->setupUi(this);
    QAndroidJniObject::callStaticMethod<jboolean>("BluetoothClass", "enableBluetooth", "()Z");
    QtAndroid::runOnAndroidThread([] {
        QAndroidJniObject jniObject("BluetoothClass");
        jniObject.callMethod<void>("registerBroadcast", "(Landroid/content/Context;)V", QtAndroid::androidContext().object());
    });

    _discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    _localDevice = new QBluetoothLocalDevice(this);

    connect(_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Widget::onAddDevice);
    connect(_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &Widget::onScanFinished);
    connect(_discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error), this, &Widget::onBluetoothError);
    connect(_localDevice, &QBluetoothLocalDevice::pairingFinished, this, &Widget::onPairingFinished);

    _mediaPlayer->setMedia(QUrl::fromLocalFile("/sdcard/Music/Synthya - Be Free.mp3"));
    _mediaPlayer->setVolume(100);
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
    bool res = QAndroidJniObject::callStaticMethod<jboolean>("BluetoothClass", "enableBluetooth", "()Z");
}

void Widget::on_offButton_released()
{
    bool res = QAndroidJniObject::callStaticMethod<jboolean>("BluetoothClass", "disableBluetooth", "()Z");
}

void Widget::on_pairButton_released()
{
    if (ui->listWidget->currentRow() == -1) {
        return;
    }
    _discoveryAgent->stop();
    QString itemStr = ui->listWidget->currentItem()->text();
    QString address = itemStr.split(" ").first();
    qDebug() << "ADDRESS: " << address;
    _localDevice->requestPairing(QBluetoothAddress(address), QBluetoothLocalDevice::Paired);
}

void Widget::onPairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing)
{
    qDebug() << "Pairing with: " << address;
}

void Widget::on_musicOnButton_released()
{
    _mediaPlayer->play();
}

void Widget::on_musicOffButton_released()
{
    _mediaPlayer->pause();
}

void Widget::on_settingsButton_released()
{
    QAndroidJniObject::callStaticMethod<jboolean>("BluetoothClass", "openBluetoothNativeSettings", "(Landroid/content/Context;)V", QtAndroid::androidContext().object());
}
