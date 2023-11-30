export module DeviceStateNotification;

export namespace gfl
{
	class DeviceStateNotification
	{
	public:
		DeviceStateNotification() = default;
		DeviceStateNotification(const DeviceStateNotification&) = delete;
		DeviceStateNotification(DeviceStateNotification&&) = delete;
		DeviceStateNotification& operator=(const DeviceStateNotification&) = delete;
		DeviceStateNotification& operator=(DeviceStateNotification&&) = delete;
		virtual ~DeviceStateNotification() = default;
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;
	};
}