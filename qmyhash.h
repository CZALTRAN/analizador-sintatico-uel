#ifndef QMYHASH_H
#define QMYHASH_H

#define HASH_SIZE 273
#define CONST_ALFA 13

#include <QMultiHash>
#include <QString>
#include "hashconteudo.h"
#include <QDebug>


class KeyHash{
public:
    KeyHash(QString k);
    QString key;
    bool operator ==(const KeyHash &outro)const;
};

uint qHash(KeyHash k);

class QMyHash:public QMultiHash<KeyHash,HashConteudo * >
{
public:
    QMyHash();
    bool exists(QString identificador, int escopo, int nvLexico);
    void print();
};



#endif // QMYHASH_H
