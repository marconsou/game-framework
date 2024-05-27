export module Game;

import std;
import DeviceResources;
import StepTimer;
import "D:/Documents/Development/Visual Studio/Game Framework/Game Framework Library/Source/App/Windows/WindowsCompact.h";

export namespace gfl
{
    class Game final : public IDeviceNotify
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
        void OnActivated();
        void OnDeactivated();
        void OnSuspending();
        void OnResuming();
        void OnWindowMoved();
        void OnDisplayChange();
        void OnWindowSizeChanged(int width, int height);

        // Properties
        void GetDefaultSize(int& width, int& height) const noexcept;

    private:

        void Update(gfl::StepTimer const& timer);
        void Render();

        void Clear();

        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();

        // Device resources.
        std::unique_ptr<gfl::DeviceResources>    m_deviceResources;

        // Rendering loop timer.
        gfl::StepTimer                           m_timer;
    };
}