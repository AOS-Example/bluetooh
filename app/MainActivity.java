public class {
	public ArrayList<BluetoothDevice> mBTDevices = new ArrayList<>();
	public DeviceListAdapter mDeviceListAdapter;
	ListView lvNewDevices;

	// Broadcast Receiver for listing devices that are not
	// yet paired
	private BroadcastReceiver mBroadcastReceiver3 = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			final String action = intent.getAction();
			Log.d(TAG, "onReceive: ACTION FOUND.");

			if (action.equals(BluetoothDevice.ACTION_FOUND)) {
				BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
				mBTDevices.add(device);
				Log.d(TAG, "onReceive: " + device.getName() + ": " + device.getAddress());
				mDeviceListAdapter = new DeviceListAdapter(context, R.layout.device_adapter_view,, mBTDevices);
				lvNewDevices.setAdapter(mDeviceListAdapter);
			}
		}
	};

	// Broadcast Receiver that detects bond state changes (pairing status change)
	private final BroadcastReceiver mBroadcastReceiver4 = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			final String action = intent.getAction();

			if (action.equals(BluetoothDevice.ACTION_BOUND_STATE_CHANGE)) {
				BluetoothDevice mDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
				// case 1: bonded already
				if (mDevice.getBondState() == BluetoothDevice.BOND_BONDED) {
					Log.d(TAG, "mBroadcastReceiver4: BOND_BONDED");
				}
				// case 2: creating a bond
				if (mDevice.getBondState() == BluetoothDevice.BOND_BONDING) {
					Log.d(TAG, "mBroadcastReceiver4: BOND_BONDING");
				}
				// case 3: breaking a bond
				if (mDevice.getBondState() == BluetoothDevice.BOND_NONE) {
					Log.d(TAG, "mBroadcastReceiver4: BOND_NONE");
				} 
			}
		}
	}

	public void btnDiscover(View view) {
		Log.d(TAG, "btnDiscover: Looking for unpaired devices.");
		if (mBluetoothAdapter.isDiscovering()) {
			mBluetoothAdapter.cancelDiscovery();
			Log.d(TAG, "btnDiscover: Canceling discovery.");

			// Check BT permission in manifest
			checkBTPermissions();
			mBluetoothAdapter.startDiscovery();
			IntentFilter discoverDevicesIntent = new IntentFilter(BluetoothDevice.ACTION_FOUND);
			registerReceiver(mBroadcastReceiver3, discoverDevicesIntent);
		}
		if (!mBluetoothAdapter.isDiscovering()) {
			// Check BT permissions in manifest
			checkBTPermissions();

			mBluetoothAdapter.startDiscovery();
			IntentFilter discoverDevicesIntent = new IntentFilter(BluetoothDevice.ACTION_FOUND);
			registerReceiver(mBroadcastReceiver3, discoverDevicesIntent);
		}
	}

	private void checkBTPermissions() {
		if (Build.VERSION.SDK_INT > Build.VERSION_CODES.LOLLIPOP) {
			int permissionCheck = this.checkSelfPermission("Manifest.permission.ACCESS_FINE_LOCATION");
            permissionCheck += this.checkSelfPermission("Manifest.permission.ACCESS_COARSE_LOCATION");
            if (permissionCheck != 0) {
            	this.requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION}, 1001); //Any number
            } else{
            	Log.d(TAG, "checkBTPermissions: No need to check permissions. SDK version < LOLLIPOP.");
			}
		}
	}

	@Override
	public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
		// First cancel discovery because it's very memory intensive
		mBluetoothAdapter.cancelDiscovery();

		Log.d(TAG, "onItemClick: You clicked on a device");
		String deviceName = mBTDevices.get(i).getName();
		String deviceAddress = mBTDevices.get(i).getAddress();

		Log.d(TAG, "onItemClick: deviceName = " + deviceName);
		Log.d(TAG, "onItemClick: deviceAddress = " + deviceAddress);

		// Create the bond
		Log.d(TAG, "Trying to pair with " + deviceName);
		mBTDevices.get(i).createBond();
	}
}