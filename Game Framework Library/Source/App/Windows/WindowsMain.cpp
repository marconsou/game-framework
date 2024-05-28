import CrtDebug;
import EntryPoint;
import WindowsApi;
import WindowsMutex;
import "WindowsCompact.h";

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfl::CrtDebug::EnableLog();

	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize{RO_INIT_MULTITHREADED};
	if FAILED(initialize)
		return EXIT_FAILURE;

	const gfl::WindowsMutex mutex;
	if (mutex.IsFirstInstance())
		return gfl::EntryPoint::Main();
	else
	{
		gfl::WindowsApi::RestoreApp();
		return EXIT_SUCCESS;
	}
}