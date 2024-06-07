export module ResourceStorage;

import std;

export namespace gfl
{
	template<typename T>
	class ResourceStorage
	{
	public:
		void Add(std::string_view resourceName, std::unique_ptr<T> resource)
		{
			this->resources.emplace(resourceName.data(), std::move(resource));
		}

		void Remove(std::string_view resourceName)
		{
			this->resources.erase(resourceName.data());
		}

		const T* Get(std::string_view resourceName) const
		{
			const auto element = this->resources.find(resourceName.data());
			return element != this->resources.end() ? element->second.get() : nullptr;
		}

		void Clear()
		{
			this->resources.clear();
		}
	private:
		std::unordered_map<std::string, std::unique_ptr<T>> resources;
	};
}