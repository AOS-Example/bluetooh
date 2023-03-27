public class MainActivity extends Activity {
	public void on(View v) {
		if (!BA.isEnabled()) {
			Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);

		}
	}

	public void off(View v) {
		BA.disable();
	}

	public void visible(View v) {
		Intent getVisible = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
	}
}