#include "arvoresintatica.h"

ArvoreSintatica::ArvoreSintatica()
{
    this->raiz = NULL;
}
NoType * ArvoreSintatica::getRaiz()
{
    return this->raiz;
}
void ArvoreSintatica::setRaiz(NoType * no)
{
    this->raiz = no;
}
void ArvoreSintatica::print(){
    this->raiz->print();
}
void ArvoreSintatica::toTree(QTreeWidgetItem * item)
{
    this->raiz->toTree(item);
}


