#include "qmyhash.h"

uint qHash(KeyHash k){
    int ret = 0;
    int ascii = 0;

    for (int i = 0; i < k.key.size();i++)
    {
        ascii = (int) k.key[i].toAscii();
        ret = ret*CONST_ALFA + ascii;
    }

    //resto de h / M
    ret = ret % HASH_SIZE;

    return ret;
}

KeyHash::KeyHash(QString k):key(k){}
bool KeyHash::operator ==(const KeyHash &outro)const
{
    return outro.key==this->key;
}

QMyHash::QMyHash():QMultiHash<KeyHash,HashConteudo*>()
{
    this->reserve(HASH_SIZE);
}

bool QMyHash::exists(QString identificador, int escopo, int nvLexico)
{
    QHash<KeyHash,HashConteudo*>::const_iterator it = this->constFind(KeyHash(identificador));
    bool find = false;
    for(;it!=this->constEnd() && !find;it++){
        HashConteudo * temp = it.value();
        if(temp->compara(identificador,nvLexico,escopo))
        {
            find = true;
        }
    }
    return find;
}
void QMyHash::print()
{
    QHash<KeyHash,HashConteudo*>::const_iterator it  = this->constBegin();
    for(;it!= this->constEnd();it++){
        qDebug()<<it.key().key<<" : "<<it.value()->toString();
    }
}
