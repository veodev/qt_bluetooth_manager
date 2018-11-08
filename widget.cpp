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
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_scanButton_released()
{
    ui->listWidget->clear();
    ui->scanButton->setEnabled(false);
    _discoveryAgent->start();
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
}

void Widget::onBluetoothError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    ui->listWidget->addItem(QString("Bluetooth error: %1").arg(error));
    ui->scanButton->setEnabled(true);
}

void Widget::on_onButton_released()
{
    bool res = QAndroidJniObject::callStaticMethod<jboolean>("BluetoothClass", "enableBluetooth", "(Landroid/content/Context;)Z", QtAndroid::androidContext().object());
}

void Widget::on_offButton_released()
{
    bool res = QAndroidJniObject::callStaticMethod<jboolean>("BluetoothClass", "disableBluetooth", "(Landroid/content/Context;)Z", QtAndroid::androidContext().object());
}
