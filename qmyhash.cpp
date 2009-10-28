#include "qmyhash.h"


QMyHash::QMyHash():QMultiHash<QString,int>()
{
    this->reserve(HASH_SIZE);
}
