export module Game;

import std;
import DeviceResources;
import StepTimer;
import AppNotify;
import DeviceNotify;
import DisplayNotify;
import LogNotify;
import "D:/Documents/Development/Visual Studio/Game Framework/Game Framework Library/Source/App/Windows/WindowsCompact.h";

import std;
import Clock;
import DateTime;
import EntryPoint;
import WindowsApi;
import WindowsApp;
import AppConfiguration;
import FileLogger;

export namespace gfl
{
	class Game final : public DeviceNotify, public AppNotify, public DisplayNotify, public LogNotify
	{
	public:

		Game() noexcept(false);
		~Game() = default;

		Game(Game&&) = default;
		Game& operator= (Game&&) = default;

		Game(Game const&) = delete;
		Game& operator= (Game const&) = delete;

		// Initialization and management
		void Initialize(HWND window, int width, int height);

		// Basic game loop
		void Tick();

		// IDeviceNotify
		void OnDeviceLost() override;
		void OnDeviceRestored() override;

		// Messages
		void OnRun() override;
		void OnActivated() override;
		void OnDeactivated() override;
		void OnSuspending() override;
		void OnResuming() override;
		void OnQuit() override;



		void OnWindowMoved()override;
		void OnDisplayChange()override;
		void OnWindowSizeChanged(int width, int height)override;


		void OnLogError(std::string_view message) const override
		{

		}

		std::unique_ptr<FileLogger> fileLogger;
		AppConfiguration appConfiguration;
		std::unique_ptr<WindowsApp> windowsApp;

	private:

		void Update(StepTimer const& timer);
		void Render();

		void Clear();

		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();

		// Device resources.
		std::unique_ptr<DeviceResources>    m_deviceResources;

		// Rendering loop timer.
		StepTimer                           m_timer;
	};
}