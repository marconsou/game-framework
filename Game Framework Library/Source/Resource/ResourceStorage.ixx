export module ResourceStorage;

import <memory>;
import <string_view>;
import <unordered_map>;

export namespace gfl
{
	template<typename T>
	class ResourceStorage
	{
	public:
		const T* AddResource(std::string_view resourceName, std::unique_ptr<T> resource)
		{
			const auto element = this->resources.emplace(resourceName.data(), std::move(resource));
			return element.second ? element.first->second.get() : nullptr;
		}

		void RemoveResource(std::string_view resourceName)
		{
			this->resources.erase(resourceName.data());
		}

		const T* GetResource(std::string_view resourceName) const
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