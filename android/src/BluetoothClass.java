import android.content.Context;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothHeadset;
import android.bluetooth.BluetoothProfile.ServiceListener;
import android.bluetooth.BluetoothA2dp;
import android.os.ParcelUuid;
import android.util.Log;
import android.bluetooth.BluetoothProfile;
import android.provider.Settings;
import java.util.UUID;
import java.lang.reflect.Method;
import java.util.Set;

public class BluetoothClass {    
    BluetoothA2dp bluetoothA2dp = null;
    static MediaPlayer mediaPlayer = mediaPlayer = new MediaPlayer();
    static BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

    final BluetoothProfile.ServiceListener btServiceListener = new BluetoothProfile.ServiceListener() {
        public void onServiceConnected(int profile, BluetoothProfile proxy) {
            if (profile == BluetoothProfile.A2DP) {
                Log.d("", "===================== A2DP SERVICE CONNECTED!!!");                                
                bluetoothA2dp = (BluetoothA2dp) proxy;
//                resetAudioManager(audioManager);
            }
        }

        public void onServiceDisconnected(int profile) {
            if (profile == BluetoothProfile.A2DP) {
                Log.d("", "===================== A2DP SERVICE DISCONNECTED!!!");
                bluetoothA2dp = null;
            }
        }
    };

    public static boolean enableBluetooth()
    {        
        return bluetoothAdapter.enable();
    }

    public static boolean disableBluetooth()
    {        
//        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        return bluetoothAdapter.disable();
    }

    private static void resetAudioManager(AudioManager audioManager)
    {
        if (audioManager != null) {
            audioManager.setMode(AudioManager.MODE_NORMAL);
            audioManager.stopBluetoothSco();
            audioManager.setBluetoothScoOn(false);
            audioManager.setSpeakerphoneOn(false);
            audioManager.setWiredHeadsetOn(false);
        }
    }

    public void registerBroadcast(Context context)
    {        
        try {
            mediaPlayer.setDataSource("/sdcard/Music/Synthya - Be Free.mp3");
            mediaPlayer.prepare();
            }
        catch (Exception e) {
        }

        BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
                    @Override
                    public void onReceive(Context context, Intent intent) {
//                        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
                        String action = intent.getAction();
                        BluetoothDevice bluetoothDevice;
                        if (action.equals(BluetoothDevice.ACTION_BOND_STATE_CHANGED)) {
                            bluetoothDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                            bluetoothAdapter.getProfileProxy(context, btServiceListener, BluetoothProfile.A2DP);
                            Log.d("", "PAIRED: " + bluetoothDevice.getAddress());
                            Method connect;
                            try {
                                connect = BluetoothA2dp.class.getDeclaredMethod("connect", BluetoothDevice.class);
                                connect.invoke(bluetoothA2dp, bluetoothDevice);
                            }
                            catch (Exception e) {
                            }
                        }
                        else if (action.equals(BluetoothDevice.ACTION_ACL_CONNECTED)) {
                            Log.d("", "============ ACTION_ACL_CONNECTED");
                        }
                        else if (action.equals(BluetoothDevice.ACTION_ACL_DISCONNECTED)) {
                            Log.d("", "============ ACTION_ACL_DISCONNECTED");
                        }
                    }
                };

            IntentFilter intentFilter = new IntentFilter(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
            intentFilter.addAction(BluetoothDevice.ACTION_ACL_CONNECTED);
            intentFilter.addAction(BluetoothDevice.ACTION_ACL_DISCONNECTED);
            context.registerReceiver(broadcastReceiver, intentFilter);
    }

    public void playMusic()
    {
        if (mediaPlayer != null) {
            mediaPlayer.start();
        }
    }

    public void stopMusic()
    {
        if (mediaPlayer != null) {
            mediaPlayer.pause();
        }
    }

    public static void openBluetoothNativeSettings(Context context)
    {
        context.startActivity(new Intent(Settings.ACTION_BLUETOOTH_SETTINGS));
    }

    public static String getPairedDevices()
    {
        String pairedDevices = new String();
        Set<BluetoothDevice> setPairedDevices = bluetoothAdapter.getBondedDevices();
        for (BluetoothDevice bluetoothDevice: setPairedDevices) {
            pairedDevices = pairedDevices.concat(bluetoothDevice.getAddress()).concat(" ").concat(bluetoothDevice.getName()).concat("|");
        }
        return pairedDevices;
    }

    public static boolean isBluetoothConnected()
    {
        return bluetoothAdapter.isEnabled() && (bluetoothAdapter.getProfileConnectionState(BluetoothProfile.A2DP) == BluetoothProfile.STATE_CONNECTED);
    }
}
