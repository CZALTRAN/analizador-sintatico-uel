#ifndef ARVORESINTATICA_H
#define ARVORESINTATICA_H

#include <QMap>
#include <QList>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "notype.h"
class ArvoreSintatica
{
public:
    ArvoreSintatica();
    NoType * getRaiz();
    void setRaiz(NoType * no);
    void print();
    void toTree(QTreeWidgetItem * item);
private:
    NoType * raiz;
};

#endif // ARVORESINTATICA_H
