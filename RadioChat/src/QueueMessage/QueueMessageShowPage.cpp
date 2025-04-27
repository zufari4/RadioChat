#include "QueueMessageShowPage.h"

QueueMessageShowPage::QueueMessageShowPage(UIPageType pageType)
    : QueueMessageBase(QueueMessageType::ShowPage)
    , pageType_(pageType)
{
}

QueueMessageShowPage::~QueueMessageShowPage() = default;

UIPageType QueueMessageShowPage::getPageType()
{
    return pageType_;
}
