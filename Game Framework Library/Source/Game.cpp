module Game;

namespace gfl
{
	int Game::Start()
	{
		return this->app->Run();
	}

	void Game::OnResuming()
	{
		this->timer.ResetElapsedTime();
	}

	void Game::OnDeviceRestored()
	{
		this->CreateDeviceDependentResources();
		this->CreateWindowSizeDependentResources();
	}

	void Game::OnResize(int width, int height)
	{
		this->CreateWindowSizeDependentResources();
	}

	void Game::OnLogError(std::string_view message) const
	{
		throw std::exception(message.data());
	}
}