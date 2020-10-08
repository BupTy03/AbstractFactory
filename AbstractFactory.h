#ifndef ABSTRACTFACTORY_ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_ABSTRACTFACTORY_H

#include <utility>


template<typename AbstractProduct,
        typename IdentifierType,
        typename ProductCreator,
        template<typename...> typename AssociativeContainer
        >
class Factory
{
public:
    bool Register(const IdentifierType& id, ProductCreator&& creator)
    {
        return associations_.emplace(id, std::forward<ProductCreator>(creator)).second;
    }

    bool Unregister(const IdentifierType& id)
    {
        return associations_.erase(id) == 1;
    }

    template<typename... Args>
    AbstractProduct* Create(const IdentifierType& id, Args&&... args) const
    {
        auto it = associations_.find(id);
        if(it == associations_.end())
            return nullptr;

        return it->second(std::forward<Args>(args)...);
    }

private:
    AssociativeContainer<IdentifierType, ProductCreator> associations_;
};


#endif //ABSTRACTFACTORY_ABSTRACTFACTORY_H
