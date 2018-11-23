#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <QMediaPlayer>
#include <QTimer>

namespace Ui
{
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget();

private slots:
    void on_scanButton_released();
    void onAddDevice(const QBluetoothDeviceInfo& info);
    void onScanFinished();
    void onScanCanceled();
    void onBluetoothError(QBluetoothDeviceDiscoveryAgent::Error error);
    void on_onButton_released();
    void on_offButton_released();
    void on_pairButton_released();
    void onPairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing);
    void on_musicOnButton_released();
    void on_musicOffButton_released();

    void on_settingsButton_released();

    void on_getPairedButton_released();

    void onCheckConnection();

private:
    Ui::Widget* ui;

    QBluetoothDeviceDiscoveryAgent* _discoveryAgent;
    QBluetoothLocalDevice* _localDevice;
    QMediaPlayer* _mediaPlayer;
    QTimer _timer;
};

#endif  // WIDGET_H
