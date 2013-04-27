#ifndef __FUNDATION__CCEVENT_H__
#define __FUNDATION__CCEVENT_H__

#include <algorithm>
#include <array>
#include <functional>
#include <list>
#include <tuple>
#include <typeinfo>

#include "CCString.h"

template <
    typename EventSender,
    typename EventArg
>
class CCEvent
{
public:
    typedef void (DelegateSignature)(EventSender*, const EventArg&);
    typedef ::std::function<DelegateSignature> Delegate;
    typedef ::std::tuple<Delegate, CCString> DelegateInfo;

    typedef CCEvent<EventSender, EventArg> ThisType;

    ThisType& add(const Delegate& delegate)
    {
        return add(delegate, delegate.target_type().name());
    }

    ThisType& add(const Delegate& delegate, const CCString& name)
    {
        _delegates.emplace_back(delegate, name);
        return *this;
    }

    Delegate* get(const CCString& name)
    {
        Delegate* ret = nullptr;
        for(auto it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            if (::std::get<1>(*it) != name) continue;
            ret = &::std::get<0>(*it);
        }
        return ret;
    }

    void raise(EventSender* sender, const EventArg& args)
    {
        ::std::for_each(_delegates.begin(), _delegates.end(),[=, & args](const DelegateInfo& info)
        {
            ::std::get<0>(info)(sender, args);
        });
    }

    ThisType& remove(const CCString& name)
    {
        for(auto it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            if (::std::get<1>(*it) != name) continue;
            _delegates.erase(it);
            break;
        };
        return *this;
    }

protected:
    ::std::list<DelegateInfo> _delegates;
};

void CCEventTest();

#endif /* __FUNDATION__CCEVENT_H__ */
