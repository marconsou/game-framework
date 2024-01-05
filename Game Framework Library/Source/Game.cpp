module Game;

namespace gfl
{
	void Game::Run(const Color& clearColor)
	{
		this->GlobalTimer.Tick([&]() { this->Update(this->GlobalTimer); });

		//Audio

		if (this->GlobalTimer.GetFrameCount() == 0)
			return;

		if (this->Video)
			this->Video->Render(clearColor);
	}

	void Game::OnActivated()
	{
		this->GlobalTimer.Resume();
	}

	void Game::OnDeactivated()
	{
		this->GlobalTimer.Pause();
	}

	void Game::OnResuming()
	{
		this->GlobalTimer.ResetElapsedTime();
	}

	void Game::OnSuspending()
	{

	}

	void Game::OnQuit()
	{

	}

	void Game::OnWindowSizeChanged(int width, int height)
	{
		if (this->Video && !this->Video->OnWindowSizeChanged(width, height))
			return;

		this->CreateWindowSizeDependentResources();
	}

	void Game::OnWindowMoved()
	{
		if (this->Video)
			this->Video->OnWindowMoved();
	}

	void Game::OnDisplayChange()
	{
		if (this->Video)
			this->Video->OnDisplayChange();
	}

	void Game::OnDeviceLost()
	{

	}

	void Game::OnDeviceRestored()
	{
		this->CreateDeviceDependentResources();
		this->CreateWindowSizeDependentResources();
	}

	void Game::CreateDeviceDependentResources()
	{

	}

	void Game::CreateWindowSizeDependentResources()
	{

	}

	void Game::Update(const StepTimer& timer)
	{

	}
}