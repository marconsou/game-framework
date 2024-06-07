export module Game;

import App;
import GameNotify;
import Input;
import Log;
import ShaderManager;
import StepTimer;
import StepTimerNotify;
import Video;

export namespace gfl
{
	class Game : public GameNotify, public StepTimerNotify
	{
	public:
		Game() = default;
		Game(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(const Game&) = delete;
		Game& operator=(Game&&) = delete;
		virtual ~Game() = default;
		int Start();
	protected:
		std::unique_ptr<Log> log;
		std::unique_ptr<App> app;
		std::unique_ptr<Video> video;
		std::unique_ptr<Input> input;
		std::unique_ptr<ShaderManager> shaderManager;
		StepTimer timer;
		virtual void OnResuming() override;
		virtual void OnDeviceRestored() override;
		virtual void OnResize(int width, int height) override;
		virtual void OnLogError(std::string_view message) const override;
		virtual void CreateDeviceDependentResources() = 0;
		virtual void CreateWindowSizeDependentResources() = 0;
	};
}