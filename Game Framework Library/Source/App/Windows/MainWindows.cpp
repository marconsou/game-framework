import "WindowsCompact.h";
import CrtDebugLogWindows;
import EntryPoint;
import Log;
import MutexWindows;
import WindowsApi;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfl::CrtDebugLogWindows::EnableCrtDebugLog();

	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize{RO_INIT_MULTITHREADED};
	if FAILED(initialize)
		gfl::Log::Error("RoInitializeWrapper RO_INIT_MULTITHREADED");

	const gfl::MutexWindows mutexWindows;
	if (mutexWindows.IsFirstInstance())
		return gfl::EntryPoint::Main();
	else
		gfl::WindowsApi::RestoreApp();

	return EXIT_SUCCESS;
}