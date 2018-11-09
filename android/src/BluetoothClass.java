import android.content.Context;
import android.bluetooth.BluetoothAdapter;
import android.media.AudioManager;
import android.os.SystemClock;

public class BluetoothClass {
    public static boolean enableBluetooth(Context context)
    {
        BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
        return btAdapter.enable();
    }

    public static boolean disableBluetooth(Context context)
    {
        BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
        return btAdapter.disable();
    }

    public static void enableSpeakerphone(Context context)
    {
        AudioManager audioManager = (AudioManager) context.getSystemService(context.AUDIO_SERVICE);
        audioManager.setMode(AudioManager.MODE_NORMAL);
        audioManager.stopBluetoothSco();
        audioManager.setBluetoothScoOn(false);
        audioManager.setSpeakerphoneOn(true);
        System.out.println("EnableSpeakerphone");

    }

    public static void disableSpeakerphone(Context context)
    {
        AudioManager audioManager = (AudioManager) context.getSystemService(context.AUDIO_SERVICE);
        audioManager.setSpeakerphoneOn(false);
        System.out.println("DisableSpeakerphone");
    }

    public static void enableBluetoothAudio(Context context)
    {
        AudioManager audioManager = (AudioManager) context.getSystemService(context.AUDIO_SERVICE);
        audioManager.setMode(0);
        audioManager.setBluetoothScoOn(true);
        audioManager.startBluetoothSco();
        audioManager.setMode(AudioManager.MODE_NORMAL);
        audioManager.setSpeakerphoneOn(true);
        System.out.println("EnableBluetoothAudio");
    }

    public static void disableBluetoothAudio(Context context)
    {
        AudioManager audioManager = (AudioManager) context.getSystemService(context.AUDIO_SERVICE);

        audioManager.setBluetoothScoOn(false);
        audioManager.stopBluetoothSco();
        System.out.println("DisableBluetoothAudio");
    }

    public static boolean isBluetoothScoOn(Context context)
    {
        AudioManager audioManager = (AudioManager) context.getSystemService(context.AUDIO_SERVICE);
//        boolean res = audioManager.isBluetoothScoOn();
//        System.out.println("BluetoothSco");
//        System.out.println(res);
        System.out.println("WIRED HEADSET: " + audioManager.isWiredHeadsetOn());
        System.out.println("BLUETOOTH SCO: " + audioManager.isBluetoothScoOn());
        System.out.println("SPEAKERPHONE: " + audioManager.isSpeakerphoneOn());
        System.out.println("=============================================");

        return true;
    }
}
