export module DisplayNotify;

export namespace gfl
{
	class DisplayNotify
	{
	public:
		DisplayNotify() = default;
		DisplayNotify(const DisplayNotify&) = delete;
		DisplayNotify(DisplayNotify&&) = delete;
		DisplayNotify& operator=(const DisplayNotify&) = delete;
		DisplayNotify& operator=(DisplayNotify&&) = delete;
		virtual ~DisplayNotify() = default;
		virtual void OnWindowSizeChanged(int width, int height) = 0;
		virtual void OnWindowMoved() = 0;
		virtual void OnDisplayChange() = 0;
	};
}