export module AppNotification;

export namespace gfl
{
	class AppNotification
	{
	public:
		AppNotification() = default;
		AppNotification(const AppNotification&) = delete;
		AppNotification(AppNotification&&) = delete;
		AppNotification& operator=(const AppNotification&) = delete;
		AppNotification& operator=(AppNotification&&) = delete;
		virtual ~AppNotification() = default;
		virtual void OnActivated() = 0;
		virtual void OnDeactivated() = 0;
		virtual void OnResuming() = 0;
		virtual void OnSuspending() = 0;
		virtual void OnQuit() = 0;
		virtual void OnWindowSizeChanged(int width, int height) = 0;
		virtual void OnWindowMoved() = 0;
		virtual void OnDisplayChange() = 0;
	};
}