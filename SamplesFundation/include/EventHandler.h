#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <vector>

template <
    typename EventSender,
    typename EventArgs
>
class EventHandler
{
public:
    typedef void (DelegateSignature)(EventSender*, EventArgs&);
    typedef std::function<DelegateSignature> Delegate;

    void add(const Delegate& delegate)
    {
        mDelegates.push_back(delegate);
    }
    void raise(EventSender* sender, EventArgs& args)
    {
        std::for_each(mDelegates.begin(), mDelegates.end(), [=, &args](const Delegate& d){d(sender, args);});
    }
protected:
    std::vector< Delegate > mDelegates;
};

template <
    typename EventData,
    size_t DataSize
>
class EventQueue
{
public:
    EventQueue()
    : mToWrite(0)
    , mReaded(DataSize - 1)
    {

    }

    bool read(EventData& data)
    {
        size_t toRead = (mReaded + 1) % DataSize;
        if (toRead != mToWrite)
        {
            data = mData[toRead];
            mReaded = toRead;
            return true;
        }
        return false;
    }

    bool write(const EventData& data)
    {
        if (mToWrite != mReaded)
        {
            mData[mToWrite] = data;
            mToWrite = (mToWrite + 1) % DataSize;
            return true;
        }
        return false;
    }

private:
    std::array<EventData, 10> mData;
    size_t mToWrite;
    size_t mReaded;
};

#endif /* EVENTHANDLER_H_ */
