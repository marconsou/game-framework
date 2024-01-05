export module ResourceManager;

export namespace gfl
{
	class ResourceManager
	{
	public:
		ResourceManager() = default;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		virtual ~ResourceManager() = default;
		virtual void Clear() = 0;
	};
}