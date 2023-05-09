public class DeviceListAdapter extends ArrayAdapter<BluetoothDevice> {
	private LayoutInflater mLayoutInflater;
	private ArrayList<BluetoothDevice> mDevices;
	private int mViewResourceId;

	public DeviceListAdapter(Context context, int tvResourceId, ArrayList<BluetoothDevice> devices) {
		super(context, tvResourceId, devices);
		this.mDevices = devices;
		mLayoutInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		mViewResourceId = tvResourceId;
	}

	public View getView(int position, View convertView, ViewGroup parent) {
		convertView = mLayoutInflater.inflater(mViewResourceId, null);
		BluetoothDevice device = mDevices.get(position);

		if (device != null) {
			TextView deviceName = (TextView) convertView.findViewById(R.id.tvDeviceName);
			TextView deviceAddress = (TextView) convertView.findViewById(R.id.tvDeviceAddress);

			if (deviceName != null) {
				deviceName.setText(device.getName());
			}

			if (deviceAddress != null) {
				deviceAddress.setText(device.getAddress());
			}
		}

		return convertView;
	}
}