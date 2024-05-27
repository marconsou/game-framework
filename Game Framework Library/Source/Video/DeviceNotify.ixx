export module DeviceNotify;

export namespace gfl
{
	class DeviceNotify
	{
	public:
		DeviceNotify() = default;
		DeviceNotify(const DeviceNotify&) = delete;
		DeviceNotify(DeviceNotify&&) = delete;
		DeviceNotify& operator=(const DeviceNotify&) = delete;
		DeviceNotify& operator=(DeviceNotify&&) = delete;
		virtual ~DeviceNotify() = default;
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;
	};
}