export module AppNotify;

export namespace gfl
{
	class AppNotify
	{
	public:
		AppNotify() = default;
		AppNotify(const AppNotify&) = delete;
		AppNotify(AppNotify&&) = delete;
		AppNotify& operator=(const AppNotify&) = delete;
		AppNotify& operator=(AppNotify&&) = delete;
		virtual ~AppNotify() = default;
		virtual void OnRun() = 0;
		virtual void OnActivated() = 0;
		virtual void OnDeactivated() = 0;
		virtual void OnResuming() = 0;
		virtual void OnSuspending() = 0;
		virtual void OnQuit() = 0;
	};
}