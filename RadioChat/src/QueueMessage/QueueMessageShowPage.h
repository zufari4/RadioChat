#pragma once

#include "QueueMessageBase.h"
#include "../UI/UIPageType.h"
#include <memory>

class QueueMessageShowPage : public QueueMessageBase
{
public:
    QueueMessageShowPage(UIPageType pageType);
    ~QueueMessageShowPage();
    UIPageType getPageType();

private:
    UIPageType pageType_;
};