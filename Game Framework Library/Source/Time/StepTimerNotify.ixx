export module StepTimerNotify;

export namespace gfl
{
	class StepTimerNotify
	{
	public:
		StepTimerNotify() = default;
		StepTimerNotify(const StepTimerNotify&) = delete;
		StepTimerNotify(StepTimerNotify&&) = delete;
		StepTimerNotify& operator=(const StepTimerNotify&) = delete;
		StepTimerNotify& operator=(StepTimerNotify&&) = delete;
		virtual ~StepTimerNotify() = default;
		virtual void Update() = 0;
	};
}