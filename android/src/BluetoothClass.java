import android.content.Context;
import android.bluetooth.BluetoothAdapter;

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

}
