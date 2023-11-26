export module DeviceNotification;

export namespace gfl
{
	class DeviceNotification
	{
	public:
		DeviceNotification() = default;
		DeviceNotification(const DeviceNotification&) = delete;
		DeviceNotification(DeviceNotification&&) = delete;
		DeviceNotification& operator=(const DeviceNotification&) = delete;
		DeviceNotification& operator=(DeviceNotification&&) = delete;
		virtual ~DeviceNotification() = default;
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;
	};
}