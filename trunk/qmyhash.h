#ifndef QMYHASH_H
#define QMYHASH_H

#define HASH_SIZE 273
#define CONST_ALFA 13

#include <QMultiHash>
#include <QString>
uint qHash(){

}

class QMyHash:public QMultiHash<QString,int>
{
public:
    QMyHash();

};



#endif // QMYHASH_H
